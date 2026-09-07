#include "Vehicle.h"
#include "Order.h"

using namespace std;

bool Vehicle::is_valid_type(const string& check_type) const
{
    return (check_type == "Мотоцикл" || check_type == "Машина");
}

double Vehicle::get_max_capacity_for_type(const string& check_type) const
{
    if (check_type == "Мотоцикл") return MAX_MOTORCYCLE_CAPACITY;
    return MAX_CAR_CAPACITY;
}

Vehicle::Vehicle(int id, const string& type, double capacity, const string& courier_name, bool is_available)
{
    this->id = id;
    this->courier_name = courier_name;
    this->is_available = is_available;
    this->current_order_id = -1;

    if (is_valid_type(type)) this->type = type;
    else
    {
        cout << "Предупреждение: Неизвестный тип транспорта '" << type
            << "'. Установлен тип по умолчанию: 'Машина'." << endl;
        this->type = "Машина";
    }

    double max_limit = get_max_capacity_for_type(this->type);
    if (capacity <= 0 || capacity > max_limit)
    {
        cout << "Предупреждение: Некорректная грузоподъемность (" << capacity
            << " кг) для типа " << this->type
            << ". Установлено максимальное значение: " << max_limit << " кг." << endl;
        this->capacity = max_limit;
    }
    else this->capacity = capacity;
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

    cout << "Курьер " << courier_name << " (" << type << " номер " << id
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
string Vehicle::get_type() const { return type; }
double Vehicle::get_capacity() const { return capacity; }
string Vehicle::get_courier_name() const { return courier_name; }
bool Vehicle::get_is_available() const { return is_available; }
int Vehicle::get_current_order_id() const { return current_order_id; }

void Vehicle::set_id(int new_id) { id = new_id; }

void Vehicle::set_type(const string& new_type)
{
    if (!is_valid_type(new_type))
    {
        cout << "Ошибка: Разрешены только типы 'Мотоцикл' и 'Машина'!" << endl;
        return;
    }

    type = new_type;
    double max_limit = get_max_capacity_for_type(type);
    if (capacity > max_limit)
    {
        capacity = max_limit;
        cout << "Грузоподъемность скорректирована под новый тип: " << capacity << " кг." << endl;
    }
}

void Vehicle::set_capacity(double new_capacity)
{
    double max_limit = get_max_capacity_for_type(type);
    if (new_capacity > 0 && new_capacity <= max_limit) capacity = new_capacity;
    else
    {
        cout << "Ошибка: Для типа " << type << " грузоподъемность должна быть от 0 до "
            << max_limit << " кг!" << endl;
    }
}

void Vehicle::set_courier_name(const string& new_courier_name) { courier_name = new_courier_name; }
void Vehicle::set_is_available(bool new_status) { is_available = new_status; }

string Vehicle::get_full_info() const
{
    string status_text = is_available ? "Свободен" : ("Занят (Заказ номер " + to_string(current_order_id) + ")");

    return "Транспорт номер " + to_string(id) +
        " (" + type + ")" +
        ". Курьер: " + courier_name +
        ". Грузоподъемность: " + to_string(capacity) + " кг" +
        ". Статус: " + status_text;
}

void Vehicle::print_full_info() const
{
    cout << get_full_info() << endl;
}