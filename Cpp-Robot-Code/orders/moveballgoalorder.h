#pragma once

#include <order.h>

class MoveBallGoalOrder : public Order {
public:
    MoveBallGoalOrder() : Order("MoveBallGoalOrder") {}
    MoveBallGoalOrder(const MoveBallGoalOrder&) = delete;
    MoveBallGoalOrder(MoveBallGoalOrder&&) = delete;
    MoveBallGoalOrder& operator=(MoveBallGoalOrder&) = delete;
    MoveBallGoalOrder& operator=(MoveBallGoalOrder&&) = delete;

    ~MoveBallGoalOrder() override = default;
};
