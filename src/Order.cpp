#include "Order.h"

using namespace std;

Order::Order(int id, const string& address, double weight, const string& district)
{
    this->id = id;
    this->address = address;
    this->district = district;
    this->is_assigned = false;

    if (weight < min_weight || weight > max_weight)
    {
        cout << "Предупреждение: Некорректный вес заказа (" << weight
            << " кг). Установлен вес по умолчанию: " << min_weight << " кг." << endl;
        this->weight = min_weight;
    }
    else this->weight = weight;
}

int Order::get_id() const { return id; }
string Order::get_address() const { return address; }
double Order::get_weight() const { return weight; }
string Order::get_district() const { return district; }
bool Order::get_is_assigned() const { return is_assigned; }

void Order::set_id(int new_id) { id = new_id; }
void Order::set_address(const string& new_address) { address = new_address; }

void Order::set_weight(double new_weight)
{
    if (new_weight < min_weight || new_weight > max_weight)
    {
        cout << "Ошибка: Вес заказа должен быть от " << min_weight
            << " до " << max_weight << " кг!" << endl;
    }
    else weight = new_weight;
}

void Order::set_district(const string& new_district) { district = new_district; }
void Order::set_is_assigned(bool status) { is_assigned = status; }

string Order::get_full_info() const
{
    string status_text = is_assigned ? "Доставляется" : "Ожидает назначения";
    return "Заказ номер " + to_string(id) +
        ". Адрес: " + address +
        ". Район: " + district +
        ". Вес: " + to_string(weight) + " кг" +
        ". Статус: " + status_text;
}

void Order::print_full_info() const
{
    cout << get_full_info() << endl;
}