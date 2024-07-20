from google.protobuf.internal import containers as _containers
from google.protobuf.internal import enum_type_wrapper as _enum_type_wrapper
from google.protobuf import descriptor as _descriptor
from google.protobuf import message as _message
from typing import ClassVar as _ClassVar, Iterable as _Iterable, Mapping as _Mapping, Optional as _Optional, Union as _Union

DESCRIPTOR: _descriptor.FileDescriptor

class LineSegment(_message.Message):
    __slots__ = ["x", "y", "vx", "vy", "id", "linkX", "linkY"]
    X_FIELD_NUMBER: _ClassVar[int]
    Y_FIELD_NUMBER: _ClassVar[int]
    VX_FIELD_NUMBER: _ClassVar[int]
    VY_FIELD_NUMBER: _ClassVar[int]
    ID_FIELD_NUMBER: _ClassVar[int]
    LINKX_FIELD_NUMBER: _ClassVar[int]
    LINKY_FIELD_NUMBER: _ClassVar[int]
    x: int
    y: int
    vx: int
    vy: int
    id: int
    linkX: int
    linkY: int
    def __init__(self, x: _Optional[int] = ..., y: _Optional[int] = ..., vx: _Optional[int] = ..., vy: _Optional[int] = ..., id: _Optional[int] = ..., linkX: _Optional[int] = ..., linkY: _Optional[int] = ...) -> None: ...

class LineEdge(_message.Message):
    __slots__ = ["px1", "px2", "py1", "py2", "nx", "ny", "d", "x", "y", "id", "matchCnt", "straight", "valid"]
    PX1_FIELD_NUMBER: _ClassVar[int]
    PX2_FIELD_NUMBER: _ClassVar[int]
    PY1_FIELD_NUMBER: _ClassVar[int]
    PY2_FIELD_NUMBER: _ClassVar[int]
    NX_FIELD_NUMBER: _ClassVar[int]
    NY_FIELD_NUMBER: _ClassVar[int]
    D_FIELD_NUMBER: _ClassVar[int]
    X_FIELD_NUMBER: _ClassVar[int]
    Y_FIELD_NUMBER: _ClassVar[int]
    ID_FIELD_NUMBER: _ClassVar[int]
    MATCHCNT_FIELD_NUMBER: _ClassVar[int]
    STRAIGHT_FIELD_NUMBER: _ClassVar[int]
    VALID_FIELD_NUMBER: _ClassVar[int]
    px1: float
    px2: float
    py1: float
    py2: float
    nx: float
    ny: float
    d: float
    x: float
    y: float
    id: int
    matchCnt: int
    straight: bool
    valid: bool
    def __init__(self, px1: _Optional[float] = ..., px2: _Optional[float] = ..., py1: _Optional[float] = ..., py2: _Optional[float] = ..., nx: _Optional[float] = ..., ny: _Optional[float] = ..., d: _Optional[float] = ..., x: _Optional[float] = ..., y: _Optional[float] = ..., id: _Optional[int] = ..., matchCnt: _Optional[int] = ..., straight: bool = ..., valid: bool = ...) -> None: ...

class Line(_message.Message):
    __slots__ = ["edges"]
    EDGES_FIELD_NUMBER: _ClassVar[int]
    edges: _containers.RepeatedCompositeFieldContainer[LineEdge]
    def __init__(self, edges: _Optional[_Iterable[_Union[LineEdge, _Mapping]]] = ...) -> None: ...

