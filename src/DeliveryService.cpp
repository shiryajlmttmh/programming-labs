#include "DeliveryService.h"
#include "Exceptions.h"
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

    for (size_t i = 0; i < vehicles.size(); i++)
        vehicles[i]->perform_specific_action();
}

void DeliveryService::print_delivery_costs(double order_weight) const
{
    if (order_weight <= 0)
        throw InvalidDataException("Вес груза должен быть положительным, получено: " + format("{:.1f}", order_weight));

    cout << "\n--- Стоимость доставки груза весом " << order_weight << " кг ---" << endl;
    if (vehicles.empty()) { cout << "Список пуст." << endl; return; }

    for (size_t i = 0; i < vehicles.size(); i++)
    {
        cout << vehicles[i]->get_type() << " номер " << vehicles[i]->get_id() << " (" << vehicles[i]->get_courier_name() << "): ";

        if (!vehicles[i]->can_carry(order_weight))
            cout << "груз превышает грузоподъемность (" << vehicles[i]->get_capacity() << " кг)" << endl;
        else
            cout << format("{:.2f}", vehicles[i]->calculate_delivery_cost(order_weight)) << " руб." << endl;
    }
}

void DeliveryService::print_all_orders() const
{
    cout << "\n--- Список активных заказов ---" << endl;
    if (orders.get_items_count() == 0) { cout << "Список пуст." << endl; return; }
    orders.print_collection();
}

void DeliveryService::add_vehicle(unique_ptr<Vehicle> vehicle)
{
    if (!vehicle) throw InvalidDataException("Невозможно добавить транспорт: передан пустой указатель");

    if (find_vehicle_index_by_id(vehicle->get_id()) != -1)
        throw DuplicateIdException("Транспорт", vehicle->get_id());

    vehicles.push_back(move(vehicle));
}

void DeliveryService::add_order(const Order& order)
{
    if (find_order_index_by_id(order.get_id()) != -1)
        throw DuplicateIdException("Заказ", order.get_id());

    orders.add_item(order);
}

void DeliveryService::remove_order_by_id(int order_id)
{
    int index = find_order_index_by_id(order_id);
    if (index == -1) throw NotFoundException("Заказ", order_id);

    if (orders.get_item_by_index(static_cast<size_t>(index)).get_is_assigned())
        throw InvalidOperationException("Нельзя удалить заказ номер " + to_string(order_id) + ", так как он находится в процессе доставки");

    orders.remove_item_by_index(static_cast<size_t>(index));
    cout << "Заказ номер " << order_id << " успешно удален из системы." << endl;
}

void DeliveryService::change_vehicle_id(int old_id, int new_id)
{
    int index = find_vehicle_index_by_id(old_id);
    if (index == -1) throw NotFoundException("Транспорт", old_id);

    if (new_id != old_id && find_vehicle_index_by_id(new_id) != -1)
        throw DuplicateIdException("Транспорт", new_id);

    vehicles[index]->set_id(new_id);
}

void DeliveryService::change_order_id(int old_id, int new_id)
{
    int index = find_order_index_by_id(old_id);
    if (index == -1) throw NotFoundException("Заказ", old_id);

    if (new_id != old_id && find_order_index_by_id(new_id) != -1)
        throw DuplicateIdException("Заказ", new_id);

    orders.get_item_by_index(static_cast<size_t>(index)).set_id(new_id);
}

int DeliveryService::find_optimal_vehicle_index(double order_weight) const
{
    int vehicle_index = -1;
    double min_capacity = 0;

    for (size_t i = 0; i < vehicles.size(); i++)
    {
        double curr_capacity = vehicles[i]->get_capacity();
        if (vehicles[i]->get_is_available() && vehicles[i]->can_carry(order_weight))
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
    for (size_t i = 0; i < orders.get_items_count(); i++)
        if (orders.get_item_by_index(i).get_id() == id) return static_cast<int>(i);

    return -1;
}

void DeliveryService::assign_order_to_vehicle(int order_id)
{
    int order_index = find_order_index_by_id(order_id);
    if (order_index == -1) throw NotFoundException("Заказ", order_id);

    Order& order = orders.get_item_by_index(static_cast<size_t>(order_index));

    if (order.get_is_assigned())
        throw InvalidOperationException("Заказ номер " + to_string(order_id) + " уже назначен на транспорт");

    int vehicle_index = find_optimal_vehicle_index(order.get_weight());
    if (vehicle_index == -1)
        throw ConstraintViolationException("Нет свободного транспорта, способного увезти заказ номер "
            + to_string(order_id) + " (" + format("{:.1f}", order.get_weight()) + " кг)");

    vehicles[vehicle_index]->assign_order(order);
    order.set_is_assigned(true);
}

void DeliveryService::complete_delivery(int vehicle_id)
{
    int vehicle_index = find_vehicle_index_by_id(vehicle_id);
    if (vehicle_index == -1) throw NotFoundException("Транспорт", vehicle_id);

    int order_id = vehicles[vehicle_index]->get_current_order_id();
    int order_index = find_order_index_by_id(order_id);

    vehicles[vehicle_index]->complete_delivery();

    if (order_index != -1) orders.remove_item_by_index(static_cast<size_t>(order_index));
}

Vehicle* DeliveryService::get_vehicle(int id)
{
    int index = find_vehicle_index_by_id(id);
    if (index == -1) throw NotFoundException("Транспорт", id);

    return vehicles[index].get();
}

Order* DeliveryService::get_order(int id)
{
    int index = find_order_index_by_id(id);
    if (index == -1) throw NotFoundException("Заказ", id);

    return &orders.get_item_by_index(static_cast<size_t>(index));
}

bool DeliveryService::check_vehicle_exists(int id) const { return find_vehicle_index_by_id(id) != -1; }
bool DeliveryService::check_order_exists(int id) const { return find_order_index_by_id(id) != -1; }

size_t DeliveryService::get_vehicles_count() const { return vehicles.size(); }

const Vehicle& DeliveryService::get_vehicle_by_index(size_t index) const
{
    if (index >= vehicles.size())
        throw out_of_range("Индекс транспорта " + to_string(index) + " выходит за пределы списка (размер: "
            + to_string(vehicles.size()) + ")");

    return *vehicles[index];
}

size_t DeliveryService::get_orders_count() const { return orders.get_items_count(); }

const Order& DeliveryService::get_order_by_index(size_t index) const
{
    return orders.get_item_by_index(index);
}

DeliveryService& DeliveryService::operator+=(const Order& order) { this->add_order(order); return *this; }
DeliveryService& DeliveryService::operator+=(unique_ptr<Vehicle> vehicle) { this->add_vehicle(move(vehicle)); return *this; }
DeliveryService& DeliveryService::operator-=(int order_id) { this->remove_order_by_id(order_id); return *this; }
DeliveryService& DeliveryService::operator-=(const Order& order) { return *this -= order.get_id(); }