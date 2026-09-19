#include "DeliveryService.h"
#include <iostream>
#include <format>

using namespace std;

void DeliveryService::print_all_vehicles() const
{
    cout << "\n--- Список транспорта ---" << endl;
    if (vehicles.empty()) { cout << "Список пуст." << endl; return; }

    for (size_t i = 0; i < vehicles.size(); i++)
        cout << *vehicles[i] << endl;
}

void DeliveryService::perform_all_specific_actions()
{
    cout << "\n--- Специфические действия всего транспорта ---" << endl;
    if (vehicles.empty()) { cout << "Список пуст." << endl; return; }

    // Динамический полиморфизм: для каждого элемента вызывается
    // перегрузка perform_specific_action() фактического типа объекта
    for (auto& vehicle : vehicles)
        vehicle->perform_specific_action();
}

void DeliveryService::print_delivery_costs(double order_weight) const
{
    cout << "\n--- Стоимость доставки груза весом " << order_weight << " кг ---" << endl;
    if (vehicles.empty()) { cout << "Список пуст." << endl; return; }

    for (const auto& vehicle : vehicles)
    {
        cout << vehicle->get_type() << " номер " << vehicle->get_id() << " (" << vehicle->get_courier_name() << "): ";

        if (order_weight > vehicle->get_capacity())
            cout << "груз превышает грузоподъемность (" << vehicle->get_capacity() << " кг)" << endl;
        else
            cout << std::format("{:.2f}", vehicle->calculate_delivery_cost(order_weight)) << " руб." << endl;
    }
}

void DeliveryService::print_all_orders() const
{
    cout << "\n--- Список активных заказов ---" << endl;
    if (orders.empty()) { cout << "Список пуст." << endl; return; }
    for (size_t i = 0; i < orders.size(); i++) cout << orders[i] << endl;
}

bool DeliveryService::add_vehicle(unique_ptr<Vehicle> vehicle)
{
    if (!vehicle) return false;

    if (find_vehicle_index_by_id(vehicle->get_id()) != -1)
    {
        cout << "Ошибка: транспорт с ID " << vehicle->get_id() << " уже существует!" << endl;
        return false;
    }
    vehicles.push_back(std::move(vehicle));
    return true;
}

bool DeliveryService::add_order(const Order& order)
{
    if (find_order_index_by_id(order.get_id()) != -1)
    {
        cout << "Ошибка: заказ с ID " << order.get_id() << " уже существует!" << endl;
        return false;
    }
    orders.push_back(order);
    return true;
}

bool DeliveryService::remove_order_by_id(int order_id)
{
    int index = find_order_index_by_id(order_id);
    if (index == -1)
    {
        cout << "Ошибка: заказ с номером " << order_id << " не найден!" << endl;
        return false;
    }

    if (orders[index].get_is_assigned())
    {
        cout << "Ошибка: нельзя удалить заказ номер " << order_id << ", так как он находится в процессе доставки!" << endl;
        return false;
    }

    orders.erase(orders.begin() + index);
    cout << "Заказ номер " << order_id << " успешно удален из системы." << endl;
    return true;
}

int DeliveryService::find_optimal_vehicle_index(double order_weight) const
{
    int vehicle_index = -1;
    double min_capacity = 0;

    for (size_t i = 0; i < vehicles.size(); i++)
    {
        double curr_capacity = vehicles[i]->get_capacity();
        if (vehicles[i]->get_is_available() && curr_capacity >= order_weight)
        {
            if (vehicle_index == -1 || curr_capacity < min_capacity)
            {
                min_capacity = curr_capacity;
                vehicle_index = static_cast<int>(i);
            }
        }
    }

    return vehicle_index;
}

int DeliveryService::find_vehicle_index_by_id(int id) const
{
    for (size_t i = 0; i < vehicles.size(); i++)
        if (vehicles[i]->get_id() == id) return static_cast<int>(i);

    return -1;
}

int DeliveryService::find_order_index_by_id(int id) const
{
    for (size_t i = 0; i < orders.size(); i++)
        if (orders[i].get_id() == id) return static_cast<int>(i);

    return -1;
}

bool DeliveryService::assign_order_to_vehicle(int order_id)
{
    int order_index = find_order_index_by_id(order_id);
    if (order_index == -1)
    {
        cout << "Ошибка: заказ с номером " << order_id << " не найден!" << endl;
        return false;
    }

    if (orders[order_index].get_is_assigned())
    {
        cout << "Ошибка: заказ номер " << order_id << " уже назначен на транспорт!" << endl;
        return false;
    }

    int vehicle_index = find_optimal_vehicle_index(orders[order_index].get_weight());
    if (vehicle_index == -1)
    {
        cout << "Ошибка: нет свободного транспорта, способного увезти заказ номер " << order_id
            << " (" << orders[order_index].get_weight() << " кг)!" << endl;
        return false;
    }

    if (vehicles[vehicle_index]->assign_order(orders[order_index]))
    {
        orders[order_index].set_is_assigned(true);
        return true;
    }

    return false;
}

bool DeliveryService::complete_delivery(int vehicle_id)
{
    int vehicle_index = find_vehicle_index_by_id(vehicle_id);
    if (vehicle_index == -1)
    {
        cout << "Ошибка: транспорт с номером " << vehicle_id << " не найден!" << endl;
        return false;
    }

    int order_id = vehicles[vehicle_index]->get_current_order_id();
    if (order_id == -1)
    {
        cout << "Ошибка: у транспорта номер " << vehicle_id << " нет активного заказа!" << endl;
        return false;
    }

    int order_index = find_order_index_by_id(order_id);

    vehicles[vehicle_index]->complete_delivery();

    if (order_index != -1) orders.erase(orders.begin() + order_index);

    return true;
}

Vehicle* DeliveryService::get_vehicle(int id)
{
    int index = find_vehicle_index_by_id(id);
    if (index != -1) return vehicles[index].get();
    return nullptr;
}

Order* DeliveryService::get_order(int id)
{
    int index = find_order_index_by_id(id);
    if (index != -1) return &orders[index];
    return nullptr;
}

bool DeliveryService::check_vehicle_exists(int id) const { return find_vehicle_index_by_id(id) != -1; }
bool DeliveryService::check_order_exists(int id) const { return find_order_index_by_id(id) != -1; }

DeliveryService& DeliveryService::operator+=(const Order& order) { this->add_order(order); return *this; }
DeliveryService& DeliveryService::operator+=(unique_ptr<Vehicle> vehicle) { this->add_vehicle(std::move(vehicle)); return *this; }
DeliveryService& DeliveryService::operator-=(int order_id) { this->remove_order_by_id(order_id); return *this; }
DeliveryService& DeliveryService::operator-=(const Order& order) { return *this -= order.get_id(); }