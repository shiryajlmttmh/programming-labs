#include "DeliveryService.h"

using namespace std;

void DeliveryService::PrintAllVehicles() const
{
    cout << "\n--- Список транспорта ---" << endl;
    if (vehicles.empty()) { cout << "Список пуст." << endl; return; }
    for (size_t i = 0; i < vehicles.size(); i++)
    {
        vehicles[i].PrintFullInfo();
    }
}

void DeliveryService::PrintAllOrders() const
{
    cout << "\n--- Список активных заказов ---" << endl;
    if (orders.empty()) { cout << "Список пуст." << endl; return; }
    for (size_t i = 0; i < orders.size(); i++)
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

    if (orders[index].GetIsAssigned())
    {
        cout << "Ошибка: нельзя удалить заказ №" << orderId << ", так как он находится в процессе доставки!" << endl;
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

    for (size_t i = 0; i < vehicles.size(); i++)
    {
        double currCapacity = vehicles[i].GetCapacity();
        if (vehicles[i].GetIsAvailable() && currCapacity >= orderWeight)
        {
            if (vehicleIndex == -1 || currCapacity < minCapacity)
            {
                minCapacity = currCapacity;
                vehicleIndex = static_cast<int>(i);
            }
        }
    }

    return vehicleIndex;
}

int DeliveryService::FindVehicleIndexById(int id) const
{
    for (size_t i = 0; i < vehicles.size(); i++)
    {
        if (vehicles[i].GetId() == id) return static_cast<int>(i);
    }
    return -1;
}

int DeliveryService::FindOrderIndexById(int id) const
{
    for (size_t i = 0; i < orders.size(); i++)
    {
        if (orders[i].GetId() == id) return static_cast<int>(i);
    }
    return -1;
}

bool DeliveryService::AssignOrderToVehicle(int orderId)
{
    int orderIndex = FindOrderIndexById(orderId);
    if (orderIndex == -1)
    {
        cout << "Ошибка: заказ с ID " << orderId << " не найден в системе!" << endl;
        return false;
    }

    if (orders[orderIndex].GetIsAssigned())
    {
        cout << "Ошибка: заказ номер " << orderId << " уже назначен на другой транспорт!" << endl;
        return false;
    }

    int vehicleIndex = FindOptimalVehicleIndex(orders[orderIndex].GetWeight());
    if (vehicleIndex == -1)
    {
        cout << "Ошибка: для заказа номер " << orderId << " нет подходящего свободного транспорта!" << endl;
        return false;
    }

    if (vehicles[vehicleIndex].AssignOrder(orders[orderIndex]))
    {
        orders[orderIndex].SetIsAssigned(true);
        return true;
    }

    return false;
}

bool DeliveryService::CompleteDelivery(int vehicleId)
{
    int vehicleIndex = FindVehicleIndexById(vehicleId);
    if (vehicleIndex == -1)
    {
        cout << "Ошибка: транспорт с ID " << vehicleId << " не найден!" << endl;
        return false;
    }

    int orderId = vehicles[vehicleIndex].GetCurrentOrderId();
    if (orderId == -1)
    {
        cout << "Ошибка: транспорт с ID " << vehicleId << " сейчас не выполняет никаких заказов!" << endl;
        return false;
    }

    int orderIndex = FindOrderIndexById(orderId);

    vehicles[vehicleIndex].CompleteDelivery();

    if (orderIndex != -1)
    {
        orders.erase(orders.begin() + orderIndex);
    }

    return true;
}