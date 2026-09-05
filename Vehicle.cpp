#include "Vehicle.h"
#include "Order.h"

using namespace std;

Vehicle::Vehicle(int id, const string& type, double capacity, const string& courierName, bool isAvailable)
{
    this->id = id;
    this->type = type;
    this->capacity = capacity;
    this->courierName = courierName;
    this->isAvailable = isAvailable;
}

bool Vehicle::AssignOrder(const Order& order)
{
    if (!isAvailable)
    {
        cout << "Ошибка: транспорт номер " << id << " (" << courierName << ") уже занят!" << endl;
        return false;
    }

    if (order.GetWeight() > capacity) 
    {
        cout << "Ошибка: вес заказа номер " << order.GetId() << " (" << order.GetWeight()
            << " кг) превышает грузоподъемность транспорта (" << capacity << " кг)!" << endl;
        return false;
    }

    isAvailable = false; 
    cout << "Курьер " << courierName << "(" << type << " номер " << id
        << ") взял заказ номер " << order.GetId()
        << " по адресу: " << order.GetAddress() << endl;

    return true;
}

void Vehicle::CompleteDelivery(const Order& order)
{
    if (isAvailable) 
    {
        cout << "Предупреждение: транспорт № " << id << " и так свободен, он не выполнял доставку." << endl;
        return;
    }

    isAvailable = true;
    cout << "Заказ номер " << order.GetId() << " успешно доставлен в район "
        << order.GetDistrict() << ". Курьер " << courierName << " снова свободен." << endl;
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