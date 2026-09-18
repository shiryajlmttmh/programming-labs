#include "Car.h"
#include "InputUtils.h"
#include <iostream>

using namespace std;

Car::Car()
    : Vehicle(0, "Машина", Vehicle::MAX_CAR_CAPACITY, "", true), trunk_volume(MIN_TRUNK_VOLUME) {}

Car::Car(int id, double capacity, const string& courier_name, bool is_available, double trunk_volume)
    : Vehicle(id, "Машина", capacity, courier_name, is_available)
{
    if (trunk_volume >= MIN_TRUNK_VOLUME && trunk_volume <= MAX_TRUNK_VOLUME)
    {
        this->trunk_volume = trunk_volume;
    }
    else
    {
        cout << "Предупреждение: Некорректный объём багажника (" << trunk_volume
            << " л). Установлено значение по умолчанию: " << MIN_TRUNK_VOLUME << " л." << endl;
        this->trunk_volume = MIN_TRUNK_VOLUME;
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

string Car::get_full_info() const
{
    return Vehicle::get_full_info() + ". Объём багажника: " + to_string(trunk_volume) + " л";
}

ostream& operator<<(ostream& os, const Car& car)
{
    os << car.get_full_info();
    return os;
}

istream& operator>>(istream& is, Car& car)
{
    is >> static_cast<Vehicle&>(car);
    car.set_type("Машина");

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