class Ellipse(_message.Message):
    __slots__ = ["a", "b", "c", "d", "e", "f", "a1", "b1", "c1", "d1", "e1", "f1", "ta", "tb", "brennpunkt", "found"]
    A_FIELD_NUMBER: _ClassVar[int]
    B_FIELD_NUMBER: _ClassVar[int]
    C_FIELD_NUMBER: _ClassVar[int]
    D_FIELD_NUMBER: _ClassVar[int]
    E_FIELD_NUMBER: _ClassVar[int]
    F_FIELD_NUMBER: _ClassVar[int]
    A1_FIELD_NUMBER: _ClassVar[int]
    B1_FIELD_NUMBER: _ClassVar[int]
    C1_FIELD_NUMBER: _ClassVar[int]
    D1_FIELD_NUMBER: _ClassVar[int]
    E1_FIELD_NUMBER: _ClassVar[int]
    F1_FIELD_NUMBER: _ClassVar[int]
    TA_FIELD_NUMBER: _ClassVar[int]
    TB_FIELD_NUMBER: _ClassVar[int]
    BRENNPUNKT_FIELD_NUMBER: _ClassVar[int]
    FOUND_FIELD_NUMBER: _ClassVar[int]
    a: float
    b: float
    c: float
    d: float
    e: float
    f: float
    a1: float
    b1: float
    c1: float
    d1: float
    e1: float
    f1: float
    ta: float
    tb: float
    brennpunkt: float
    found: bool
    def __init__(self, a: _Optional[float] = ..., b: _Optional[float] = ..., c: _Optional[float] = ..., d: _Optional[float] = ..., e: _Optional[float] = ..., f: _Optional[float] = ..., a1: _Optional[float] = ..., b1: _Optional[float] = ..., c1: _Optional[float] = ..., d1: _Optional[float] = ..., e1: _Optional[float] = ..., f1: _Optional[float] = ..., ta: _Optional[float] = ..., tb: _Optional[float] = ..., brennpunkt: _Optional[float] = ..., found: bool = ...) -> None: ...

class Point2D(_message.Message):
    __slots__ = ["x", "y"]
    X_FIELD_NUMBER: _ClassVar[int]
    Y_FIELD_NUMBER: _ClassVar[int]
    x: int
    y: int
    def __init__(self, x: _Optional[int] = ..., y: _Optional[int] = ...) -> None: ...

class Point2Df(_message.Message):
    __slots__ = ["x", "y"]
    X_FIELD_NUMBER: _ClassVar[int]
    Y_FIELD_NUMBER: _ClassVar[int]
    x: float
    y: float
    def __init__(self, x: _Optional[float] = ..., y: _Optional[float] = ...) -> None: ...

class Point3Df(_message.Message):
    __slots__ = ["x", "y", "z"]
    X_FIELD_NUMBER: _ClassVar[int]
    Y_FIELD_NUMBER: _ClassVar[int]
    Z_FIELD_NUMBER: _ClassVar[int]
    x: float
    y: float
    z: float
    def __init__(self, x: _Optional[float] = ..., y: _Optional[float] = ..., z: _Optional[float] = ...) -> None: ...

class Color(_message.Message):
    __slots__ = ["cy", "cb", "cr"]
    CY_FIELD_NUMBER: _ClassVar[int]
    CB_FIELD_NUMBER: _ClassVar[int]
    CR_FIELD_NUMBER: _ClassVar[int]
    cy: int
    cb: int
    cr: int
    def __init__(self, cy: _Optional[int] = ..., cb: _Optional[int] = ..., cr: _Optional[int] = ...) -> None: ...

class LineCross(_message.Message):
    __slots__ = ["px", "py", "vx", "vy"]
    PX_FIELD_NUMBER: _ClassVar[int]
    PY_FIELD_NUMBER: _ClassVar[int]
    VX_FIELD_NUMBER: _ClassVar[int]
    VY_FIELD_NUMBER: _ClassVar[int]
    px: float
    py: float
    vx: float
    vy: float
    def __init__(self, px: _Optional[float] = ..., py: _Optional[float] = ..., vx: _Optional[float] = ..., vy: _Optional[float] = ...) -> None: ...

class YPR(_message.Message):
    __slots__ = ["yaw", "pitch", "roll"]
    YAW_FIELD_NUMBER: _ClassVar[int]
    PITCH_FIELD_NUMBER: _ClassVar[int]
    ROLL_FIELD_NUMBER: _ClassVar[int]
    yaw: float
    pitch: float
    roll: float
    def __init__(self, yaw: _Optional[float] = ..., pitch: _Optional[float] = ..., roll: _Optional[float] = ...) -> None: ...

class YawPitch(_message.Message):
    __slots__ = ["yaw", "pitch"]
    YAW_FIELD_NUMBER: _ClassVar[int]
    PITCH_FIELD_NUMBER: _ClassVar[int]
    yaw: float
    pitch: float
    def __init__(self, yaw: _Optional[float] = ..., pitch: _Optional[float] = ...) -> None: ...

class PitchRoll(_message.Message):
    __slots__ = ["pitch", "roll"]
    PITCH_FIELD_NUMBER: _ClassVar[int]
    ROLL_FIELD_NUMBER: _ClassVar[int]
    pitch: float
    roll: float
    def __init__(self, pitch: _Optional[float] = ..., roll: _Optional[float] = ...) -> None: ...

