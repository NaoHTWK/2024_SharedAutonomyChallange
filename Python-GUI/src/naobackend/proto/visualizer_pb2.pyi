from google.protobuf.internal import containers as _containers
from google.protobuf.internal import enum_type_wrapper as _enum_type_wrapper
from google.protobuf import descriptor as _descriptor
from google.protobuf import message as _message
from typing import ClassVar as _ClassVar, Iterable as _Iterable, Mapping as _Mapping, Optional as _Optional, Union as _Union

DESCRIPTOR: _descriptor.FileDescriptor

class VisualizerTransactionType(int, metaclass=_enum_type_wrapper.EnumTypeWrapper):
    __slots__ = []
    ABSOLUTE: _ClassVar[VisualizerTransactionType]
    RELATIVE_HEAD: _ClassVar[VisualizerTransactionType]
    RELATIVE_BODY: _ClassVar[VisualizerTransactionType]

class VisualizerReplacement(int, metaclass=_enum_type_wrapper.EnumTypeWrapper):
    __slots__ = []
    NO_REPLACE: _ClassVar[VisualizerReplacement]
    REPLACE: _ClassVar[VisualizerReplacement]
ABSOLUTE: VisualizerTransactionType
RELATIVE_HEAD: VisualizerTransactionType
RELATIVE_BODY: VisualizerTransactionType
NO_REPLACE: VisualizerReplacement
REPLACE: VisualizerReplacement

class Head(_message.Message):
    __slots__ = ["yaw", "pitch"]
    YAW_FIELD_NUMBER: _ClassVar[int]
    PITCH_FIELD_NUMBER: _ClassVar[int]
    yaw: float
    pitch: float
    def __init__(self, yaw: _Optional[float] = ..., pitch: _Optional[float] = ...) -> None: ...

class Parameter(_message.Message):
    __slots__ = ["name", "intParams", "floatParams", "color"]
    NAME_FIELD_NUMBER: _ClassVar[int]
    INTPARAMS_FIELD_NUMBER: _ClassVar[int]
    FLOATPARAMS_FIELD_NUMBER: _ClassVar[int]
    COLOR_FIELD_NUMBER: _ClassVar[int]
    name: str
    intParams: _containers.RepeatedScalarFieldContainer[int]
    floatParams: _containers.RepeatedScalarFieldContainer[float]
    color: Color
    def __init__(self, name: _Optional[str] = ..., intParams: _Optional[_Iterable[int]] = ..., floatParams: _Optional[_Iterable[float]] = ..., color: _Optional[_Union[Color, _Mapping]] = ...) -> None: ...

class Color(_message.Message):
    __slots__ = ["r", "g", "b", "a"]
    R_FIELD_NUMBER: _ClassVar[int]
    G_FIELD_NUMBER: _ClassVar[int]
    B_FIELD_NUMBER: _ClassVar[int]
    A_FIELD_NUMBER: _ClassVar[int]
    r: int
    g: int
    b: int
    a: int
    def __init__(self, r: _Optional[int] = ..., g: _Optional[int] = ..., b: _Optional[int] = ..., a: _Optional[int] = ...) -> None: ...

class Line2D(_message.Message):
    __slots__ = ["color", "fadingTime", "x1", "y1", "x2", "y2", "type"]
    class LineType(int, metaclass=_enum_type_wrapper.EnumTypeWrapper):
        __slots__ = []
        NORMAL: _ClassVar[Line2D.LineType]
        ARROW: _ClassVar[Line2D.LineType]
    NORMAL: Line2D.LineType
    ARROW: Line2D.LineType
    COLOR_FIELD_NUMBER: _ClassVar[int]
    FADINGTIME_FIELD_NUMBER: _ClassVar[int]
    X1_FIELD_NUMBER: _ClassVar[int]
    Y1_FIELD_NUMBER: _ClassVar[int]
    X2_FIELD_NUMBER: _ClassVar[int]
    Y2_FIELD_NUMBER: _ClassVar[int]
    TYPE_FIELD_NUMBER: _ClassVar[int]
    color: Color
    fadingTime: int
    x1: float
    y1: float
    x2: float
    y2: float
    type: Line2D.LineType
    def __init__(self, color: _Optional[_Union[Color, _Mapping]] = ..., fadingTime: _Optional[int] = ..., x1: _Optional[float] = ..., y1: _Optional[float] = ..., x2: _Optional[float] = ..., y2: _Optional[float] = ..., type: _Optional[_Union[Line2D.LineType, str]] = ...) -> None: ...

