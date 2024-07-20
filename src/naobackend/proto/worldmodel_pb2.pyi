from google.protobuf.internal import containers as _containers
from google.protobuf.internal import enum_type_wrapper as _enum_type_wrapper
from google.protobuf import descriptor as _descriptor
from google.protobuf import message as _message
from typing import ClassVar as _ClassVar, Iterable as _Iterable, Mapping as _Mapping, Optional as _Optional, Union as _Union

DESCRIPTOR: _descriptor.FileDescriptor

class CameraState(int, metaclass=_enum_type_wrapper.EnumTypeWrapper):
    __slots__ = []
    OK: _ClassVar[CameraState]
    DIRTY: _ClassVar[CameraState]
    SAME_PICTURE: _ClassVar[CameraState]
    WDG_TRIGGERED: _ClassVar[CameraState]
    SCRAMBLED: _ClassVar[CameraState]
OK: CameraState
DIRTY: CameraState
SAME_PICTURE: CameraState
WDG_TRIGGERED: CameraState
SCRAMBLED: CameraState

class Point2D(_message.Message):
    __slots__ = ["x", "y"]
    X_FIELD_NUMBER: _ClassVar[int]
    Y_FIELD_NUMBER: _ClassVar[int]
    x: float
    y: float
    def __init__(self, x: _Optional[float] = ..., y: _Optional[float] = ...) -> None: ...

class RelativeBall(_message.Message):
    __slots__ = ["pos", "speed", "lastSeen"]
    POS_FIELD_NUMBER: _ClassVar[int]
    SPEED_FIELD_NUMBER: _ClassVar[int]
    LASTSEEN_FIELD_NUMBER: _ClassVar[int]
    pos: Point2D
    speed: Point2D
    lastSeen: int
    def __init__(self, pos: _Optional[_Union[Point2D, _Mapping]] = ..., speed: _Optional[_Union[Point2D, _Mapping]] = ..., lastSeen: _Optional[int] = ...) -> None: ...

class LocationInfo(_message.Message):
    __slots__ = ["x", "y", "a", "qual"]
    X_FIELD_NUMBER: _ClassVar[int]
    Y_FIELD_NUMBER: _ClassVar[int]
    A_FIELD_NUMBER: _ClassVar[int]
    QUAL_FIELD_NUMBER: _ClassVar[int]
    x: float
    y: float
    a: float
    qual: float
    def __init__(self, x: _Optional[float] = ..., y: _Optional[float] = ..., a: _Optional[float] = ..., qual: _Optional[float] = ...) -> None: ...

class Position(_message.Message):
    __slots__ = ["x", "y", "a"]
    X_FIELD_NUMBER: _ClassVar[int]
    Y_FIELD_NUMBER: _ClassVar[int]
    A_FIELD_NUMBER: _ClassVar[int]
    x: float
    y: float
    a: float
    def __init__(self, x: _Optional[float] = ..., y: _Optional[float] = ..., a: _Optional[float] = ...) -> None: ...

class Teamball(_message.Message):
    __slots__ = ["pos", "qual", "trusted"]
    POS_FIELD_NUMBER: _ClassVar[int]
    QUAL_FIELD_NUMBER: _ClassVar[int]
    TRUSTED_FIELD_NUMBER: _ClassVar[int]
    pos: Point2D
    qual: float
    trusted: bool
    def __init__(self, pos: _Optional[_Union[Point2D, _Mapping]] = ..., qual: _Optional[float] = ..., trusted: bool = ...) -> None: ...

class Obstacle(_message.Message):
    __slots__ = ["pos", "confindence", "ownTeamProb"]
    POS_FIELD_NUMBER: _ClassVar[int]
    CONFINDENCE_FIELD_NUMBER: _ClassVar[int]
    OWNTEAMPROB_FIELD_NUMBER: _ClassVar[int]
    pos: Point2D
    confindence: float
    ownTeamProb: float
    def __init__(self, pos: _Optional[_Union[Point2D, _Mapping]] = ..., confindence: _Optional[float] = ..., ownTeamProb: _Optional[float] = ...) -> None: ...

class MoveBallOrder(_message.Message):
    __slots__ = ["pos"]
    POS_FIELD_NUMBER: _ClassVar[int]
    pos: Point2D
    def __init__(self, pos: _Optional[_Union[Point2D, _Mapping]] = ...) -> None: ...

