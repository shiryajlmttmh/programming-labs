#include <iostream>
#include <windows.h>
#include <string>
#include "DeliveryService.h"

using namespace std;

void ClearBuffer();
int ReadInt(const string& prompt);
double ReadDouble(const string& prompt);
void SeedData(DeliveryService& service);
void PrintMenu();
void HandleAddVehicle(DeliveryService& service);
void HandleAddOrder(DeliveryService& service);
void HandleAssignOrder(DeliveryService& service);
void HandleCompleteDelivery(DeliveryService& service);

int main()
{
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);

    DeliveryService service;
    SeedData(service);

    PrintMenu();

    int choice = -1;
    while (choice != 0)
    {
        choice = ReadInt("Выберите действие: ");

        switch (choice)
        {
        case 1:
            HandleAddVehicle(service);
            break;
        case 2:
            HandleAddOrder(service);
            break;
        case 3:
            service.PrintAllVehicles();
            break;
        case 4:
            service.PrintAllOrders();
            break;
        case 5:
            HandleAssignOrder(service);
            break;
        case 6:
            HandleCompleteDelivery(service);
            break;
        case 0:
            cout << "Завершение работы." << endl;
            break;
        default:
            cout << "Неверный пункт меню!" << endl;
        }
    }

    return 0;
}

void ClearBuffer()
{
    while (cin.get() != '\n' && cin);
}

int ReadInt(const string& prompt)
{
    int value;
    while (true)
    {
        cout << prompt;
        if (cin >> value)
        {
            char nextChar = cin.get();

            while (nextChar == ' ' || nextChar == '\t') nextChar = cin.get();

            if (nextChar == '\n' || nextChar == EOF) return value;
        }
        else cin.clear();

        ClearBuffer();
        cout << "Ошибка: введите корректное целое число без посторонних символов!" << endl;
    }
}

double ReadDouble(const string& prompt)
{
    double value;
    while (true)
    {
        cout << prompt;
        if (cin >> value)
        {
            char nextChar = cin.get();

            while (nextChar == ' ' || nextChar == '\t') nextChar = cin.get();

            if (nextChar == '\n' || nextChar == EOF) return value;
        }
        else cin.clear();

        ClearBuffer();
        cout << "Ошибка: введите корректное число без посторонних символов!" << endl;
    }
}

void SeedData(DeliveryService& service)
{
    service.AddVehicle(Vehicle(1, "Мотоцикл", 30.0, "Иван", true));
    service.AddVehicle(Vehicle(2, "Машина", 500.0, "Алексей", true));
    service.AddVehicle(Vehicle(3, "Машина", 1200.0, "Дмитрий", true));

    service.AddOrder(Order(101, "ул. Ленина, 5", 15.0, "Центральный"));
    service.AddOrder(Order(102, "пр. Мира, 12", 250.0, "Северный"));

    cout << "Тестовые данные успешно загружены!" << endl;
}

void PrintMenu()
{
    cout << "\n--- МЕНЮ СЛУЖБЫ ДОСТАВКИ ---" << endl;
    cout << "1. Добавить транспорт вручную\n"
        << "2. Добавить заказ вручную\n"
        << "3. Показать транспорт\n"
        << "4. Показать заказы\n"
        << "5. Назначить заказ на транспорт\n"
        << "6. Завершить доставку по ID транспорта\n"
        << "0. Выход\n";
}

void HandleAddVehicle(DeliveryService& service)
{
    int id = ReadInt("Введите ID транспорта: ");

    cout << "Введите тип (Мотоцикл/Машина): ";
    string type;
    getline(cin, type);

    double capacity = ReadDouble("Введите грузоподъемность (кг): ");

    cout << "Введите имя курьера: ";
    string courier;
    getline(cin, courier);

    service.AddVehicle(Vehicle(id, type, capacity, courier, true));
    cout << "Успех: транспорт добавлен в систему." << endl;
}

void HandleAddOrder(DeliveryService& service)
{
    int id = ReadInt("Введите ID заказа: ");

    cout << "Введите адрес: ";
    string address;
    getline(cin, address);

    double weight = ReadDouble("Введите вес (кг): ");

    cout << "Введите район: ";
    string district;
    getline(cin, district);

    service.AddOrder(Order(id, address, weight, district));
    cout << "Успех: заказ добавлен в систему." << endl;
}

void HandleAssignOrder(DeliveryService& service)
{
    int orderId = ReadInt("Введите ID заказа: ");
    service.AssignOrderToVehicle(orderId);
}

void HandleCompleteDelivery(DeliveryService& service)
{
    int vehicleId = ReadInt("Введите ID транспорта, завершившего доставку: ");
    service.CompleteDelivery(vehicleId);
}