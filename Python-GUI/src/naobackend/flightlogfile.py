import struct
import zlib
from typing import Callable

from typing.io import BinaryIO

from src.naobackend.abstractflightlog import AbstractFlightLog
from src.naobackend.logentryadapter import LogEntryAdapter, LogEntryNonIndexedProtobufAdapter
from src.naobackend.proto import LogEntry


class _FlightlogFile:
    def __init__(self):
        pass

    def read(self, size: int) -> bytes:
        pass


class _FlightlogFileRaw(_FlightlogFile):
    __fp: BinaryIO

    def __init__(self, filename: str):
        super().__init__()
        self.__fp = open(filename, "rb")

    def read(self, size: int) -> bytes:
        return self.__fp.read(size)


class _FlightlogFileZlibCompressed(_FlightlogFile):
    __fp: BinaryIO
    __data: bytes

    def __init__(self, filename: str):
        super().__init__()
        self.__fp = open(filename, "rb")
        self.__decompressor = zlib.decompressobj()
        self.__data = bytes()

    def __enter__(self):
        return self

    def __exit__(self, exc_type, exc_val, exc_tb):
        self.__fp.close()

    def read(self, length: int) -> bytes:
        if len(self.__data) >= length:
            data = self.__data[:length]
            self.__data = self.__data[length:]
            return data

        while len(self.__data) < length:
            data = self.__fp.read(1024 * 1024)

            if len(data) == 0:
                if len(self.__data) > 0:
                    data = self.__data;
                    self.__data = bytes()
                    return data;
                else:
                    return bytes()

            self.__data += self.__decompressor.decompress(data)

        data = self.__data[:length]
        self.__data = self.__data[length:]
        return data


class FlightlogFile(AbstractFlightLog):
    _fp: _FlightlogFile

    def __init__(self, filename: str):
        super().__init__()
        self.parameter = dict()
        if filename.endswith(".z"):
            self._fp = _FlightlogFileZlibCompressed(filename)
        else:
            self._fp = _FlightlogFileRaw(filename)

    def filter(self, filter_cb: Callable[[LogEntryAdapter], bool]) -> [LogEntryAdapter]:
        log_entries = []
        while True:
            log_entry = self.next()
            if log_entry is None:
                break

            if filter_cb(log_entry):
                log_entries.append(log_entry)

        return log_entries

    def next(self) -> LogEntryAdapter | None:
        log_entry_length = self._fp.read(4)

        if len(log_entry_length) == 0:
            return None

        length = struct.unpack("<i", log_entry_length)[0]
        log_entry_pb = LogEntry()
        log_entry = LogEntryAdapter()
        buf = self._fp.read(length)

        if len(buf) == 0:
            return None

        if len(buf) < length:
            print("Error reading the log entry. Reason unknown!")

        log_entry_pb.ParseFromString(buf)
        log_entry = LogEntryNonIndexedProtobufAdapter(log_entry_pb)

        if log_entry.subsystem() == "VisualizerFileLog" and log_entry.typeinfo() == "Transaction":
            self._process_subscriber_updates_visualizer_processor(log_entry)
        elif log_entry.subsystem() == "LolaConnector" and log_entry.typeinfo() == "LolaDebugFrame":
            self._process_subscriber_updates_lola_connector(log_entry)
        else:
            self._process_subscriber_updates_log_entries(log_entry)

        return log_entry

    def read(self) -> [LogEntryAdapter]:
        log_entries = []
        while True:
            log_entry = self.next()
            if log_entry is None:
                break
            log_entries.append(log_entry)

        return log_entries

    def process(self):
        while True:
            log_entry = self.next()
            if log_entry is None:
                break
