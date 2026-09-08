#include "DeliveryService.h"

using namespace std;

void DeliveryService::print_all_vehicles() const
{
    cout << "\n--- Список транспорта ---" << endl;
    if (vehicles.empty()) { cout << "Список пуст." << endl; return; }
    for (int i = 0; i < vehicles.size(); i++) vehicles[i].print_full_info();
}

void DeliveryService::print_all_orders() const
{
    cout << "\n--- Список активных заказов ---" << endl;
    if (orders.empty()) { cout << "Список пуст." << endl; return; }
    for (int i = 0; i < orders.size(); i++) orders[i].print_full_info();
}

bool DeliveryService::add_vehicle(const Vehicle& vehicle)
{
    if (find_vehicle_index_by_id(vehicle.get_id()) != -1)
    {
        cout << "Ошибка: транспорт с ID " << vehicle.get_id() << " уже существует!" << endl;
        return false;
    }
    vehicles.push_back(vehicle);
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

    for (int i = 0; i < vehicles.size(); i++)
    {
        double curr_capacity = vehicles[i].get_capacity();
        if (vehicles[i].get_is_available() && curr_capacity >= order_weight)
        {
            if (vehicle_index == -1 || curr_capacity < min_capacity)
            {
                min_capacity = curr_capacity;
                vehicle_index = i;
            }
        }
    }

    return vehicle_index;
}

int DeliveryService::find_vehicle_index_by_id(int id) const
{
    for (int i = 0; i < vehicles.size(); i++)
        if (vehicles[i].get_id() == id) return i;

    return -1;
}

int DeliveryService::find_order_index_by_id(int id) const
{
    for (int i = 0; i < orders.size(); i++)
        if (orders[i].get_id() == id) return i;

    return -1;
}

bool DeliveryService::assign_order_to_vehicle(int order_id)
{
    int order_index = find_order_index_by_id(order_id);
    if (order_index == -1)
    {
        cout << "Ошибка: заказ с ID " << order_id << " не найден в системе!" << endl;
        return false;
    }

    if (orders[order_index].get_is_assigned())
    {
        cout << "Ошибка: заказ номер " << order_id << " уже назначен на другой транспорт!" << endl;
        return false;
    }

    int vehicle_index = find_optimal_vehicle_index(orders[order_index].get_weight());
    if (vehicle_index == -1)
    {
        cout << "Ошибка: для заказа номер " << order_id << " нет подходящего свободного транспорта!" << endl;
        return false;
    }

    if (vehicles[vehicle_index].assign_order(orders[order_index]))
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
        cout << "Ошибка: транспорт с ID " << vehicle_id << " не найден!" << endl;
        return false;
    }

    int order_id = vehicles[vehicle_index].get_current_order_id();
    if (order_id == -1)
    {
        cout << "Ошибка: транспорт с ID " << vehicle_id << " сейчас не выполняет никаких заказов!" << endl;
        return false;
    }

    int order_index = find_order_index_by_id(order_id);

    vehicles[vehicle_index].complete_delivery();

    if (order_index != -1) orders.erase(orders.begin() + order_index);

    return true;
}