class ObjectHypotheses(_message.Message):
    __slots__ = ["x", "y", "radius", "prob", "found", "rating"]
    X_FIELD_NUMBER: _ClassVar[int]
    Y_FIELD_NUMBER: _ClassVar[int]
    RADIUS_FIELD_NUMBER: _ClassVar[int]
    PROB_FIELD_NUMBER: _ClassVar[int]
    FOUND_FIELD_NUMBER: _ClassVar[int]
    RATING_FIELD_NUMBER: _ClassVar[int]
    x: int
    y: int
    radius: int
    prob: float
    found: bool
    rating: float
    def __init__(self, x: _Optional[int] = ..., y: _Optional[int] = ..., radius: _Optional[int] = ..., prob: _Optional[float] = ..., found: bool = ..., rating: _Optional[float] = ...) -> None: ...

class GoalPost(_message.Message):
    __slots__ = ["x", "y", "radius", "prob"]
    X_FIELD_NUMBER: _ClassVar[int]
    Y_FIELD_NUMBER: _ClassVar[int]
    RADIUS_FIELD_NUMBER: _ClassVar[int]
    PROB_FIELD_NUMBER: _ClassVar[int]
    x: int
    y: int
    radius: int
    prob: float
    def __init__(self, x: _Optional[int] = ..., y: _Optional[int] = ..., radius: _Optional[int] = ..., prob: _Optional[float] = ...) -> None: ...

class BoundingBox(_message.Message):
    __slots__ = ["a", "b", "prob"]
    A_FIELD_NUMBER: _ClassVar[int]
    B_FIELD_NUMBER: _ClassVar[int]
    PROB_FIELD_NUMBER: _ClassVar[int]
    a: Point2D
    b: Point2D
    prob: float
    def __init__(self, a: _Optional[_Union[Point2D, _Mapping]] = ..., b: _Optional[_Union[Point2D, _Mapping]] = ..., prob: _Optional[float] = ...) -> None: ...

class RobotBoundingBox(_message.Message):
    __slots__ = ["bb", "dist", "angle", "own_team_prob"]
    BB_FIELD_NUMBER: _ClassVar[int]
    DIST_FIELD_NUMBER: _ClassVar[int]
    ANGLE_FIELD_NUMBER: _ClassVar[int]
    OWN_TEAM_PROB_FIELD_NUMBER: _ClassVar[int]
    bb: BoundingBox
    dist: float
    angle: float
    own_team_prob: float
    def __init__(self, bb: _Optional[_Union[BoundingBox, _Mapping]] = ..., dist: _Optional[float] = ..., angle: _Optional[float] = ..., own_team_prob: _Optional[float] = ...) -> None: ...

class CenterCirclePoint(_message.Message):
    __slots__ = ["x", "y", "radius", "prob", "position"]
    class CenterCirclePointSide(int, metaclass=_enum_type_wrapper.EnumTypeWrapper):
        __slots__ = []
        LEFT: _ClassVar[CenterCirclePoint.CenterCirclePointSide]
        CENTER: _ClassVar[CenterCirclePoint.CenterCirclePointSide]
        RIGHT: _ClassVar[CenterCirclePoint.CenterCirclePointSide]
        UNKNOWN: _ClassVar[CenterCirclePoint.CenterCirclePointSide]
    LEFT: CenterCirclePoint.CenterCirclePointSide
    CENTER: CenterCirclePoint.CenterCirclePointSide
    RIGHT: CenterCirclePoint.CenterCirclePointSide
    UNKNOWN: CenterCirclePoint.CenterCirclePointSide
    X_FIELD_NUMBER: _ClassVar[int]
    Y_FIELD_NUMBER: _ClassVar[int]
    RADIUS_FIELD_NUMBER: _ClassVar[int]
    PROB_FIELD_NUMBER: _ClassVar[int]
    POSITION_FIELD_NUMBER: _ClassVar[int]
    x: int
    y: int
    radius: int
    prob: float
    position: CenterCirclePoint.CenterCirclePointSide
    def __init__(self, x: _Optional[int] = ..., y: _Optional[int] = ..., radius: _Optional[int] = ..., prob: _Optional[float] = ..., position: _Optional[_Union[CenterCirclePoint.CenterCirclePointSide, str]] = ...) -> None: ...

