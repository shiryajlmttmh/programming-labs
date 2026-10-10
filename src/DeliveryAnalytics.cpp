#include "DeliveryAnalytics.h"
#include "Exceptions.h"
#include <algorithm>
#include <format>

using namespace std;

DeliveryAnalytics::DeliveryAnalytics(const DeliveryService& service) : service(service) {}

vector<const Order*> DeliveryAnalytics::find_orders(const OrderFilter& filter) const
{
    if (filter.min_weight > filter.max_weight)
        throw InvalidDataException("Минимальный вес (" + format("{:.1f}", filter.min_weight)
            + " кг) не может быть больше максимального (" + format("{:.1f}", filter.max_weight) + " кг)");

    const Collection<Order>& orders = service.get_orders();
    vector<const Order*> result;

    for_each(orders.begin(), orders.end(),
        [&filter, &result](const Order& order)
        {
            bool matches = (filter.district.empty() || order.get_district() == filter.district)
                && order.get_weight() >= filter.min_weight
                && order.get_weight() <= filter.max_weight
                && (!filter.is_assigned.has_value() || order.get_is_assigned() == *filter.is_assigned);

            if (matches) result.push_back(&order);
        });

    return result;
}

vector<const Vehicle*> DeliveryAnalytics::find_vehicles(const VehicleFilter& filter) const
{
    if (filter.min_capacity < 0)
        throw InvalidDataException("Минимальная грузоподъемность не может быть отрицательной, получено: "
            + format("{:.1f}", filter.min_capacity));

    const vector<unique_ptr<Vehicle>>& vehicles = service.get_vehicles();
    vector<const Vehicle*> result;

    for_each(vehicles.begin(), vehicles.end(),
        [&filter, &result](const unique_ptr<Vehicle>& vehicle)
        {
            bool matches = (filter.type.empty() || vehicle->get_type() == filter.type)
                && vehicle->get_capacity() >= filter.min_capacity
                && (!filter.is_available.has_value() || vehicle->get_is_available() == *filter.is_available);

            if (matches) result.push_back(vehicle.get());
        });

    return result;
}

vector<const Order*> DeliveryAnalytics::sort_orders(OrderSortKey key, bool ascending) const
{
    vector<const Order*> result = find_orders(OrderFilter{});

    auto is_before = [key](const Order* first, const Order* second)
        {
            switch (key)
            {
            case OrderSortKey::Weight:
                if (first->get_weight() != second->get_weight()) return first->get_weight() < second->get_weight();
                break;
            case OrderSortKey::District:
                if (first->get_district() != second->get_district()) return first->get_district() < second->get_district();
                break;
            case OrderSortKey::Id:
                break;
            }
            return first->get_id() < second->get_id();
        };

    sort(result.begin(), result.end(),
        [&is_before, ascending](const Order* first, const Order* second)
        {
            return ascending ? is_before(first, second) : is_before(second, first);
        });

    return result;
}

vector<const Vehicle*> DeliveryAnalytics::sort_vehicles(VehicleSortKey key, bool ascending) const
{
    vector<const Vehicle*> result = find_vehicles(VehicleFilter{});

    auto is_before = [key](const Vehicle* first, const Vehicle* second)
        {
            switch (key)
            {
            case VehicleSortKey::Capacity:
                if (first->get_capacity() != second->get_capacity()) return first->get_capacity() < second->get_capacity();
                break;
            case VehicleSortKey::CourierName:
                if (first->get_courier_name() != second->get_courier_name()) return first->get_courier_name() < second->get_courier_name();
                break;
            case VehicleSortKey::Id:
                break;
            }
            return first->get_id() < second->get_id();
        };

    sort(result.begin(), result.end(),
        [&is_before, ascending](const Vehicle* first, const Vehicle* second)
        {
            return ascending ? is_before(first, second) : is_before(second, first);
        });

    return result;
}

pair<const Order*, const Order*> DeliveryAnalytics::find_lightest_and_heaviest_order() const
{
    const Collection<Order>& orders = service.get_orders();
    if (orders.get_items_count() == 0)
        throw InvalidOperationException("Нет заказов, невозможно определить минимальный и максимальный вес");

    auto [lightest, heaviest] = minmax_element(orders.begin(), orders.end(),
        [](const Order& first, const Order& second) { return first.get_weight() < second.get_weight(); });

    return { &*lightest, &*heaviest };
}

pair<const Vehicle*, const Vehicle*> DeliveryAnalytics::find_smallest_and_largest_vehicle() const
{
    const vector<unique_ptr<Vehicle>>& vehicles = service.get_vehicles();
    if (vehicles.empty())
        throw InvalidOperationException("Нет транспорта, невозможно определить минимальную и максимальную грузоподъемность");

    auto [smallest, largest] = minmax_element(vehicles.begin(), vehicles.end(),
        [](const unique_ptr<Vehicle>& first, const unique_ptr<Vehicle>& second)
        {
            return first->get_capacity() < second->get_capacity();
        });

    return { smallest->get(), largest->get() };
}

size_t DeliveryAnalytics::count_waiting_orders() const
{
    const Collection<Order>& orders = service.get_orders();

    return static_cast<size_t>(count_if(orders.begin(), orders.end(),
        [](const Order& order) { return !order.get_is_assigned(); }));
}

size_t DeliveryAnalytics::count_orders_heavier_than(double weight) const
{
    const Collection<Order>& orders = service.get_orders();

    return static_cast<size_t>(count_if(orders.begin(), orders.end(),
        [weight](const Order& order) { return order.get_weight() > weight; }));
}

size_t DeliveryAnalytics::count_available_vehicles() const
{
    const vector<unique_ptr<Vehicle>>& vehicles = service.get_vehicles();

    return static_cast<size_t>(count_if(vehicles.begin(), vehicles.end(),
        [](const unique_ptr<Vehicle>& vehicle) { return vehicle->get_is_available(); }));
}

set<string> DeliveryAnalytics::get_unique_districts() const
{
    const Collection<Order>& orders = service.get_orders();
    set<string> districts;

    for_each(orders.begin(), orders.end(),
        [&districts](const Order& order)
        {
            if (!order.get_district().empty()) districts.insert(order.get_district());
        });

    return districts;
}