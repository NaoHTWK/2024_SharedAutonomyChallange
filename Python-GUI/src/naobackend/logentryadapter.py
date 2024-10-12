from src.naobackend.indexflightlog import IndexFlightlogFile
from src.naobackend.proto import IndexedLogEntry
from src.naobackend.proto import LogLevel, LogType, LogSource, LogEntry


class LogEntryAdapter:
    def logEntry(self) -> bytes:
        pass

    def logLevel(self) -> LogLevel:
        pass

    def logType(self) -> LogType:
        pass

    def size(self) -> int:
        pass

    def src(self) -> LogSource:
        pass

    def subsystem(self) -> str:
        pass

    def typeinfo(self) -> str:
        pass

    def timestamp(self) -> int:
        pass

class LogEntryNonIndexedProtobufAdapter(LogEntryAdapter):
    def __init__(self, log_entry: LogEntry):
        self.__l = log_entry

    def logEntry(self) -> bytes:
        return self.__l.logEntry

    def logLevel(self) -> LogLevel:
        return self.__l.logLevel

    def size(self) -> int:
        return self.__l.size

    def src(self) -> LogSource:
        return self.__l.src

    def subsystem(self) -> str:
        return self.__l.subsystem

    def typeinfo(self) -> str:
        return self.__l.typeinfo

    def timestamp(self) -> int:
        return self.__l.timestamp

class LogEntryIndexedProtobufAdapter(LogEntryAdapter):
    flightlog: IndexFlightlogFile

    def __init__(self, flightlog: IndexFlightlogFile, log_entry: IndexedLogEntry):
        self.__l = log_entry
        self.flightlog = flightlog

    def logEntry(self) -> bytes:
        return self.flightlog.get_logentry_bytes(self.__l)

    def logLevel(self) -> LogLevel:
        return self.__l.level

    def size(self) -> int:
        return self.__l.size

    def src(self) -> LogSource:
        return self.__l.src

    def subsystem(self) -> str:
        return self.__l.subsystem

    def typeinfo(self) -> str:
        return self.__l.typeinfo

    def timestamp(self) -> int:
        return self.__l.timestamp