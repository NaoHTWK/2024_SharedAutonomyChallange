#pragma once

#include <memory>
#include <string>

#include <motion_command.h>
#include <order.h>
#include <worldmodel.h>

class AgentBase {
public:
    AgentBase(std::string name) : name(std::move(name)) {}
    AgentBase(const AgentBase&) = delete;
    AgentBase(AgentBase&&) = delete;
    AgentBase& operator=(const AgentBase&) = delete;
    AgentBase& operator=(AgentBase&&) = delete;
    virtual ~AgentBase() = default;

    virtual MotionCommand proceed(std::shared_ptr<Order> order, WorldModel* world_model) = 0;

    template <class... O>
    static bool isOrder(const std::shared_ptr<Order>& order) {
        return ((dynamic_cast<O*>(order.get()) != nullptr) || ...);
    }

    const std::string name;
};
