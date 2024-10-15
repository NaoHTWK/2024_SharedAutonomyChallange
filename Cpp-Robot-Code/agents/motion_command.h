#pragma once

#include <optional>
#include <head_focus.h>

#include <joints.h>
#include <keyframe_controller.h>

class MotionCommand {

public:
    enum class Type { NOTHING, WALK, STAND, PLAY_MOTION };

    static MotionCommand Nothing;
    static MotionCommand Walk(WalkRequest walk_request, HeadFocus focus = HeadFocus::NOTHING) {
        return {Type::WALK, walk_request, focus, {}};
    }
    static MotionCommand Stand(HeadFocus focus = HeadFocus::NOTHING) {
        return {Type::STAND, {}, focus, {}};
    }

    static MotionCommand PlayMotion(KeyframeMotionID motion_id) {
        return {Type::PLAY_MOTION, {}, {}, motion_id};
    }

    Type type{Type::NOTHING};
    WalkRequest walk_request{};
    HeadFocus focus{HeadFocus::NOTHING};
    std::optional<KeyframeMotionID> motion_id;

    friend bool operator==(const MotionCommand& lhs, const MotionCommand& rhs) {
        return lhs.type == rhs.type;
    }

    friend bool operator!=(const MotionCommand& lhs, const MotionCommand& rhs) {
        return !(lhs == rhs);
    }
};
