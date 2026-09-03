#include "Order.h"

using namespace std;

Order::Order(int id, const string& address, double weight, const string& district)
{
    this->id = id;
    this->address = address;
    this->weight = weight;
    this->district = district;
}

int Order::GetId() const { return id; }
string Order::GetAddress() const { return address; }
double Order::GetWeight() const { return weight; }
string Order::GetDistrict() const { return district; }

void Order::SetId(int newId) { id = newId; }
void Order::SetAddress(const string& newAddress) { address = newAddress; }
void Order::SetWeight(double newWeight) { weight = newWeight; }
void Order::SetDistrict(const string& newDistrict) { district = newDistrict; }

string Order::GetFullInfo() const
{
    return "Заказ номер " + to_string(id) +
        ". Адрес: " + address +
        ". Район: " + district +
        ". Вес: " + to_string(weight) + " кг";
}

void Order::PrintFullInfo() const
{
    cout << GetFullInfo() << endl;
}