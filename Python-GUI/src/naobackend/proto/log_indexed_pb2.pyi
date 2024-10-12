import log_pb2 as _log_pb2
from google.protobuf.internal import containers as _containers
from google.protobuf.internal import enum_type_wrapper as _enum_type_wrapper
from google.protobuf import descriptor as _descriptor
from google.protobuf import message as _message
from typing import ClassVar as _ClassVar, Iterable as _Iterable, Mapping as _Mapping, Optional as _Optional, Union as _Union

DESCRIPTOR: _descriptor.FileDescriptor

class ChunkType(int, metaclass=_enum_type_wrapper.EnumTypeWrapper):
    __slots__ = []
    PLAIN: _ClassVar[ChunkType]
    ZSTD: _ClassVar[ChunkType]
PLAIN: ChunkType
ZSTD: ChunkType

class IndexedLogEntry(_message.Message):
    __slots__ = ["chunk", "offset", "timestamp", "level", "type", "src", "subsystem", "typeinfo", "size"]
    CHUNK_FIELD_NUMBER: _ClassVar[int]
    OFFSET_FIELD_NUMBER: _ClassVar[int]
    TIMESTAMP_FIELD_NUMBER: _ClassVar[int]
    LEVEL_FIELD_NUMBER: _ClassVar[int]
    TYPE_FIELD_NUMBER: _ClassVar[int]
    SRC_FIELD_NUMBER: _ClassVar[int]
    SUBSYSTEM_FIELD_NUMBER: _ClassVar[int]
    TYPEINFO_FIELD_NUMBER: _ClassVar[int]
    SIZE_FIELD_NUMBER: _ClassVar[int]
    chunk: int
    offset: int
    timestamp: int
    level: _log_pb2.LogLevel
    type: _log_pb2.LogType
    src: _log_pb2.LogSource
    subsystem: str
    typeinfo: str
    size: int
    def __init__(self, chunk: _Optional[int] = ..., offset: _Optional[int] = ..., timestamp: _Optional[int] = ..., level: _Optional[_Union[_log_pb2.LogLevel, str]] = ..., type: _Optional[_Union[_log_pb2.LogType, str]] = ..., src: _Optional[_Union[_log_pb2.LogSource, str]] = ..., subsystem: _Optional[str] = ..., typeinfo: _Optional[str] = ..., size: _Optional[int] = ...) -> None: ...

class Chunk(_message.Message):
    __slots__ = ["id", "type", "offset", "size"]
    ID_FIELD_NUMBER: _ClassVar[int]
    TYPE_FIELD_NUMBER: _ClassVar[int]
    OFFSET_FIELD_NUMBER: _ClassVar[int]
    SIZE_FIELD_NUMBER: _ClassVar[int]
    id: int
    type: ChunkType
    offset: int
    size: int
    def __init__(self, id: _Optional[int] = ..., type: _Optional[_Union[ChunkType, str]] = ..., offset: _Optional[int] = ..., size: _Optional[int] = ...) -> None: ...

class IndexData(_message.Message):
    __slots__ = ["version", "chunks", "entries"]
    VERSION_FIELD_NUMBER: _ClassVar[int]
    CHUNKS_FIELD_NUMBER: _ClassVar[int]
    ENTRIES_FIELD_NUMBER: _ClassVar[int]
    version: int
    chunks: _containers.RepeatedCompositeFieldContainer[Chunk]
    entries: _containers.RepeatedCompositeFieldContainer[IndexedLogEntry]
    def __init__(self, version: _Optional[int] = ..., chunks: _Optional[_Iterable[_Union[Chunk, _Mapping]]] = ..., entries: _Optional[_Iterable[_Union[IndexedLogEntry, _Mapping]]] = ...) -> None: ...
