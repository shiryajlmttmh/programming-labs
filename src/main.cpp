#include <iostream>
#include <windows.h>
#include <string>
#include "DeliveryService.h"

using namespace std;

void clear_buffer();
int read_int(const string& prompt);
double read_double(const string& prompt);
void seed_data(DeliveryService& service);
void print_menu();
void handle_add_vehicle(DeliveryService& service);
void handle_add_order(DeliveryService& service);
void handle_assign_order(DeliveryService& service);
void handle_complete_delivery(DeliveryService& service);

int main()
{
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);

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

    return 0;
}

void clear_buffer()
{
    while (cin.get() != '\n' && cin);
}

int read_int(const string& prompt)
{
    int value;
    while (true)
    {
        cout << prompt;
        if (cin >> value)
        {
            char next_char = cin.get();

            while (next_char == ' ' || next_char == '\t') next_char = cin.get();

            if (next_char == '\n' || next_char == EOF) return value;
        }
        else cin.clear();

        clear_buffer();
        cout << "Ошибка: введите корректное целое число без посторонних символов!" << endl;
    }
}

double read_double(const string& prompt)
{
    double value;
    while (true)
    {
        cout << prompt;
        if (cin >> value)
        {
            char next_char = cin.get();

            while (next_char == ' ' || next_char == '\t') next_char = cin.get();

            if (next_char == '\n' || next_char == EOF) return value;
        }
        else cin.clear();

        clear_buffer();
        cout << "Ошибка: введите корректное число без посторонних символов!" << endl;
    }
}

void seed_data(DeliveryService& service)
{
    service.add_vehicle(Vehicle(1, "Мотоцикл", 30.0, "Иван", true));
    service.add_vehicle(Vehicle(2, "Машина", 500.0, "Алексей", true));
    service.add_vehicle(Vehicle(3, "Машина", 1200.0, "Дмитрий", true));

    service.add_order(Order(101, "ул. Ленина, 5", 15.0, "Центральный"));
    service.add_order(Order(102, "пр. Мира, 12", 250.0, "Северный"));

    cout << "Тестовые данные успешно загружены!" << endl;
}

void print_menu()
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

void handle_add_vehicle(DeliveryService& service)
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

void handle_add_order(DeliveryService& service)
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

void handle_assign_order(DeliveryService& service)
{
    int order_id = read_int("Введите ID заказа: ");
    service.assign_order_to_vehicle(order_id);
}

void handle_complete_delivery(DeliveryService& service)
{
    int vehicle_id = read_int("Введите ID транспорта, завершившего доставку: ");
    service.complete_delivery(vehicle_id);
}