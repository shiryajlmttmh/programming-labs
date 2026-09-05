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

    int FindVehicleIndexById(int id) const;
    int FindOrderIndexById(int id) const;
    int FindOptimalVehicleIndex(double orderWeight) const;

public:
    void PrintAllVehicles() const;
    void PrintAllOrders() const;
    void AddVehicle(const Vehicle& vehicle);
    void AddOrder(const Order& order);
    bool AssignOrderToVehicle(int orderId);
    bool RemoveOrderById(int orderId);
    bool CompleteDelivery(int vehicleId, int orderId);
};