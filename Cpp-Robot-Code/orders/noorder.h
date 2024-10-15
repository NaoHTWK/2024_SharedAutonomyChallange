#pragma once

#include "order.h"

class NoOrder : public Order {
public:
    NoOrder() : Order("NoOrder") {}
};