class LowerCamObstacleResult(_message.Message):
    __slots__ = ["width", "height", "obstacle"]
    WIDTH_FIELD_NUMBER: _ClassVar[int]
    HEIGHT_FIELD_NUMBER: _ClassVar[int]
    OBSTACLE_FIELD_NUMBER: _ClassVar[int]
    width: int
    height: int
    obstacle: _containers.RepeatedScalarFieldContainer[float]
    def __init__(self, width: _Optional[int] = ..., height: _Optional[int] = ..., obstacle: _Optional[_Iterable[float]] = ...) -> None: ...

class CamPose(_message.Message):
    __slots__ = ["leg_height", "body_angles", "head_angles", "cam_id", "body_offset", "head_offset", "ellipse_angles"]
    class CamID(int, metaclass=_enum_type_wrapper.EnumTypeWrapper):
        __slots__ = []
        UPPER: _ClassVar[CamPose.CamID]
        LOWER: _ClassVar[CamPose.CamID]
    UPPER: CamPose.CamID
    LOWER: CamPose.CamID
    LEG_HEIGHT_FIELD_NUMBER: _ClassVar[int]
    BODY_ANGLES_FIELD_NUMBER: _ClassVar[int]
    HEAD_ANGLES_FIELD_NUMBER: _ClassVar[int]
    CAM_ID_FIELD_NUMBER: _ClassVar[int]
    BODY_OFFSET_FIELD_NUMBER: _ClassVar[int]
    HEAD_OFFSET_FIELD_NUMBER: _ClassVar[int]
    ELLIPSE_ANGLES_FIELD_NUMBER: _ClassVar[int]
    leg_height: float
    body_angles: YPR
    head_angles: YawPitch
    cam_id: CamPose.CamID
    body_offset: PitchRoll
    head_offset: PitchRoll
    ellipse_angles: PitchRoll
    def __init__(self, leg_height: _Optional[float] = ..., body_angles: _Optional[_Union[YPR, _Mapping]] = ..., head_angles: _Optional[_Union[YawPitch, _Mapping]] = ..., cam_id: _Optional[_Union[CamPose.CamID, str]] = ..., body_offset: _Optional[_Union[PitchRoll, _Mapping]] = ..., head_offset: _Optional[_Union[PitchRoll, _Mapping]] = ..., ellipse_angles: _Optional[_Union[PitchRoll, _Mapping]] = ...) -> None: ...

class TinyImage(_message.Message):
    __slots__ = ["y", "u", "v", "width", "height"]
    Y_FIELD_NUMBER: _ClassVar[int]
    U_FIELD_NUMBER: _ClassVar[int]
    V_FIELD_NUMBER: _ClassVar[int]
    WIDTH_FIELD_NUMBER: _ClassVar[int]
    HEIGHT_FIELD_NUMBER: _ClassVar[int]
    y: bytes
    u: bytes
    v: bytes
    width: int
    height: int
    def __init__(self, y: _Optional[bytes] = ..., u: _Optional[bytes] = ..., v: _Optional[bytes] = ..., width: _Optional[int] = ..., height: _Optional[int] = ...) -> None: ...