class KeepGoalOrder(_message.Message):
    __slots__ = ["allowedToMove"]
    ALLOWEDTOMOVE_FIELD_NUMBER: _ClassVar[int]
    allowedToMove: bool
    def __init__(self, allowedToMove: bool = ...) -> None: ...

class WalkToPositionOrder(_message.Message):
    __slots__ = ["pos", "useA", "mode", "obstacles", "head_focus"]
    class Mode(int, metaclass=_enum_type_wrapper.EnumTypeWrapper):
        __slots__ = []
        MODE_USE_A: _ClassVar[WalkToPositionOrder.Mode]
        MODE_STRIKER: _ClassVar[WalkToPositionOrder.Mode]
        MODE_SUPPORTER: _ClassVar[WalkToPositionOrder.Mode]
        MODE_KEEP_A: _ClassVar[WalkToPositionOrder.Mode]
        MODE_FOCUS_DIRECTION: _ClassVar[WalkToPositionOrder.Mode]
    MODE_USE_A: WalkToPositionOrder.Mode
    MODE_STRIKER: WalkToPositionOrder.Mode
    MODE_SUPPORTER: WalkToPositionOrder.Mode
    MODE_KEEP_A: WalkToPositionOrder.Mode
    MODE_FOCUS_DIRECTION: WalkToPositionOrder.Mode
    class HeadFocus(int, metaclass=_enum_type_wrapper.EnumTypeWrapper):
        __slots__ = []
        FOCUS_NOTHING: _ClassVar[WalkToPositionOrder.HeadFocus]
        FOCUS_LOC: _ClassVar[WalkToPositionOrder.HeadFocus]
        FOCUS_BALL: _ClassVar[WalkToPositionOrder.HeadFocus]
        FOCUS_BALL_SEARCH_LEFT: _ClassVar[WalkToPositionOrder.HeadFocus]
        FOCUS_BALL_SEARCH_RIGHT: _ClassVar[WalkToPositionOrder.HeadFocus]
        FOCUS_BALL_GOALIE: _ClassVar[WalkToPositionOrder.HeadFocus]
        FOCUS_OBSTACLES: _ClassVar[WalkToPositionOrder.HeadFocus]
    FOCUS_NOTHING: WalkToPositionOrder.HeadFocus
    FOCUS_LOC: WalkToPositionOrder.HeadFocus
    FOCUS_BALL: WalkToPositionOrder.HeadFocus
    FOCUS_BALL_SEARCH_LEFT: WalkToPositionOrder.HeadFocus
    FOCUS_BALL_SEARCH_RIGHT: WalkToPositionOrder.HeadFocus
    FOCUS_BALL_GOALIE: WalkToPositionOrder.HeadFocus
    FOCUS_OBSTACLES: WalkToPositionOrder.HeadFocus
    POS_FIELD_NUMBER: _ClassVar[int]
    USEA_FIELD_NUMBER: _ClassVar[int]
    MODE_FIELD_NUMBER: _ClassVar[int]
    OBSTACLES_FIELD_NUMBER: _ClassVar[int]
    HEAD_FOCUS_FIELD_NUMBER: _ClassVar[int]
    pos: Position
    useA: bool
    mode: WalkToPositionOrder.Mode
    obstacles: _containers.RepeatedCompositeFieldContainer[Point2D]
    head_focus: WalkToPositionOrder.HeadFocus
    def __init__(self, pos: _Optional[_Union[Position, _Mapping]] = ..., useA: bool = ..., mode: _Optional[_Union[WalkToPositionOrder.Mode, str]] = ..., obstacles: _Optional[_Iterable[_Union[Point2D, _Mapping]]] = ..., head_focus: _Optional[_Union[WalkToPositionOrder.HeadFocus, str]] = ...) -> None: ...

class MoveBallGoalOrder(_message.Message):
    __slots__ = ["y"]
    Y_FIELD_NUMBER: _ClassVar[int]
    y: float
    def __init__(self, y: _Optional[float] = ...) -> None: ...

