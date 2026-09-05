#include "Vehicle.h"
#include "Order.h"

using namespace std;

bool Vehicle::IsValidType(const string& checkType) const
{
    return (checkType == "Мотоцикл" || checkType == "Машина");
}

double Vehicle::GetMaxCapacityForType(const string& checkType) const
{
    if (checkType == "Мотоцикл") return MAX_MOTORCYCLE_CAPACITY;
    if (checkType == "Машина") return MAX_CAR_CAPACITY;
    return MAX_CAR_CAPACITY; // дефолт
}

Vehicle::Vehicle(int id, const string& type, double capacity, const string& courierName, bool isAvailable)
{
    this->id = id;
    this->courierName = courierName;
    this->isAvailable = isAvailable;

    if (IsValidType(type)) this->type = type;
    else
    {
        cout << "Предупреждение: Неизвестный тип транспорта '" << type
            << "'. Установлен тип по умолчанию: 'Машина'." << endl;
        this->type = "Машина";
    }

    double maxLimit = GetMaxCapacityForType(this->type);
    if (capacity <= 0 || capacity > maxLimit)
    {
        cout << "Предупреждение: Некорректная грузоподъемность (" << capacity
            << " кг) для типа " << this->type
            << ". Установлено максимальное значение: " << maxLimit << " кг." << endl;
        this->capacity = maxLimit;
    }
    else this->capacity = capacity;
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

void Vehicle::SetType(const string& newType) 
{
    if (!IsValidType(newType))
    {
        cout << "Ошибка: Разрешены только типы 'Мотоцикл' и 'Машина'!" << endl;
        return;
    }

    type = newType;
    double maxLimit = GetMaxCapacityForType(type);
    if (capacity > maxLimit)
    {
        capacity = maxLimit;
        cout << "Грузоподъемность скорректирована под новый тип: " << capacity << " кг." << endl;
    }
}

void Vehicle::SetCapacity(double newCapacity)
{
    double maxLimit = GetMaxCapacityForType(type);
    if (newCapacity > 0 && newCapacity <= maxLimit)
    {
        capacity = newCapacity;
    }
    else
    {
        cout << "Ошибка: Для типа " << type << " грузоподъемность должна быть от 0 до "
            << maxLimit << " кг!" << endl;
    }
}

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