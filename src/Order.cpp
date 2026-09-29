#include "Order.h"
#include "InputUtils.h"
#include "Exceptions.h"
#include <iostream>
#include <format>

using namespace std;

Order::Order() : id(0), address(""), weight(MIN_WEIGHT), district(""), is_assigned(false) {}

Order::Order(int id, const string& address, double weight, const string& district)
{
    this->id = id;
    this->address = address;
    this->district = district;
    this->is_assigned = false;

    if (weight < MIN_WEIGHT || weight > MAX_WEIGHT)
        throw InvalidDataException("Вес заказа должен быть от " + format("{:.1f}", MIN_WEIGHT)
            + " до " + format("{:.1f}", MAX_WEIGHT) + " кг, получено: " + format("{:.1f}", weight));

    this->weight = weight;
}

int Order::get_id() const { return id; }
string Order::get_address() const { return address; }
double Order::get_weight() const { return weight; }
string Order::get_district() const { return district; }
bool Order::get_is_assigned() const { return is_assigned; }

void Order::set_id(int new_id)
{
    if (is_assigned)
        throw InvalidOperationException("Нельзя менять номер заказа " + to_string(id) + ", он уже доставляется");

    id = new_id;
}

void Order::set_address(const string& new_address) { address = new_address; }

void Order::set_weight(double new_weight)
{
    if (is_assigned)
        throw InvalidOperationException("Нельзя менять вес заказа " + to_string(id) + ", он уже назначен на транспорт");

    if (new_weight < MIN_WEIGHT || new_weight > MAX_WEIGHT)
        throw InvalidDataException("Вес заказа должен быть от " + format("{:.1f}", MIN_WEIGHT)
            + " до " + format("{:.1f}", MAX_WEIGHT) + " кг, получено: " + format("{:.1f}", new_weight));

    weight = new_weight;
}

void Order::set_district(const string& new_district) { district = new_district; }
void Order::set_is_assigned(bool status) { is_assigned = status; }

string Order::get_full_info() const
{
    string status_text = is_assigned ? "Доставляется" : "Ожидает назначения";
    string weight_str = format("{:.1f}", weight);

    return "Заказ номер " + to_string(id) +
        ". Адрес: " + address +
        ". Район: " + district +
        ". Вес: " + weight_str + " кг" +
        ". Статус: " + status_text;
}

bool Order::operator==(const Order& other) const { return this->id == other.id; }
bool Order::operator!=(const Order& other) const { return !(*this == other); }

bool Order::operator<(const Order& other) const { return this->weight < other.weight; }
bool Order::operator>(const Order& other) const { return this->weight > other.weight; }
bool Order::operator<=(const Order& other) const { return this->weight <= other.weight; }
bool Order::operator>=(const Order& other) const { return this->weight >= other.weight; }

ostream& operator<<(ostream& os, const Order& order)
{
    os << order.get_full_info();
    return os;
}

istream& operator>>(istream& is, Order& order)
{
    order.id = read_int("Введите ID заказа: ");

    cout << "Введите адрес: ";
    getline(is, order.address);

    double input_weight = read_double("Введите вес (кг): ");
    if (input_weight >= Order::MIN_WEIGHT && input_weight <= Order::MAX_WEIGHT) order.weight = input_weight;
    else throw InvalidDataException("Вес заказа должен быть от " + format("{:.1f}", Order::MIN_WEIGHT)
        + " до " + format("{:.1f}", Order::MAX_WEIGHT) + " кг, получено: " + format("{:.1f}", input_weight));

    cout << "Введите район: ";
    getline(is, order.district);

    order.is_assigned = false;

    return is;
}