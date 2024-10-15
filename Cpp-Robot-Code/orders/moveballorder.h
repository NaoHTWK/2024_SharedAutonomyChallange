#ifndef MOVEBALLORDER_H_
#define MOVEBALLORDER_H_

#include <order.h>
#include <point_2d.h>
#include <point_2d_helper.h>
#include <worldmodel.pb.h>

class MoveBallOrder: public Order {
private:
    htwk::point_2d pos;

public:

    MoveBallOrder() = delete;
    MoveBallOrder(const MoveBallOrder&) = delete;
    MoveBallOrder(MoveBallOrder&&) = delete;
    MoveBallOrder& operator=(const MoveBallOrder&) = delete;
    MoveBallOrder& operator=(MoveBallOrder&&) = delete;

    MoveBallOrder(float _x,float _y): Order("MoveBallOrder"), pos(htwk::point_2d(_x,_y)){}
    MoveBallOrder(htwk::point_2d pos): Order("MoveBallOrder"), pos(pos){}

    ~MoveBallOrder() override = default;

    htwk::point_2d getPosition() const{
        return pos;
    }

    void toProtobuf(protobuf::worldmodel::Order* order) override {
        order->set_name(getClassName());
        ::toProtobuf(pos, order->mutable_moveballorder()->mutable_pos());
    }

};

#endif
