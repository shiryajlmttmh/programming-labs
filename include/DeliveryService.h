#pragma once
#include <vector>
#include "Vehicle.h"
#include "Order.h"

class DeliveryService
{
private:
    std::vector<Vehicle*> vehicles;
    std::vector<Order> orders;

    int find_vehicle_index_by_id(int id) const;
    int find_order_index_by_id(int id) const;
    int find_optimal_vehicle_index(double order_weight) const;

public:
    DeliveryService() = default;

    void print_all_vehicles() const;
    void print_all_orders() const;

    bool add_vehicle(Vehicle* vehicle);
    bool add_order(const Order& order);
    bool assign_order_to_vehicle(int order_id);
    bool remove_order_by_id(int order_id);
    bool complete_delivery(int vehicle_id);

    Vehicle* get_vehicle(int id);
    Order* get_order(int id);
    bool check_vehicle_exists(int id) const;
    bool check_order_exists(int id) const;

    DeliveryService& operator+=(const Order& order);
    DeliveryService& operator+=(Vehicle* vehicle);
    DeliveryService& operator-=(int order_id);
    DeliveryService& operator-=(const Order& order);
};