class Order(_message.Message):
    __slots__ = ["name", "moveBallOrder", "walkToPositionOrder", "moveBallGoalOrder", "keepGoalOrder"]
    NAME_FIELD_NUMBER: _ClassVar[int]
    MOVEBALLORDER_FIELD_NUMBER: _ClassVar[int]
    WALKTOPOSITIONORDER_FIELD_NUMBER: _ClassVar[int]
    MOVEBALLGOALORDER_FIELD_NUMBER: _ClassVar[int]
    KEEPGOALORDER_FIELD_NUMBER: _ClassVar[int]
    name: str
    moveBallOrder: MoveBallOrder
    walkToPositionOrder: WalkToPositionOrder
    moveBallGoalOrder: MoveBallGoalOrder
    keepGoalOrder: KeepGoalOrder
    def __init__(self, name: _Optional[str] = ..., moveBallOrder: _Optional[_Union[MoveBallOrder, _Mapping]] = ..., walkToPositionOrder: _Optional[_Union[WalkToPositionOrder, _Mapping]] = ..., moveBallGoalOrder: _Optional[_Union[MoveBallGoalOrder, _Mapping]] = ..., keepGoalOrder: _Optional[_Union[KeepGoalOrder, _Mapping]] = ...) -> None: ...

class WorldModel(_message.Message):
    __slots__ = ["pid", "time", "ball", "pos", "isFallen", "penalized", "obstacles"]
    PID_FIELD_NUMBER: _ClassVar[int]
    TIME_FIELD_NUMBER: _ClassVar[int]
    BALL_FIELD_NUMBER: _ClassVar[int]
    POS_FIELD_NUMBER: _ClassVar[int]
    ISFALLEN_FIELD_NUMBER: _ClassVar[int]
    PENALIZED_FIELD_NUMBER: _ClassVar[int]
    OBSTACLES_FIELD_NUMBER: _ClassVar[int]
    pid: int
    time: int
    ball: RelativeBall
    pos: LocationInfo
    isFallen: bool
    penalized: bool
    obstacles: _containers.RepeatedCompositeFieldContainer[Obstacle]
    def __init__(self, pid: _Optional[int] = ..., time: _Optional[int] = ..., ball: _Optional[_Union[RelativeBall, _Mapping]] = ..., pos: _Optional[_Union[LocationInfo, _Mapping]] = ..., isFallen: bool = ..., penalized: bool = ..., obstacles: _Optional[_Iterable[_Union[Obstacle, _Mapping]]] = ...) -> None: ...

class RobotMetaInfo(_message.Message):
    __slots__ = ["hostname", "battery", "wifiStrength", "cpuTemp", "lanIp", "lanMac", "wifiIp", "wifiMac", "jointTemperatureMax", "jointTemperatureName"]
    HOSTNAME_FIELD_NUMBER: _ClassVar[int]
    BATTERY_FIELD_NUMBER: _ClassVar[int]
    WIFISTRENGTH_FIELD_NUMBER: _ClassVar[int]
    CPUTEMP_FIELD_NUMBER: _ClassVar[int]
    LANIP_FIELD_NUMBER: _ClassVar[int]
    LANMAC_FIELD_NUMBER: _ClassVar[int]
    WIFIIP_FIELD_NUMBER: _ClassVar[int]
    WIFIMAC_FIELD_NUMBER: _ClassVar[int]
    JOINTTEMPERATUREMAX_FIELD_NUMBER: _ClassVar[int]
    JOINTTEMPERATURENAME_FIELD_NUMBER: _ClassVar[int]
    hostname: str
    battery: int
    wifiStrength: int
    cpuTemp: int
    lanIp: int
    lanMac: bytes
    wifiIp: int
    wifiMac: bytes
    jointTemperatureMax: float
    jointTemperatureName: str
    def __init__(self, hostname: _Optional[str] = ..., battery: _Optional[int] = ..., wifiStrength: _Optional[int] = ..., cpuTemp: _Optional[int] = ..., lanIp: _Optional[int] = ..., lanMac: _Optional[bytes] = ..., wifiIp: _Optional[int] = ..., wifiMac: _Optional[bytes] = ..., jointTemperatureMax: _Optional[float] = ..., jointTemperatureName: _Optional[str] = ...) -> None: ...

