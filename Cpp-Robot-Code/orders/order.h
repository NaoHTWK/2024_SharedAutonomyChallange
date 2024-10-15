#pragma once

#include <memory>
#include <string>

#include <worldmodel.pb.h>

class Order {
private:
    std::string name;

public:
    explicit Order(std::string name) : name(std::move(name)) {}
    Order(const Order&) = delete;
    Order(Order&&) = delete;
    Order& operator=(const Order&) = delete;
    Order& operator=(Order&&) = delete;
    virtual ~Order();

    virtual const std::string& getClassName() const {
        return name;
    }

    virtual void toProtobuf(protobuf::worldmodel::Order* order) {
        order->set_name(getClassName());
    }

    virtual std::string to_string() const final {
        return name;
    }
};
