#include "DeliveryService.h"

using namespace std;

void DeliveryService::PrintAllVehicles() const
{
    cout << "Список транспорта" << endl;
    for (int i = 0; i < vehicles.size(); i++)
    {
        vehicles[i].PrintFullInfo();
    }
}

void DeliveryService::PrintAllOrders() const
{
    cout << "Список активных заказов" << endl;
    for (int i = 0; i < orders.size(); i++)
    {
        orders[i].PrintFullInfo();
    }
}

void DeliveryService::AddVehicle(const Vehicle& vehicle)
{
    if (FindVehicleIndexById(vehicle.GetId()) != -1)
    {
        cout << "Ошибка: транспорт с ID " << vehicle.GetId() << " уже существует!" << endl;
        return;
    }
    vehicles.push_back(vehicle);
}

void DeliveryService::AddOrder(const Order& order)
{
    if (FindOrderIndexById(order.GetId()) != -1)
    {
        cout << "Ошибка: заказ с ID " << order.GetId() << " уже существует!" << endl;
        return;
    }
    orders.push_back(order);
}

bool DeliveryService::RemoveOrderById(int orderId)
{
    int index = FindOrderIndexById(orderId);
    if (index == -1)
    {
        cout << "Ошибка: заказ с номером " << orderId << " не найден!" << endl;
        return false;
    }

    orders.erase(orders.begin() + index);
    cout << "Заказ номер " << orderId << " успешно удален из системы." << endl;
    return true;
}

int DeliveryService::FindOptimalVehicleIndex(double orderWeight) const
{
    int vehicleIndex = -1;
    double minCapacity = 0;

    for (int i = 0; i < vehicles.size(); i++)
    {
        double currCapacity = vehicles[i].GetCapacity();
        if (vehicles[i].GetIsAvailable() && currCapacity >= orderWeight)
        {
            if (vehicleIndex == -1 || currCapacity < minCapacity)
            {
                minCapacity = currCapacity;
                vehicleIndex = i;
            }
        }
    }

    return vehicleIndex;
}

int DeliveryService::FindVehicleIndexById(int id) const
{
    for (int i = 0; i < vehicles.size(); i++)
    {
        if (vehicles[i].GetId() == id)
        {
            return i;
        }
    }
    return -1;
}

int DeliveryService::FindOrderIndexById(int id) const
{
    for (int i = 0; i < orders.size(); i++)
    {
        if (orders[i].GetId() == id)
        {
            return i;
        }
    }
    return -1;
}

bool DeliveryService::CompleteDelivery(int vehicleId, int orderId)
{
    int vehicleIndex = FindVehicleIndexById(vehicleId);
    int orderIndex = FindOrderIndexById(orderId);

    if (vehicleIndex == -1)
    {
        cout << "Ошибка: транспорт с ID " << vehicleId << " не найден!" << endl;
        return false;
    }

    if (orderIndex == -1)
    {
        cout << "Ошибка: заказ с ID " << orderId << " не найден!" << endl;
        return false;
    }

    vehicles[vehicleIndex].CompleteDelivery(orders[orderIndex]);
    orders.erase(orders.begin() + orderIndex);

    return true;
}

bool DeliveryService::AssignOrderToVehicle(int orderId)
{
    int orderIndex = FindOrderIndexById(orderId);
    if (orderIndex == -1)
    {
        cout << "Ошибка: заказ с ID " << orderId << " не найден в системе!" << endl;
        return false;
    }

    int vehicleIndex = FindOptimalVehicleIndex(orders[orderIndex].GetWeight());
    if (vehicleIndex == -1)
    {
        cout << "Ошибка: для заказа номер " << orderId << " нет подходящего свободного транспорта!" << endl;
        return false;
    }

    vehicles[vehicleIndex].AssignOrder(orders[orderIndex]);
    return true;
}