class VisionFrame(_message.Message):
    __slots__ = ["frameId", "width", "height", "fieldColor", "fieldBorderY", "lineSegments", "lines", "ellipse", "ball", "lineCrosses", "feet", "time", "penaltySpot", "feets", "obstacle", "camPose", "tinyImage", "objHypotheses", "goalpost", "centercirclepoint", "robots"]
    FRAMEID_FIELD_NUMBER: _ClassVar[int]
    WIDTH_FIELD_NUMBER: _ClassVar[int]
    HEIGHT_FIELD_NUMBER: _ClassVar[int]
    FIELDCOLOR_FIELD_NUMBER: _ClassVar[int]
    FIELDBORDERY_FIELD_NUMBER: _ClassVar[int]
    LINESEGMENTS_FIELD_NUMBER: _ClassVar[int]
    LINES_FIELD_NUMBER: _ClassVar[int]
    ELLIPSE_FIELD_NUMBER: _ClassVar[int]
    BALL_FIELD_NUMBER: _ClassVar[int]
    LINECROSSES_FIELD_NUMBER: _ClassVar[int]
    FEET_FIELD_NUMBER: _ClassVar[int]
    TIME_FIELD_NUMBER: _ClassVar[int]
    PENALTYSPOT_FIELD_NUMBER: _ClassVar[int]
    FEETS_FIELD_NUMBER: _ClassVar[int]
    OBSTACLE_FIELD_NUMBER: _ClassVar[int]
    CAMPOSE_FIELD_NUMBER: _ClassVar[int]
    TINYIMAGE_FIELD_NUMBER: _ClassVar[int]
    OBJHYPOTHESES_FIELD_NUMBER: _ClassVar[int]
    GOALPOST_FIELD_NUMBER: _ClassVar[int]
    CENTERCIRCLEPOINT_FIELD_NUMBER: _ClassVar[int]
    ROBOTS_FIELD_NUMBER: _ClassVar[int]
    frameId: int
    width: int
    height: int
    fieldColor: Color
    fieldBorderY: _containers.RepeatedScalarFieldContainer[int]
    lineSegments: _containers.RepeatedCompositeFieldContainer[LineSegment]
    lines: _containers.RepeatedCompositeFieldContainer[Line]
    ellipse: Ellipse
    ball: ObjectHypotheses
    lineCrosses: _containers.RepeatedCompositeFieldContainer[LineCross]
    feet: Point2D
    time: int
    penaltySpot: ObjectHypotheses
    feets: _containers.RepeatedCompositeFieldContainer[ObjectHypotheses]
    obstacle: LowerCamObstacleResult
    camPose: CamPose
    tinyImage: TinyImage
    objHypotheses: _containers.RepeatedCompositeFieldContainer[ObjectHypotheses]
    goalpost: _containers.RepeatedCompositeFieldContainer[GoalPost]
    centercirclepoint: _containers.RepeatedCompositeFieldContainer[CenterCirclePoint]
    robots: _containers.RepeatedCompositeFieldContainer[RobotBoundingBox]
    def __init__(self, frameId: _Optional[int] = ..., width: _Optional[int] = ..., height: _Optional[int] = ..., fieldColor: _Optional[_Union[Color, _Mapping]] = ..., fieldBorderY: _Optional[_Iterable[int]] = ..., lineSegments: _Optional[_Iterable[_Union[LineSegment, _Mapping]]] = ..., lines: _Optional[_Iterable[_Union[Line, _Mapping]]] = ..., ellipse: _Optional[_Union[Ellipse, _Mapping]] = ..., ball: _Optional[_Union[ObjectHypotheses, _Mapping]] = ..., lineCrosses: _Optional[_Iterable[_Union[LineCross, _Mapping]]] = ..., feet: _Optional[_Union[Point2D, _Mapping]] = ..., time: _Optional[int] = ..., penaltySpot: _Optional[_Union[ObjectHypotheses, _Mapping]] = ..., feets: _Optional[_Iterable[_Union[ObjectHypotheses, _Mapping]]] = ..., obstacle: _Optional[_Union[LowerCamObstacleResult, _Mapping]] = ..., camPose: _Optional[_Union[CamPose, _Mapping]] = ..., tinyImage: _Optional[_Union[TinyImage, _Mapping]] = ..., objHypotheses: _Optional[_Iterable[_Union[ObjectHypotheses, _Mapping]]] = ..., goalpost: _Optional[_Iterable[_Union[GoalPost, _Mapping]]] = ..., centercirclepoint: _Optional[_Iterable[_Union[CenterCirclePoint, _Mapping]]] = ..., robots: _Optional[_Iterable[_Union[RobotBoundingBox, _Mapping]]] = ...) -> None: ...

class WalkRequest(_message.Message):
    __slots__ = ["dx", "dy", "da", "shoot"]
    class Shoot(int, metaclass=_enum_type_wrapper.EnumTypeWrapper):
        __slots__ = []
        NONE: _ClassVar[WalkRequest.Shoot]
        LEFT: _ClassVar[WalkRequest.Shoot]
        RIGHT: _ClassVar[WalkRequest.Shoot]
    NONE: WalkRequest.Shoot
    LEFT: WalkRequest.Shoot
    RIGHT: WalkRequest.Shoot
    DX_FIELD_NUMBER: _ClassVar[int]
    DY_FIELD_NUMBER: _ClassVar[int]
    DA_FIELD_NUMBER: _ClassVar[int]
    SHOOT_FIELD_NUMBER: _ClassVar[int]
    dx: float
    dy: float
    da: float
    shoot: WalkRequest.Shoot
    def __init__(self, dx: _Optional[float] = ..., dy: _Optional[float] = ..., da: _Optional[float] = ..., shoot: _Optional[_Union[WalkRequest.Shoot, str]] = ...) -> None: ...

