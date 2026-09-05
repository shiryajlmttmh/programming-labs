#include <iostream>
#include <windows.h>
#include "DeliveryService.h"

using namespace std;

void SeedData(DeliveryService& service)
{
    service.AddVehicle(Vehicle(1, "Мотоцикл", 30.0, "Иван", true));
    service.AddVehicle(Vehicle(2, "Машина", 500.0, "Алексей", true));
    service.AddVehicle(Vehicle(3, "Машина", 1200.0, "Дмитрий", true));

    service.AddOrder(Order(101, "ул. Ленина, 5", 15.0, "Центральный"));
    service.AddOrder(Order(102, "пр. Мира, 12", 250.0, "Северный"));

    cout << "Тестовые данные успешно загружены!" << endl;
}

int main()
{
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);

    DeliveryService service;

    SeedData(service);

    cout << "\n--- МЕНЮ СЛУЖБЫ ДОСТАВКИ ---" << endl;
    cout << "1. Показать транспорт" << endl;
    cout << "2. Показать заказы" << endl;
    cout << "3. Назначить заказ на транспорт" << endl;
    cout << "4. Завершить доставку" << endl;
    cout << "0. Выход" << endl;

    int choice = -1;
    while (choice != 0)
    {
        cout << "Выберите действие: ";
        cin >> choice;
        cout << "\n";

        switch (choice)
        {
        case 1:
            service.PrintAllVehicles();
            break;
        case 2:
            service.PrintAllOrders();
            break;
        case 3: {
            int orderId;
            cout << "Введите ID заказа: ";
            cin >> orderId;
            service.AssignOrderToVehicle(orderId);
            break;
        }
        case 4: {
            int vehicleId, orderId;
            cout << "Введите ID транспорта: ";
            cin >> vehicleId;
            cout << "Введите ID заказа: ";
            cin >> orderId;
            service.CompleteDelivery(vehicleId, orderId);
            break;
        }
        case 0:
            cout << "Завершение работы." << endl;
            break;
        default:
            cout << "Неверный пункт меню!" << endl;
        }
    }

    return 0;
}