#include "Vehicle.h"

using namespace std;

Vehicle::Vehicle(int id, const string& type, double capacity, const string& courierName, bool isAvailable)
{
    this->id = id;
    this->type = type;
    this->capacity = capacity;
    this->courierName = courierName;
    this->isAvailable = isAvailable;
}

int Vehicle::GetId() const { return id; }
string Vehicle::GetType() const { return type; }
double Vehicle::GetCapacity() const { return capacity; }
string Vehicle::GetCourierName() const { return courierName; }
bool Vehicle::GetIsAvailable() const { return isAvailable; }

void Vehicle::SetId(int newId) { id = newId; }
void Vehicle::SetType(const string& newType) { type = newType; }
void Vehicle::SetCapacity(double newCapacity) { capacity = newCapacity; }
void Vehicle::SetCourierName(const string& newCourierName) { courierName = newCourierName; }
void Vehicle::SetIsAvailable(bool newStatus) { isAvailable = newStatus; }

string Vehicle::GetFullInfo() const
{
    string statusText = isAvailable ? "Свободен" : "Занят";

    return "Транспорт номер " + to_string(id) +
        " (" + type + ")" +
        ". Курьер: " + courierName +
        ". Грузоподъемность: " + to_string(capacity) + " кг" +
        ". Статус: " + statusText;
}

void Vehicle::PrintFullInfo() const
{
    cout << GetFullInfo() << endl;
}