class Foot(_message.Message):
    __slots__ = ["p", "alpha", "beta"]
    P_FIELD_NUMBER: _ClassVar[int]
    ALPHA_FIELD_NUMBER: _ClassVar[int]
    BETA_FIELD_NUMBER: _ClassVar[int]
    p: Point3Df
    alpha: float
    beta: float
    def __init__(self, p: _Optional[_Union[Point3Df, _Mapping]] = ..., alpha: _Optional[float] = ..., beta: _Optional[float] = ...) -> None: ...

class Feet(_message.Message):
    __slots__ = ["left", "right", "gamma"]
    LEFT_FIELD_NUMBER: _ClassVar[int]
    RIGHT_FIELD_NUMBER: _ClassVar[int]
    GAMMA_FIELD_NUMBER: _ClassVar[int]
    left: Foot
    right: Foot
    gamma: float
    def __init__(self, left: _Optional[_Union[Foot, _Mapping]] = ..., right: _Optional[_Union[Foot, _Mapping]] = ..., gamma: _Optional[float] = ...) -> None: ...

class SensorFrame(_message.Message):
    __slots__ = ["time", "head_pitch", "head_yaw", "body_pitch", "body_roll", "motion_vec", "gyro", "accel", "leg_height", "step_height", "walk_request", "feet"]
    TIME_FIELD_NUMBER: _ClassVar[int]
    HEAD_PITCH_FIELD_NUMBER: _ClassVar[int]
    HEAD_YAW_FIELD_NUMBER: _ClassVar[int]
    BODY_PITCH_FIELD_NUMBER: _ClassVar[int]
    BODY_ROLL_FIELD_NUMBER: _ClassVar[int]
    MOTION_VEC_FIELD_NUMBER: _ClassVar[int]
    GYRO_FIELD_NUMBER: _ClassVar[int]
    ACCEL_FIELD_NUMBER: _ClassVar[int]
    LEG_HEIGHT_FIELD_NUMBER: _ClassVar[int]
    STEP_HEIGHT_FIELD_NUMBER: _ClassVar[int]
    WALK_REQUEST_FIELD_NUMBER: _ClassVar[int]
    FEET_FIELD_NUMBER: _ClassVar[int]
    time: int
    head_pitch: float
    head_yaw: float
    body_pitch: float
    body_roll: float
    motion_vec: Point3Df
    gyro: YPR
    accel: Point3Df
    leg_height: float
    step_height: float
    walk_request: WalkRequest
    feet: Feet
    def __init__(self, time: _Optional[int] = ..., head_pitch: _Optional[float] = ..., head_yaw: _Optional[float] = ..., body_pitch: _Optional[float] = ..., body_roll: _Optional[float] = ..., motion_vec: _Optional[_Union[Point3Df, _Mapping]] = ..., gyro: _Optional[_Union[YPR, _Mapping]] = ..., accel: _Optional[_Union[Point3Df, _Mapping]] = ..., leg_height: _Optional[float] = ..., step_height: _Optional[float] = ..., walk_request: _Optional[_Union[WalkRequest, _Mapping]] = ..., feet: _Optional[_Union[Feet, _Mapping]] = ...) -> None: ...

class RawImage(_message.Message):
    __slots__ = ["vision", "image", "time_us", "cam_id", "reason"]
    VISION_FIELD_NUMBER: _ClassVar[int]
    IMAGE_FIELD_NUMBER: _ClassVar[int]
    TIME_US_FIELD_NUMBER: _ClassVar[int]
    CAM_ID_FIELD_NUMBER: _ClassVar[int]
    REASON_FIELD_NUMBER: _ClassVar[int]
    vision: VisionFrame
    image: bytes
    time_us: int
    cam_id: int
    reason: str
    def __init__(self, vision: _Optional[_Union[VisionFrame, _Mapping]] = ..., image: _Optional[bytes] = ..., time_us: _Optional[int] = ..., cam_id: _Optional[int] = ..., reason: _Optional[str] = ...) -> None: ...
