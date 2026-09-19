#include "Car.h"
#include "InputUtils.h"
#include <iostream>
#include <format>

using namespace std;

Car::Car()
    : Vehicle(0, MAX_CAPACITY, "", true), trunk_volume(DEFAULT_TRUNK_VOLUME) {}

Car::Car(int id, double capacity, const string& courier_name, bool is_available, double trunk_volume)
    : Vehicle(id, validate_capacity(capacity, MAX_CAPACITY, "машины"), courier_name, is_available)
{
    if (trunk_volume >= MIN_TRUNK_VOLUME && trunk_volume <= MAX_TRUNK_VOLUME)
    {
        this->trunk_volume = trunk_volume;
    }
    else
    {
        cout << "Предупреждение: Некорректный объём багажника (" << trunk_volume
            << " л). Установлено значение по умолчанию: " << DEFAULT_TRUNK_VOLUME << " л." << endl;
        this->trunk_volume = DEFAULT_TRUNK_VOLUME;
    }
}

double Car::get_trunk_volume() const
{
    return trunk_volume;
}

void Car::set_trunk_volume(double new_volume)
{
    if (new_volume >= MIN_TRUNK_VOLUME && new_volume <= MAX_TRUNK_VOLUME)
    {
        trunk_volume = new_volume;
    }
    else
    {
        cout << "Ошибка: объём багажника должен быть от " << MIN_TRUNK_VOLUME
            << " до " << MAX_TRUNK_VOLUME << " литров!" << endl;
    }
}

bool Car::is_trunk_spacious() const
{
    return trunk_volume >= SPACIOUS_TRUNK_THRESHOLD;
}

string Car::get_type() const
{
    return "Машина";
}

double Car::get_max_capacity() const
{
    return MAX_CAPACITY;
}

string Car::get_full_info() const
{
    return Vehicle::get_full_info() + ". Объём багажника: " + std::format("{:.1f}", trunk_volume) + " л";
}

void Car::perform_specific_action()
{
    if (is_trunk_spacious())
    {
        cout << "Багажник у машины ID " << get_id() << " вместительный (объём: "
            << trunk_volume << " л)." << endl;
    }
    else
    {
        cout << "Багажник у машины ID " << get_id() << " стандартный (объём: "
            << trunk_volume << " л)." << endl;
    }
}

string Car::get_specific_action_name() const
{
    return "Проверить вместительность багажника";
}

double Car::calculate_delivery_cost(double order_weight) const
{
    double cost = BASE_COST + COST_PER_KG * order_weight;
    if (is_trunk_spacious()) cost *= SPACIOUS_TRUNK_DISCOUNT;
    return cost;
}

istream& operator>>(istream& is, Car& car)
{
    is >> static_cast<Vehicle&>(car);

    double volume;
    while (true)
    {
        volume = read_double("Введите объём багажника (в литрах): ");
        if (volume >= Car::MIN_TRUNK_VOLUME && volume <= Car::MAX_TRUNK_VOLUME) break;
        cout << "Ошибка: объём багажника должен быть от " << Car::MIN_TRUNK_VOLUME
            << " до " << Car::MAX_TRUNK_VOLUME << " литров!" << endl;
    }

    car.trunk_volume = volume;
    return is;
}