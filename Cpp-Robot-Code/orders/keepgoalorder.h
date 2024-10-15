#ifndef KEEPGOALORDER_H_
#define KEEPGOALORDER_H_

#include <order.h>
#include <point_2d.h>
#include <position.h>
#include <soccerfield.h>

class KeepGoalOrder : public Order {
public:
    KeepGoalOrder(const KeepGoalOrder&) = delete;
    KeepGoalOrder(KeepGoalOrder&&) = delete;
    KeepGoalOrder& operator=(KeepGoalOrder&) = delete;
    KeepGoalOrder& operator=(KeepGoalOrder&&) = delete;

    KeepGoalOrder(bool allowedToMove = true,
                  const htwk::point_2d& walkToPos = {-SoccerField::length() / 2.f + .3f, 0.f})
        : Order("KeepGoalOrder"), allowedToMove(allowedToMove), pos(walkToPos, 0) {}
    ~KeepGoalOrder() override = default;

    const bool allowedToMove;
    const Position pos;

    void toProtobuf(protobuf::worldmodel::Order* order) override {
        order->set_name(getClassName());
        order->mutable_keepgoalorder()->set_allowedtomove(allowedToMove);
    }

};

#endif /* KEEPGOALORDER_H_ */
