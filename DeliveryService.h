#pragma once
#include <iostream>
#include <vector>
#include "Vehicle.h"
#include "Order.h"

class DeliveryService
{
private:
    std::vector<Vehicle> vehicles;
    std::vector<Order> orders;

public:
    void AddVehicle(const Vehicle& vehicle);
    void AddOrder(const Order& order);
    bool FindVehicleForOrder(const Order& order);
};