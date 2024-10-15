#pragma once

#include "order.h"
#include "point_2d.h"

enum class STRENGTH {LOW, NORMAL};

class ShootOrder : public Order {
private:
    htwk::point_2d target;
    STRENGTH strength;
public:
    ShootOrder(const ShootOrder&) = delete;
    ShootOrder(ShootOrder&&) = delete;
    ShootOrder& operator=(const ShootOrder&) = delete;
    ShootOrder& operator=(ShootOrder&&) = delete;

    explicit ShootOrder(htwk::point_2d target, STRENGTH strength = STRENGTH::NORMAL) : Order("ShootOrder"), target {target}, strength{strength}  {}
    ~ShootOrder() override = default;

    htwk::point_2d getTarget() const{
        return target;
    }

    STRENGTH getStrength() const{
        return strength;
    }

    // todo: protobuf?
};

