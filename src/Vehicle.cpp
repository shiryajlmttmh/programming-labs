#include "Vehicle.h"
#include "Order.h"
#include "InputUtils.h"
#include <iostream>
#include <format>

using namespace std;

Vehicle::Vehicle(int id, double capacity, const string& courier_name, bool is_available)
{
    this->id = id;
    this->courier_name = courier_name;
    this->is_available = is_available;
    this->current_order_id = -1;
    this->capacity = (capacity > 0) ? capacity : 0.0;
}

double Vehicle::validate_capacity(double capacity, double max_capacity, const string& type_name)
{
    if (capacity <= 0 || capacity > max_capacity)
    {
        cout << "Предупреждение: Некорректная грузоподъемность (" << capacity
            << " кг) для " << type_name << ". Установлено максимальное значение: " << max_capacity << " кг." << endl;
        return max_capacity;
    }
    return capacity;
}

bool Vehicle::assign_order(const Order& order)
{
    if (!is_available)
    {
        cout << "Ошибка: транспорт номер " << id << " (" << courier_name << ") уже занят!" << endl;
        return false;
    }

    if (order.get_weight() > capacity)
    {
        cout << "Ошибка: вес заказа номер " << order.get_id() << " (" << order.get_weight()
            << " кг) превышает грузоподъемность транспорта (" << capacity << " кг)!" << endl;
        return false;
    }

    is_available = false;
    current_order_id = order.get_id();

    cout << "Курьер " << courier_name << " (" << get_type() << " номер " << id
        << ") взял заказ номер " << order.get_id()
        << " по адресу: " << order.get_address() << endl;

    return true;
}

void Vehicle::complete_delivery()
{
    if (is_available || current_order_id == -1)
    {
        cout << "Предупреждение: транспорт номер " << id << " свободный, на нем нет активных заказов." << endl;
        return;
    }

    cout << "Курьер " << courier_name << " завершил доставку заказа номер " << current_order_id << "." << endl;
    is_available = true;
    current_order_id = -1;
}

int Vehicle::get_id() const { return id; }
double Vehicle::get_capacity() const { return capacity; }
string Vehicle::get_courier_name() const { return courier_name; }
bool Vehicle::get_is_available() const { return is_available; }
int Vehicle::get_current_order_id() const { return current_order_id; }

void Vehicle::set_id(int new_id) { id = new_id; }

void Vehicle::set_capacity(double new_capacity)
{
    double max_limit = get_max_capacity();
    if (new_capacity > 0 && new_capacity <= max_limit) capacity = new_capacity;
    else
    {
        cout << "Ошибка: Для " << get_type() << " грузоподъемность должна быть от 0 до "
            << max_limit << " кг!" << endl;
    }
}

void Vehicle::set_courier_name(const string& new_courier_name) { courier_name = new_courier_name; }
void Vehicle::set_is_available(bool new_status)
{
    if (current_order_id != -1)
    {
        cout << "Ошибка: транспорт номер " << id << " выполняет заказ, изменить статус доступности нельзя!" << endl;
        return;
    }
    is_available = new_status;
}

string Vehicle::get_full_info() const
{
    string status_text = is_available ? "Свободен" : ("Занят (Заказ номер " + to_string(current_order_id) + ")");
    string capacity_str = std::format("{:.1f}", capacity);

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
    while (true)
    {
        double cap = read_double("Введите грузоподъемность (кг): ");
        if (cap > 0 && cap <= max_cap)
        {
            vehicle.capacity = cap;
            break;
        }
        cout << "Ошибка: грузоподъемность для " << vehicle.get_type() << " должна быть от 0 до " << max_cap << " кг!" << endl;
    }

    cout << "Введите имя курьера: ";
    getline(is, vehicle.courier_name);

    vehicle.is_available = true;
    vehicle.current_order_id = -1;

    return is;
}