import socket
import struct
import threading
from queue import SimpleQueue

from src.naobackend.abstractflightlog import AbstractFlightLog
from src.naobackend.log_entry_type import LogEntryType
from src.naobackend.logentryadapter import LogEntryAdapter, LogEntryNonIndexedProtobufAdapter
from src.naobackend.proto import LogEntry


class Flightlog(AbstractFlightLog):
    _log_entry_queue: SimpleQueue[(LogEntryType, LogEntryAdapter)]

    max_items_in_queue = 1000

    def __init__(self, ipaddr: str, port: int = 9995):
        super().__init__()
        self._log_message_subscribers = list()
        self._log_entry_queue = SimpleQueue()
        threading.Thread(target=self._reader, name="FlightLogReader", args=(ipaddr, port), daemon=True).start()
        threading.Thread(target=self._subscriber_updater, name="FlightLogVisualizer", daemon=True).start()

    def _subscriber_updater(self):
        while True:
            entry = self._log_entry_queue.get()

            if entry[0] == LogEntryType.VISUAL_TRANSACTION:
                self._process_subscriber_updates_visualizer_processor(entry[1])
                continue
            if entry[0] == LogEntryType.LOLA_STATUS_INFO:
                self._process_subscriber_updates_lola_connector(entry[1])
                continue
            if entry[0] == LogEntryType.LOG_ENTRY:
                self._process_subscriber_updates_log_entries(entry[1])
                continue

    def _reader(self, ipaddr: str, port: int):
        sock = socket.create_connection((ipaddr, port))
        fp = sock.makefile("rb")

        while True:
            length = struct.unpack("<i", fp.read(4))[0]
            log_entry_pb = LogEntry()
            buf = fp.read(length)

            if len(buf) < length:
                print("Error reading the log entry. Reason unknown!")

            log_entry_pb.ParseFromString(buf)
            log_entry = LogEntryNonIndexedProtobufAdapter(log_entry_pb)

            if log_entry.subsystem() == "VisualizerFileLog" and log_entry.typeinfo() == "Transaction":
                self._log_entry_queue.put((LogEntryType.VISUAL_TRANSACTION, log_entry))
            elif log_entry.subsystem() == "LolaConnector" and log_entry.typeinfo() == "LolaDebugFrame":
                self._log_entry_queue.put((LogEntryType.LOLA_STATUS_INFO, log_entry))
            else:
                self._log_entry_queue.put((LogEntryType.LOG_ENTRY, log_entry))

            while self._log_entry_queue.qsize() > self.max_items_in_queue:
                self._log_entry_queue.get()
