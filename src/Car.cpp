#include "Car.h"
#include "InputUtils.h"
#include "Exceptions.h"
#include <iostream>
#include <format>

using namespace std;

static string make_volume_error(double volume, double min_volume, double max_volume)
{
    return "Некорректный объём багажника (" + format("{:.1f}", volume) + " л). Допустимо: от "
        + format("{:.1f}", min_volume) + " до " + format("{:.1f}", max_volume) + " л";
}

Car::Car()
    : Vehicle(0, MAX_CAPACITY, "", true), trunk_volume(DEFAULT_TRUNK_VOLUME) {}

Car::Car(int id, double capacity, const string& courier_name, bool is_available, double trunk_volume)
    : Vehicle(id, validate_capacity(capacity, MAX_CAPACITY, "машины"), courier_name, is_available)
{
    if (trunk_volume < MIN_TRUNK_VOLUME || trunk_volume > MAX_TRUNK_VOLUME)
        throw InvalidDataException(make_volume_error(trunk_volume, MIN_TRUNK_VOLUME, MAX_TRUNK_VOLUME));

    this->trunk_volume = trunk_volume;
}

double Car::get_trunk_volume() const
{
    return trunk_volume;
}

void Car::set_trunk_volume(double new_volume)
{
    if (new_volume < MIN_TRUNK_VOLUME || new_volume > MAX_TRUNK_VOLUME)
        throw InvalidDataException(make_volume_error(new_volume, MIN_TRUNK_VOLUME, MAX_TRUNK_VOLUME));

    trunk_volume = new_volume;
}

bool Car::is_trunk_spacious() const
{
    return trunk_volume >= SPACIOUS_TRUNK_THRESHOLD;
}

string Car::get_type() const
{
    return "Машина";
}

string Car::get_type_code() const
{
    return "CAR";
}

string Car::get_specific_field() const
{
    return format("{}", trunk_volume);
}

double Car::get_max_capacity() const
{
    return MAX_CAPACITY;
}

string Car::get_full_info() const
{
    return Vehicle::get_full_info() + ". Объём багажника: " + format("{:.1f}", trunk_volume) + " л";
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
    validate_cargo_weight(order_weight);

    double cost = BASE_COST + COST_PER_KG * order_weight;
    if (is_trunk_spacious()) cost *= SPACIOUS_TRUNK_DISCOUNT;
    return cost;
}

istream& operator>>(istream& is, Car& car)
{
    is >> static_cast<Vehicle&>(car);

    double volume = read_double("Введите объём багажника (в литрах): ");
    if (volume < Car::MIN_TRUNK_VOLUME || volume > Car::MAX_TRUNK_VOLUME)
        throw InvalidDataException(make_volume_error(volume, Car::MIN_TRUNK_VOLUME, Car::MAX_TRUNK_VOLUME));

    car.trunk_volume = volume;
    return is;
}