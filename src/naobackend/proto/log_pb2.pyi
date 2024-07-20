from google.protobuf.internal import enum_type_wrapper as _enum_type_wrapper
from google.protobuf import descriptor as _descriptor
from google.protobuf import message as _message
from typing import ClassVar as _ClassVar, Optional as _Optional, Union as _Union

DESCRIPTOR: _descriptor.FileDescriptor

class LogType(int, metaclass=_enum_type_wrapper.EnumTypeWrapper):
    __slots__ = []
    TEXT: _ClassVar[LogType]
    PROTOBUF: _ClassVar[LogType]
    BINARY: _ClassVar[LogType]

class LogLevel(int, metaclass=_enum_type_wrapper.EnumTypeWrapper):
    __slots__ = []
    INFO: _ClassVar[LogLevel]
    DEBUG: _ClassVar[LogLevel]
    WARN: _ClassVar[LogLevel]
    ERROR: _ClassVar[LogLevel]

class LogSource(int, metaclass=_enum_type_wrapper.EnumTypeWrapper):
    __slots__ = []
    FIRMWARE: _ClassVar[LogSource]
    BRIDGE: _ClassVar[LogSource]
TEXT: LogType
PROTOBUF: LogType
BINARY: LogType
INFO: LogLevel
DEBUG: LogLevel
WARN: LogLevel
ERROR: LogLevel
FIRMWARE: LogSource
BRIDGE: LogSource

class LogEntry(_message.Message):
    __slots__ = ["timestamp", "logLevel", "logType", "subsystem", "size", "logEntry", "typeinfo", "src"]
    TIMESTAMP_FIELD_NUMBER: _ClassVar[int]
    LOGLEVEL_FIELD_NUMBER: _ClassVar[int]
    LOGTYPE_FIELD_NUMBER: _ClassVar[int]
    SUBSYSTEM_FIELD_NUMBER: _ClassVar[int]
    SIZE_FIELD_NUMBER: _ClassVar[int]
    LOGENTRY_FIELD_NUMBER: _ClassVar[int]
    TYPEINFO_FIELD_NUMBER: _ClassVar[int]
    SRC_FIELD_NUMBER: _ClassVar[int]
    timestamp: int
    logLevel: LogLevel
    logType: LogType
    subsystem: str
    size: int
    logEntry: bytes
    typeinfo: str
    src: LogSource
    def __init__(self, timestamp: _Optional[int] = ..., logLevel: _Optional[_Union[LogLevel, str]] = ..., logType: _Optional[_Union[LogType, str]] = ..., subsystem: _Optional[str] = ..., size: _Optional[int] = ..., logEntry: _Optional[bytes] = ..., typeinfo: _Optional[str] = ..., src: _Optional[_Union[LogSource, str]] = ...) -> None: ...