class Ellipsis2D(_message.Message):
    __slots__ = ["borderColor", "fillColor", "fadingTime", "centerX", "centerY", "width", "height", "angle"]
    BORDERCOLOR_FIELD_NUMBER: _ClassVar[int]
    FILLCOLOR_FIELD_NUMBER: _ClassVar[int]
    FADINGTIME_FIELD_NUMBER: _ClassVar[int]
    CENTERX_FIELD_NUMBER: _ClassVar[int]
    CENTERY_FIELD_NUMBER: _ClassVar[int]
    WIDTH_FIELD_NUMBER: _ClassVar[int]
    HEIGHT_FIELD_NUMBER: _ClassVar[int]
    ANGLE_FIELD_NUMBER: _ClassVar[int]
    borderColor: Color
    fillColor: Color
    fadingTime: int
    centerX: float
    centerY: float
    width: float
    height: float
    angle: float
    def __init__(self, borderColor: _Optional[_Union[Color, _Mapping]] = ..., fillColor: _Optional[_Union[Color, _Mapping]] = ..., fadingTime: _Optional[int] = ..., centerX: _Optional[float] = ..., centerY: _Optional[float] = ..., width: _Optional[float] = ..., height: _Optional[float] = ..., angle: _Optional[float] = ...) -> None: ...

class Rectangle2D(_message.Message):
    __slots__ = ["borderColor", "fillColor", "fadingTime", "upperLeftX", "upperLeftY", "width", "height"]
    BORDERCOLOR_FIELD_NUMBER: _ClassVar[int]
    FILLCOLOR_FIELD_NUMBER: _ClassVar[int]
    FADINGTIME_FIELD_NUMBER: _ClassVar[int]
    UPPERLEFTX_FIELD_NUMBER: _ClassVar[int]
    UPPERLEFTY_FIELD_NUMBER: _ClassVar[int]
    WIDTH_FIELD_NUMBER: _ClassVar[int]
    HEIGHT_FIELD_NUMBER: _ClassVar[int]
    borderColor: Color
    fillColor: Color
    fadingTime: int
    upperLeftX: float
    upperLeftY: float
    width: float
    height: float
    def __init__(self, borderColor: _Optional[_Union[Color, _Mapping]] = ..., fillColor: _Optional[_Union[Color, _Mapping]] = ..., fadingTime: _Optional[int] = ..., upperLeftX: _Optional[float] = ..., upperLeftY: _Optional[float] = ..., width: _Optional[float] = ..., height: _Optional[float] = ...) -> None: ...

class Arc2D(_message.Message):
    __slots__ = ["borderColor", "fillColor", "fadingTime", "centerX", "centerY", "width", "height", "startAngle", "arcAngle", "type"]
    class Arc2DType(int, metaclass=_enum_type_wrapper.EnumTypeWrapper):
        __slots__ = []
        OPEN: _ClassVar[Arc2D.Arc2DType]
        CHORD: _ClassVar[Arc2D.Arc2DType]
        PIE: _ClassVar[Arc2D.Arc2DType]
    OPEN: Arc2D.Arc2DType
    CHORD: Arc2D.Arc2DType
    PIE: Arc2D.Arc2DType
    BORDERCOLOR_FIELD_NUMBER: _ClassVar[int]
    FILLCOLOR_FIELD_NUMBER: _ClassVar[int]
    FADINGTIME_FIELD_NUMBER: _ClassVar[int]
    CENTERX_FIELD_NUMBER: _ClassVar[int]
    CENTERY_FIELD_NUMBER: _ClassVar[int]
    WIDTH_FIELD_NUMBER: _ClassVar[int]
    HEIGHT_FIELD_NUMBER: _ClassVar[int]
    STARTANGLE_FIELD_NUMBER: _ClassVar[int]
    ARCANGLE_FIELD_NUMBER: _ClassVar[int]
    TYPE_FIELD_NUMBER: _ClassVar[int]
    borderColor: Color
    fillColor: Color
    fadingTime: int
    centerX: float
    centerY: float
    width: float
    height: float
    startAngle: float
    arcAngle: float
    type: Arc2D.Arc2DType
    def __init__(self, borderColor: _Optional[_Union[Color, _Mapping]] = ..., fillColor: _Optional[_Union[Color, _Mapping]] = ..., fadingTime: _Optional[int] = ..., centerX: _Optional[float] = ..., centerY: _Optional[float] = ..., width: _Optional[float] = ..., height: _Optional[float] = ..., startAngle: _Optional[float] = ..., arcAngle: _Optional[float] = ..., type: _Optional[_Union[Arc2D.Arc2DType, str]] = ...) -> None: ...