class DebugData(_message.Message):
    __slots__ = ["agent", "order", "teamball", "whistleTimestamp", "stateLowerCam", "stateUpperCam"]
    AGENT_FIELD_NUMBER: _ClassVar[int]
    ORDER_FIELD_NUMBER: _ClassVar[int]
    TEAMBALL_FIELD_NUMBER: _ClassVar[int]
    WHISTLETIMESTAMP_FIELD_NUMBER: _ClassVar[int]
    STATELOWERCAM_FIELD_NUMBER: _ClassVar[int]
    STATEUPPERCAM_FIELD_NUMBER: _ClassVar[int]
    agent: str
    order: Order
    teamball: Teamball
    whistleTimestamp: int
    stateLowerCam: CameraState
    stateUpperCam: CameraState
    def __init__(self, agent: _Optional[str] = ..., order: _Optional[_Union[Order, _Mapping]] = ..., teamball: _Optional[_Union[Teamball, _Mapping]] = ..., whistleTimestamp: _Optional[int] = ..., stateLowerCam: _Optional[_Union[CameraState, str]] = ..., stateUpperCam: _Optional[_Union[CameraState, str]] = ...) -> None: ...

class FirmwareInfo(_message.Message):
    __slots__ = ["teamNumber", "playerIdx", "worldmodel", "debug", "msgBudget", "penalized", "currentGcState"]
    TEAMNUMBER_FIELD_NUMBER: _ClassVar[int]
    PLAYERIDX_FIELD_NUMBER: _ClassVar[int]
    WORLDMODEL_FIELD_NUMBER: _ClassVar[int]
    DEBUG_FIELD_NUMBER: _ClassVar[int]
    MSGBUDGET_FIELD_NUMBER: _ClassVar[int]
    PENALIZED_FIELD_NUMBER: _ClassVar[int]
    CURRENTGCSTATE_FIELD_NUMBER: _ClassVar[int]
    teamNumber: int
    playerIdx: int
    worldmodel: WorldModel
    debug: DebugData
    msgBudget: int
    penalized: bool
    currentGcState: int
    def __init__(self, teamNumber: _Optional[int] = ..., playerIdx: _Optional[int] = ..., worldmodel: _Optional[_Union[WorldModel, _Mapping]] = ..., debug: _Optional[_Union[DebugData, _Mapping]] = ..., msgBudget: _Optional[int] = ..., penalized: bool = ..., currentGcState: _Optional[int] = ...) -> None: ...

class TeamCommMsg(_message.Message):
    __slots__ = ["type", "loc_quality", "player_idx", "is_fallen", "pos", "has_ball", "ball_age_us", "ball_pos", "opponents", "sent_time"]
    class Type(int, metaclass=_enum_type_wrapper.EnumTypeWrapper):
        __slots__ = []
        NONE: _ClassVar[TeamCommMsg.Type]
        STRIKER: _ClassVar[TeamCommMsg.Type]
        GOALIE: _ClassVar[TeamCommMsg.Type]
        NEW_STRIKER: _ClassVar[TeamCommMsg.Type]
        POSITION_UPDATE: _ClassVar[TeamCommMsg.Type]
        OPPONENT_UPDATE: _ClassVar[TeamCommMsg.Type]
        NEW_STRIKER_MOVE: _ClassVar[TeamCommMsg.Type]
    NONE: TeamCommMsg.Type
    STRIKER: TeamCommMsg.Type
    GOALIE: TeamCommMsg.Type
    NEW_STRIKER: TeamCommMsg.Type
    POSITION_UPDATE: TeamCommMsg.Type
    OPPONENT_UPDATE: TeamCommMsg.Type
    NEW_STRIKER_MOVE: TeamCommMsg.Type
    TYPE_FIELD_NUMBER: _ClassVar[int]
    LOC_QUALITY_FIELD_NUMBER: _ClassVar[int]
    PLAYER_IDX_FIELD_NUMBER: _ClassVar[int]
    IS_FALLEN_FIELD_NUMBER: _ClassVar[int]
    POS_FIELD_NUMBER: _ClassVar[int]
    HAS_BALL_FIELD_NUMBER: _ClassVar[int]
    BALL_AGE_US_FIELD_NUMBER: _ClassVar[int]
    BALL_POS_FIELD_NUMBER: _ClassVar[int]
    OPPONENTS_FIELD_NUMBER: _ClassVar[int]
    SENT_TIME_FIELD_NUMBER: _ClassVar[int]
    type: TeamCommMsg.Type
    loc_quality: float
    player_idx: int
    is_fallen: bool
    pos: Position
    has_ball: bool
    ball_age_us: int
    ball_pos: Point2D
    opponents: _containers.RepeatedCompositeFieldContainer[Obstacle]
    sent_time: int
    def __init__(self, type: _Optional[_Union[TeamCommMsg.Type, str]] = ..., loc_quality: _Optional[float] = ..., player_idx: _Optional[int] = ..., is_fallen: bool = ..., pos: _Optional[_Union[Position, _Mapping]] = ..., has_ball: bool = ..., ball_age_us: _Optional[int] = ..., ball_pos: _Optional[_Union[Point2D, _Mapping]] = ..., opponents: _Optional[_Iterable[_Union[Obstacle, _Mapping]]] = ..., sent_time: _Optional[int] = ...) -> None: ...

