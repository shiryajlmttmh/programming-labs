#pragma once
#include <vector>
#include <set>
#include <map>
#include <string>
#include <optional>
#include <limits>
#include <utility>
#include "DeliveryService.h"
#include "Vehicle.h"
#include "Order.h"


struct OrderFilter
{
    std::string district;
    double min_weight = 0.0;
    double max_weight = std::numeric_limits<double>::max();
    std::optional<bool> is_assigned;
};

struct VehicleFilter
{
    std::string type;
    double min_capacity = 0.0;
    std::optional<bool> is_available;
};

enum class OrderSortKey { Id, Weight, District };
enum class VehicleSortKey { Id, Capacity, CourierName };

class DeliveryAnalytics
{
public:
    using OrderGroups = std::map<std::string, std::vector<const Order*>>;
    using VehicleGroups = std::map<std::string, std::vector<const Vehicle*>>;

private:
    const DeliveryService& service;

    std::vector<const Order*> collect_orders() const;
    std::vector<const Vehicle*> collect_vehicles() const;

public:
    explicit DeliveryAnalytics(const DeliveryService& service);

    std::vector<const Order*> find_orders(const OrderFilter& filter) const;
    std::vector<const Vehicle*> find_vehicles(const VehicleFilter& filter) const;

    std::vector<const Order*> sort_orders(OrderSortKey key, bool ascending) const;
    std::vector<const Vehicle*> sort_vehicles(VehicleSortKey key, bool ascending) const;

    std::pair<const Order*, const Order*> find_lightest_and_heaviest_order() const;
    std::pair<const Vehicle*, const Vehicle*> find_smallest_and_largest_vehicle() const;

    size_t count_waiting_orders() const;
    size_t count_orders_heavier_than(double weight) const;
    size_t count_available_vehicles() const;

    std::set<std::string> get_unique_districts() const;

    OrderGroups group_orders_by_district() const;
    VehicleGroups group_vehicles_by_type() const;

    double get_total_order_weight() const;
    double get_waiting_order_weight() const;
    double get_average_order_weight() const;
    double get_total_vehicle_capacity() const;
    double get_available_vehicle_capacity() const;
    std::map<std::string, double> get_weight_by_district() const;

    double get_waiting_load_ratio() const;
};