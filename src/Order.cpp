#include "Order.h"

using namespace std;

Order::Order(int id, const string& address, double weight, const string& district)
{
    this->id = id;
    this->address = address;
    this->district = district;
    this->isAssigned = false;

    if (weight < MIN_WEIGHT || weight > MAX_WEIGHT)
    {
        cout << "Предупреждение: Некорректный вес заказа (" << weight
            << " кг). Установлен вес по умолчанию: " << MIN_WEIGHT << " кг." << endl;
        this->weight = MIN_WEIGHT;
    }
    else this->weight = weight;
}

int Order::GetId() const { return id; }
string Order::GetAddress() const { return address; }
double Order::GetWeight() const { return weight; }
string Order::GetDistrict() const { return district; }
bool Order::GetIsAssigned() const { return isAssigned; }

void Order::SetId(int newId) { id = newId; }
void Order::SetAddress(const string& newAddress) { address = newAddress; }

void Order::SetWeight(double newWeight)
{
    if (newWeight < MIN_WEIGHT || newWeight > MAX_WEIGHT)
    {
        cout << "Ошибка: Вес заказа должен быть от " << MIN_WEIGHT
            << " до " << MAX_WEIGHT << " кг!" << endl;
    }
    else weight = newWeight;
}

void Order::SetDistrict(const string& newDistrict) { district = newDistrict; }
void Order::SetIsAssigned(bool status) { isAssigned = status; }

string Order::GetFullInfo() const
{
    string statusText = isAssigned ? "Доставляется" : "Ожидает назначения";
    return "Заказ номер " + to_string(id) +
        ". Адрес: " + address +
        ". Район: " + district +
        ". Вес: " + to_string(weight) + " кг" +
        ". Статус: " + statusText;
}

void Order::PrintFullInfo() const
{
    cout << GetFullInfo() << endl;
}