class TeamStrategyDebug(_message.Message):
    __slots__ = ["playerToRole", "roleToLocation"]
    class Role(int, metaclass=_enum_type_wrapper.EnumTypeWrapper):
        __slots__ = []
        GOALIE: _ClassVar[TeamStrategyDebug.Role]
        COVER: _ClassVar[TeamStrategyDebug.Role]
        SHADOW: _ClassVar[TeamStrategyDebug.Role]
        MARK: _ClassVar[TeamStrategyDebug.Role]
        COVER2: _ClassVar[TeamStrategyDebug.Role]
        SHADOW2: _ClassVar[TeamStrategyDebug.Role]
        STRIKER: _ClassVar[TeamStrategyDebug.Role]
        INVALID: _ClassVar[TeamStrategyDebug.Role]
    GOALIE: TeamStrategyDebug.Role
    COVER: TeamStrategyDebug.Role
    SHADOW: TeamStrategyDebug.Role
    MARK: TeamStrategyDebug.Role
    COVER2: TeamStrategyDebug.Role
    SHADOW2: TeamStrategyDebug.Role
    STRIKER: TeamStrategyDebug.Role
    INVALID: TeamStrategyDebug.Role
    class PlayerToRoleEntry(_message.Message):
        __slots__ = ["key", "value"]
        KEY_FIELD_NUMBER: _ClassVar[int]
        VALUE_FIELD_NUMBER: _ClassVar[int]
        key: int
        value: TeamStrategyDebug.Role
        def __init__(self, key: _Optional[int] = ..., value: _Optional[_Union[TeamStrategyDebug.Role, str]] = ...) -> None: ...
    class RoleToLocationEntry(_message.Message):
        __slots__ = ["key", "value"]
        KEY_FIELD_NUMBER: _ClassVar[int]
        VALUE_FIELD_NUMBER: _ClassVar[int]
        key: int
        value: Point2D
        def __init__(self, key: _Optional[int] = ..., value: _Optional[_Union[Point2D, _Mapping]] = ...) -> None: ...
    PLAYERTOROLE_FIELD_NUMBER: _ClassVar[int]
    ROLETOLOCATION_FIELD_NUMBER: _ClassVar[int]
    playerToRole: _containers.ScalarMap[int, TeamStrategyDebug.Role]
    roleToLocation: _containers.MessageMap[int, Point2D]
    def __init__(self, playerToRole: _Optional[_Mapping[int, TeamStrategyDebug.Role]] = ..., roleToLocation: _Optional[_Mapping[int, Point2D]] = ...) -> None: ...

class Infocast(_message.Message):
    __slots__ = ["meta", "world", "debug"]
    META_FIELD_NUMBER: _ClassVar[int]
    WORLD_FIELD_NUMBER: _ClassVar[int]
    DEBUG_FIELD_NUMBER: _ClassVar[int]
    meta: RobotMetaInfo
    world: WorldModel
    debug: DebugData
    def __init__(self, meta: _Optional[_Union[RobotMetaInfo, _Mapping]] = ..., world: _Optional[_Union[WorldModel, _Mapping]] = ..., debug: _Optional[_Union[DebugData, _Mapping]] = ...) -> None: ...
