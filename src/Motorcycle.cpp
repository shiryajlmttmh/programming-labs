#include "Motorcycle.h"
#include "InputUtils.h"
#include <iostream>

using namespace std;

Motorcycle::Motorcycle()
    : Vehicle(0, "Мотоцикл", Vehicle::MAX_MOTORCYCLE_CAPACITY, "", true), has_thermal_box(false) {}

Motorcycle::Motorcycle(int id, double capacity, const string& courier_name, bool is_available, bool has_thermal_box)
    : Vehicle(id, "Мотоцикл", capacity, courier_name, is_available), has_thermal_box(has_thermal_box) {}

bool Motorcycle::get_has_thermal_box() const
{
    return has_thermal_box;
}

void Motorcycle::set_has_thermal_box(bool new_status)
{
    has_thermal_box = new_status;
}

void Motorcycle::toggle_thermal_box()
{
    has_thermal_box = !has_thermal_box;
    cout << "Статус термокороба у мотоцикла ID " << get_id()
        << " изменен на: " << (has_thermal_box ? "Установлен" : "Снят") << endl;
}

string Motorcycle::get_full_info() const
{
    return Vehicle::get_full_info() + ". Термокороб: " + (has_thermal_box ? "Есть" : "Нет");
}

ostream& operator<<(ostream& os, const Motorcycle& motorcycle)
{
    os << motorcycle.get_full_info();
    return os;
}

istream& operator>>(istream& is, Motorcycle& motorcycle)
{
    is >> static_cast<Vehicle&>(motorcycle);
    motorcycle.set_type("Мотоцикл");

    int choice;
    while (true)
    {
        choice = read_int("Есть ли термокороб? (1 - Да, 0 - Нет): ");
        if (choice == 0 || choice == 1) break;
        cout << "Ошибка: введите 1 (Да) или 0 (Нет)!" << endl;
    }

    motorcycle.has_thermal_box = (choice == 1);
    return is;
}