#include "DeliveryService.h"

using namespace std;

void DeliveryService::AddVehicle(const Vehicle& vehicle) { vehicles.push_back(vehicle); }
void DeliveryService::AddOrder(const Order& order) { orders.push_back(order); }

bool DeliveryService::FindVehicleForOrder(const Order& order)
{
    int vehicleIndex = -1;
    double minCapacity = 0;

    for (int i = 0; i < vehicles.size(); i++)
    {
        double currCapacity = vehicles[i].GetCapacity();
        if (vehicles[i].GetIsAvailable() && currCapacity >= order.GetWeight())
        {
            if (vehicleIndex == -1 || currCapacity < minCapacity)
            {
                minCapacity = currCapacity;
                vehicleIndex = i;
            }
        }
    }

    if (vehicleIndex == -1)
    {
        cout << "Ошибка: для заказа номер " << order.GetId() << " нет подходящего свободного транспорта!" << endl;
        return false;
    }

    vehicles[vehicleIndex].AssignOrder(order);
    return true;
}