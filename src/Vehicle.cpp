#include "Vehicle.h"
#include "Order.h"
#include "InputUtils.h"
#include "Exceptions.h"
#include <iostream>
#include <format>

using namespace std;

Vehicle::Vehicle(int id, double capacity, const string& courier_name, bool is_available)
{
    this->id = id;
    this->courier_name = courier_name;
    this->is_available = is_available;
    this->current_order_id = -1;

    if (capacity <= 0)
        throw InvalidDataException("Грузоподъемность должна быть положительной, получено: " + format("{:.1f}", capacity));

    this->capacity = capacity;
}

double Vehicle::validate_capacity(double capacity, double max_capacity, const string& type_name)
{
    if (capacity <= 0 || capacity > max_capacity)
        throw InvalidDataException("Некорректная грузоподъемность (" + format("{:.1f}", capacity) + " кг) для "
            + type_name + ". Допустимо: больше 0 и не более " + format("{:.1f}", max_capacity) + " кг");

    return capacity;
}

void Vehicle::validate_cargo_weight(double cargo_weight) const
{
    if (cargo_weight <= 0)
        throw InvalidDataException("Вес груза должен быть положительным, получено: " + format("{:.1f}", cargo_weight));

    if (!can_carry(cargo_weight))
        throw ConstraintViolationException("Груз весом " + format("{:.1f}", cargo_weight)
            + " кг превышает грузоподъемность транспорта номер " + to_string(id)
            + " (" + format("{:.1f}", capacity) + " кг)");
}

bool Vehicle::can_carry(double cargo_weight) const
{
    return cargo_weight <= capacity;
}

void Vehicle::assign_order(const Order& order)
{
    if (!is_available)
        throw InvalidOperationException("Транспорт номер " + to_string(id) + " (" + courier_name + ") занят или недоступен");

    if (!can_carry(order.get_weight()))
        throw ConstraintViolationException("Вес заказа номер " + to_string(order.get_id()) + " ("
            + format("{:.1f}", order.get_weight()) + " кг) превышает грузоподъемность транспорта номер "
            + to_string(id) + " (" + format("{:.1f}", capacity) + " кг)");

    is_available = false;
    current_order_id = order.get_id();

    cout << "Курьер " << courier_name << " (" << get_type() << " номер " << id
        << ") взял заказ номер " << order.get_id()
        << " по адресу: " << order.get_address() << endl;
}

void Vehicle::complete_delivery()
{
    if (is_available || current_order_id == -1)
        throw InvalidOperationException("Транспорт номер " + to_string(id) + " свободен, на нём нет активных заказов");

    cout << "Курьер " << courier_name << " завершил доставку заказа номер " << current_order_id << "." << endl;
    is_available = true;
    current_order_id = -1;
}

int Vehicle::get_id() const { return id; }
double Vehicle::get_capacity() const { return capacity; }
string Vehicle::get_courier_name() const { return courier_name; }
bool Vehicle::get_is_available() const { return is_available; }
int Vehicle::get_current_order_id() const { return current_order_id; }

void Vehicle::set_id(int new_id)
{
    if (current_order_id != -1)
        throw InvalidOperationException("Нельзя менять номер транспорта " + to_string(id) + " во время доставки");

    id = new_id;
}

void Vehicle::set_capacity(double new_capacity)
{
    if (current_order_id != -1)
        throw InvalidOperationException("Нельзя менять грузоподъемность транспорта номер " + to_string(id) + " во время доставки");

    double max_limit = get_max_capacity();
    if (new_capacity <= 0 || new_capacity > max_limit)
        throw InvalidDataException("Для типа \"" + get_type() + "\" грузоподъемность должна быть больше 0 и не более "
            + format("{:.1f}", max_limit) + " кг, получено: " + format("{:.1f}", new_capacity));

    capacity = new_capacity;
}

void Vehicle::set_courier_name(const string& new_courier_name) { courier_name = new_courier_name; }
void Vehicle::set_is_available(bool new_status)
{
    if (current_order_id != -1)
        throw InvalidOperationException("Транспорт номер " + to_string(id) + " выполняет заказ, изменить статус доступности нельзя");

    is_available = new_status;
}

string Vehicle::get_full_info() const
{
    string status_text = is_available ? "Свободен" : ("Занят (Заказ номер " + to_string(current_order_id) + ")");
    string capacity_str = format("{:.1f}", capacity);

    return "Транспорт номер " + to_string(id) +
        " (" + get_type() + ")" +
        ". Курьер: " + courier_name +
        ". Грузоподъемность: " + capacity_str + " кг" +
        ". Статус: " + status_text;
}

bool Vehicle::operator==(const Vehicle& other) const { return this->id == other.id; }
bool Vehicle::operator!=(const Vehicle& other) const { return !(*this == other); }

bool Vehicle::operator<(const Vehicle& other) const { return this->capacity < other.capacity; }
bool Vehicle::operator>(const Vehicle& other) const { return this->capacity > other.capacity; }
bool Vehicle::operator<=(const Vehicle& other) const { return this->capacity <= other.capacity; }
bool Vehicle::operator>=(const Vehicle& other) const { return this->capacity >= other.capacity; }

ostream& operator<<(ostream& os, const Vehicle& vehicle)
{
    os << vehicle.get_full_info();
    return os;
}

istream& operator>>(istream& is, Vehicle& vehicle)
{
    vehicle.id = read_int("Введите ID транспорта: ");

    double max_cap = vehicle.get_max_capacity();
    double cap = read_double("Введите грузоподъемность (кг): ");
    if (cap <= 0 || cap > max_cap)
        throw InvalidDataException("Для типа \"" + vehicle.get_type() + "\" грузоподъемность должна быть больше 0 и не более "
            + format("{:.1f}", max_cap) + " кг, получено: " + format("{:.1f}", cap));

    vehicle.capacity = cap;

    cout << "Введите имя курьера: ";
    getline(is, vehicle.courier_name);

    vehicle.is_available = true;
    vehicle.current_order_id = -1;

    return is;
}