#include "Truck.h"
#include "InputUtils.h"
#include <iostream>

using namespace std;

Truck::Truck()
    : Vehicle(0, "Грузовик", Vehicle::MAX_TRUCK_CAPACITY, "", true), has_tail_lift(false) {}

Truck::Truck(int id, double capacity, const string& courier_name, bool is_available, bool has_tail_lift)
    : Vehicle(id, "Грузовик", capacity, courier_name, is_available), has_tail_lift(has_tail_lift) {}

bool Truck::get_has_tail_lift() const
{
    return has_tail_lift;
}

void Truck::set_has_tail_lift(bool new_status)
{
    has_tail_lift = new_status;
}

void Truck::toggle_tail_lift()
{
    has_tail_lift = !has_tail_lift;
    cout << "Статус гидроборта у грузовика ID " << get_id()
        << " изменен на: " << (has_tail_lift ? "Установлен" : "Снят") << endl;
}

string Truck::get_full_info() const
{
    return Vehicle::get_full_info() + ". Гидроборт: " + (has_tail_lift ? "Есть" : "Нет");
}

ostream& operator<<(ostream& os, const Truck& truck)
{
    os << truck.get_full_info();
    return os;
}

istream& operator>>(istream& is, Truck& truck)
{
    is >> static_cast<Vehicle&>(truck);
    truck.set_type("Грузовик");

    int choice;
    while (true)
    {
        choice = read_int("Есть ли гидроборт? (1 - Да, 0 - Нет): ");
        if (choice == 0 || choice == 1)
        {
            break;
        }
        cout << "Ошибка: введите 1 (Да) или 0 (Нет)!" << endl;
    }

    truck.has_tail_lift = (choice == 1);
    return is;
}