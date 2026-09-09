#include "ConsoleUI.h"
#include "InputUtils.h"
#include "DeliveryService.h"
#include <iostream>
#include <windows.h>
#include <string>

using namespace std;

static void setup_console_encoding();
static void seed_data(DeliveryService& service);
static void print_menu();
static void handle_add_vehicle(DeliveryService& service);
static void handle_add_order(DeliveryService& service);
static void handle_assign_order(DeliveryService& service);
static void handle_complete_delivery(DeliveryService& service);

void run_delivery_app()
{
    setup_console_encoding();

    DeliveryService service;
    seed_data(service);

    print_menu();

    int choice = -1;
    while (choice != 0)
    {
        choice = read_int("Выберите действие: ");

        switch (choice)
        {
        case 1:
            handle_add_vehicle(service);
            break;
        case 2:
            handle_add_order(service);
            break;
        case 3:
            service.print_all_vehicles();
            break;
        case 4:
            service.print_all_orders();
            break;
        case 5:
            handle_assign_order(service);
            break;
        case 6:
            handle_complete_delivery(service);
            break;
        case 0:
            cout << "Завершение работы." << endl;
            break;
        default:
            cout << "Неверный пункт меню!" << endl;
        }
    }
}

static void setup_console_encoding()
{
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
}

static void seed_data(DeliveryService& service)
{
    service.add_vehicle(Vehicle(1, "Мотоцикл", 30.0, "Иван", true));
    service.add_vehicle(Vehicle(2, "Машина", 500.0, "Алексей", true));
    service.add_vehicle(Vehicle(3, "Машина", 1200.0, "Дмитрий", true));

    service.add_order(Order(101, "ул. Ленина, 5", 15.0, "Центральный"));
    service.add_order(Order(102, "пр. Мира, 12", 250.0, "Северный"));

    cout << "Тестовые данные успешно загружены!" << endl;
}

static void print_menu()
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

static void handle_add_vehicle(DeliveryService& service)
{
    int id = read_int("Введите ID транспорта: ");

    cout << "Введите тип (Мотоцикл/Машина): ";
    string type;
    getline(cin, type);

    double capacity = read_double("Введите грузоподъемность (кг): ");

    cout << "Введите имя курьера: ";
    string courier;
    getline(cin, courier);

    if (service.add_vehicle(Vehicle(id, type, capacity, courier, true)))
    {
        cout << "Успех: транспорт добавлен в систему." << endl;
    }
}

static void handle_add_order(DeliveryService& service)
{
    int id = read_int("Введите ID заказа: ");

    cout << "Введите адрес: ";
    string address;
    getline(cin, address);

    double weight = read_double("Введите вес (кг): ");

    cout << "Введите район: ";
    string district;
    getline(cin, district);

    if (service.add_order(Order(id, address, weight, district)))
    {
        cout << "Успех: заказ добавлен в систему." << endl;
    }
}

static void handle_assign_order(DeliveryService& service)
{
    int order_id = read_int("Введите ID заказа: ");
    service.assign_order_to_vehicle(order_id);
}

static void handle_complete_delivery(DeliveryService& service)
{
    int vehicle_id = read_int("Введите ID транспорта, завершившего доставку: ");
    service.complete_delivery(vehicle_id);
}