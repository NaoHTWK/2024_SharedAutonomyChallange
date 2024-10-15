#ifndef WALKTOPOSITIONORDER_H_
#define WALKTOPOSITIONORDER_H_

#include <head_focus.h>
#include <order.h>
#include <position.h>
#include <worldmodel.pb.h>

class WalkToPositionOrder : public Order {
public:
    // FOCUS_DIRECTION: Starts to rotate to the target angle even if position isn't reached yet and might walk sideways
    //                  or backwards for longer periods.
    enum class Mode { USE_A, STRIKER, SUPPORTER, FOCUS_DIRECTION };

    WalkToPositionOrder(const htwk::point_2d& pos, Mode mode, bool precise = false, std::optional<HeadFocus> head_focus = std::nullopt,
                        std::vector<htwk::point_2d> obstacles = {})
        : Order("WalkToPositionOrder"),
          pos(pos, 0),
          mode(mode),
          precise(precise),
          head_focus(head_focus),
          obstacles(std::move(obstacles)) {}
    WalkToPositionOrder(Position pos, Mode mode = Mode::USE_A, bool precise = false, std::optional<HeadFocus> head_focus = std::nullopt,
                        std::vector<htwk::point_2d> obstacles = {})
        : Order("WalkToPositionOrder"), pos(pos), mode(mode), precise(precise), head_focus(head_focus), obstacles(std::move(obstacles)) {}

    const Position pos;
    const Mode mode;
    const bool precise;
    const std::optional<HeadFocus> head_focus;
    const std::vector<htwk::point_2d> obstacles;  // absolute coordinates

    void toProtobuf(protobuf::worldmodel::Order* order) override {
        order->set_name(getClassName());

        auto* wpos = order->mutable_walktopositionorder()->mutable_pos();

        wpos->set_x(pos.x);
        wpos->set_y(pos.y);
        wpos->set_a(pos.a);
        for (const auto& o : obstacles) {
            auto* obs = order->mutable_walktopositionorder()->add_obstacles();
            obs->set_x(o.x);
            obs->set_y(o.y);
        }
        order->mutable_walktopositionorder()->set_precise(precise);
        if (head_focus)
            order->mutable_walktopositionorder()->set_head_focus(
                    protobuf::worldmodel::WalkToPositionOrder_HeadFocus(as_integer(*head_focus)));

        switch (mode) {
            case Mode::USE_A:
                order->mutable_walktopositionorder()->set_usea(true);
                order->mutable_walktopositionorder()->set_mode(protobuf::worldmodel::WalkToPositionOrder::MODE_USE_A);
                break;
            case Mode::STRIKER:
                order->mutable_walktopositionorder()->set_usea(false);
                order->mutable_walktopositionorder()->set_mode(protobuf::worldmodel::WalkToPositionOrder::MODE_STRIKER);
                break;
            case Mode::SUPPORTER:
                order->mutable_walktopositionorder()->set_usea(false);
                order->mutable_walktopositionorder()->set_mode(
                        protobuf::worldmodel::WalkToPositionOrder::MODE_SUPPORTER);
                break;
            case Mode::FOCUS_DIRECTION:
                order->mutable_walktopositionorder()->set_usea(true);
                order->mutable_walktopositionorder()->set_mode(
                        protobuf::worldmodel::WalkToPositionOrder::MODE_FOCUS_DIRECTION);
                break;
        }
    }
};

#endif