class Text2D(_message.Message):
    __slots__ = ["color", "fadingTime", "x", "y", "size", "text"]
    COLOR_FIELD_NUMBER: _ClassVar[int]
    FADINGTIME_FIELD_NUMBER: _ClassVar[int]
    X_FIELD_NUMBER: _ClassVar[int]
    Y_FIELD_NUMBER: _ClassVar[int]
    SIZE_FIELD_NUMBER: _ClassVar[int]
    TEXT_FIELD_NUMBER: _ClassVar[int]
    color: Color
    fadingTime: int
    x: float
    y: float
    size: float
    text: str
    def __init__(self, color: _Optional[_Union[Color, _Mapping]] = ..., fadingTime: _Optional[int] = ..., x: _Optional[float] = ..., y: _Optional[float] = ..., size: _Optional[float] = ..., text: _Optional[str] = ...) -> None: ...

class Raster2D(_message.Message):
    __slots__ = ["x", "y", "rotation", "scale", "width", "height", "color", "fadingTime", "data"]
    X_FIELD_NUMBER: _ClassVar[int]
    Y_FIELD_NUMBER: _ClassVar[int]
    ROTATION_FIELD_NUMBER: _ClassVar[int]
    SCALE_FIELD_NUMBER: _ClassVar[int]
    WIDTH_FIELD_NUMBER: _ClassVar[int]
    HEIGHT_FIELD_NUMBER: _ClassVar[int]
    COLOR_FIELD_NUMBER: _ClassVar[int]
    FADINGTIME_FIELD_NUMBER: _ClassVar[int]
    DATA_FIELD_NUMBER: _ClassVar[int]
    x: float
    y: float
    rotation: float
    scale: float
    width: int
    height: int
    color: Color
    fadingTime: int
    data: _containers.RepeatedScalarFieldContainer[int]
    def __init__(self, x: _Optional[float] = ..., y: _Optional[float] = ..., rotation: _Optional[float] = ..., scale: _Optional[float] = ..., width: _Optional[int] = ..., height: _Optional[int] = ..., color: _Optional[_Union[Color, _Mapping]] = ..., fadingTime: _Optional[int] = ..., data: _Optional[_Iterable[int]] = ...) -> None: ...

class VisualizerTransaction(_message.Message):
    __slots__ = ["replacement", "transactionType", "time", "subsystem", "messages", "head", "parameter", "lines", "ellipses", "rectangles", "arcs", "texts", "raster"]
    REPLACEMENT_FIELD_NUMBER: _ClassVar[int]
    TRANSACTIONTYPE_FIELD_NUMBER: _ClassVar[int]
    TIME_FIELD_NUMBER: _ClassVar[int]
    SUBSYSTEM_FIELD_NUMBER: _ClassVar[int]
    MESSAGES_FIELD_NUMBER: _ClassVar[int]
    HEAD_FIELD_NUMBER: _ClassVar[int]
    PARAMETER_FIELD_NUMBER: _ClassVar[int]
    LINES_FIELD_NUMBER: _ClassVar[int]
    ELLIPSES_FIELD_NUMBER: _ClassVar[int]
    RECTANGLES_FIELD_NUMBER: _ClassVar[int]
    ARCS_FIELD_NUMBER: _ClassVar[int]
    TEXTS_FIELD_NUMBER: _ClassVar[int]
    RASTER_FIELD_NUMBER: _ClassVar[int]
    replacement: VisualizerReplacement
    transactionType: VisualizerTransactionType
    time: int
    subsystem: str
    messages: _containers.RepeatedScalarFieldContainer[str]
    head: Head
    parameter: _containers.RepeatedCompositeFieldContainer[Parameter]
    lines: _containers.RepeatedCompositeFieldContainer[Line2D]
    ellipses: _containers.RepeatedCompositeFieldContainer[Ellipsis2D]
    rectangles: _containers.RepeatedCompositeFieldContainer[Rectangle2D]
    arcs: _containers.RepeatedCompositeFieldContainer[Arc2D]
    texts: _containers.RepeatedCompositeFieldContainer[Text2D]
    raster: _containers.RepeatedCompositeFieldContainer[Raster2D]
    def __init__(self, replacement: _Optional[_Union[VisualizerReplacement, str]] = ..., transactionType: _Optional[_Union[VisualizerTransactionType, str]] = ..., time: _Optional[int] = ..., subsystem: _Optional[str] = ..., messages: _Optional[_Iterable[str]] = ..., head: _Optional[_Union[Head, _Mapping]] = ..., parameter: _Optional[_Iterable[_Union[Parameter, _Mapping]]] = ..., lines: _Optional[_Iterable[_Union[Line2D, _Mapping]]] = ..., ellipses: _Optional[_Iterable[_Union[Ellipsis2D, _Mapping]]] = ..., rectangles: _Optional[_Iterable[_Union[Rectangle2D, _Mapping]]] = ..., arcs: _Optional[_Iterable[_Union[Arc2D, _Mapping]]] = ..., texts: _Optional[_Iterable[_Union[Text2D, _Mapping]]] = ..., raster: _Optional[_Iterable[_Union[Raster2D, _Mapping]]] = ...) -> None: ...
