#pragma once
#include <vector>
#include <memory>
#include "Vehicle.h"
#include "Order.h"
#include "Collection.h"

class DeliveryService
{
private:
    std::vector<std::unique_ptr<Vehicle>> vehicles;
    Collection<Order> orders;

    int find_vehicle_index_by_id(int id) const;
    int find_order_index_by_id(int id) const;
    int find_optimal_vehicle_index(double order_weight) const;

public:
    void print_all_vehicles() const;
    void print_all_orders() const;

    void perform_all_specific_actions();
    void print_delivery_costs(double order_weight) const;

    void add_vehicle(std::unique_ptr<Vehicle> vehicle);
    void add_order(const Order& order);
    void assign_order_to_vehicle(int order_id);
    void restore_assignment(int vehicle_id, int order_id);
    void remove_order_by_id(int order_id);
    void complete_delivery(int vehicle_id);

    void change_vehicle_id(int old_id, int new_id);
    void change_order_id(int old_id, int new_id);

    Vehicle* get_vehicle(int id);
    Order* get_order(int id);
    bool check_vehicle_exists(int id) const;
    bool check_order_exists(int id) const;

    size_t get_vehicles_count() const;
    const Vehicle& get_vehicle_by_index(size_t index) const;
    size_t get_orders_count() const;
    const Order& get_order_by_index(size_t index) const;

    const std::vector<std::unique_ptr<Vehicle>>& get_vehicles() const;
    const Collection<Order>& get_orders() const;

    DeliveryService& operator+=(const Order& order);
    DeliveryService& operator+=(std::unique_ptr<Vehicle> vehicle);
    DeliveryService& operator-=(int order_id);
    DeliveryService& operator-=(const Order& order);
};