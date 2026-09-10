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
static void print_manage_vehicle_menu(int id);
static void print_manage_order_menu(int id);

static void handle_add_vehicle(DeliveryService& service);
static void handle_add_order(DeliveryService& service);
static void handle_manage_vehicle(DeliveryService& service);
static void handle_manage_order(DeliveryService& service);
static void handle_assign_order(DeliveryService& service);
static void handle_complete_delivery(DeliveryService& service);

static void handle_change_vehicle_id(DeliveryService& service, Vehicle* v);
static void handle_change_vehicle_type(Vehicle* v);
static void handle_change_vehicle_capacity(Vehicle* v);
static void handle_change_vehicle_courier(Vehicle* v);
static void handle_change_vehicle_status(Vehicle* v);

static void handle_change_order_id(DeliveryService& service, Order* o);
static void handle_change_order_address(Order* o);
static void handle_change_order_weight(Order* o);
static void handle_change_order_district(Order* o);

void run_delivery_app()
{
    setup_console_encoding();

    DeliveryService service;
    seed_data(service);

    int choice = -1;
    while (choice != 0)
    {
        print_menu();
        choice = read_int("Выберите действие: ");

        switch (choice)
        {
        case 1: handle_add_vehicle(service); break;
        case 2: handle_add_order(service); break;
        case 3: handle_manage_vehicle(service); break;
        case 4: handle_manage_order(service); break;
        case 5: service.print_all_vehicles(); break;
        case 6: service.print_all_orders(); break;
        case 7: handle_assign_order(service); break;
        case 8: handle_complete_delivery(service); break;
        case 0: cout << "Завершение работы." << endl; break;
        default: cout << "Неверный пункт меню!" << endl;
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
        << "3. Управление транспортом (Просмотр и изменение)\n"
        << "4. Управление заказом (Просмотр и изменение)\n"
        << "5. Показать транспорт\n"
        << "6. Показать заказы\n"
        << "7. Назначить заказ на транспорт\n"
        << "8. Завершить доставку по ID транспорта\n"
        << "0. Выход\n";
}

static void print_manage_vehicle_menu(int id)
{
    cout << "\n--- Управление транспортом ID " << id << " ---" << endl;
    cout << "[Просмотр полей]\n"
        << "1. Показать полную информацию\n"
        << "2. Показать ID\n"
        << "3. Показать тип\n"
        << "4. Показать грузоподъемность\n"
        << "5. Показать имя курьера\n"
        << "6. Показать статус доступности\n"
        << "[Изменение полей]\n"
        << "7. Изменить ID\n"
        << "8. Изменить тип\n"
        << "9. Изменить грузоподъемность\n"
        << "10. Изменить имя курьера\n"
        << "11. Изменить статус доступности\n"
        << "0. Назад в главное меню\n";
}

static void print_manage_order_menu(int id)
{
    cout << "\n--- Управление заказом ID " << id << " ---" << endl;
    cout << "[Просмотр полей]\n"
        << "1. Показать полную информацию\n"
        << "2. Показать ID\n"
        << "3. Показать адрес\n"
        << "4. Показать вес\n"
        << "5. Показать район\n"
        << "6. Показать статус назначения\n"
        << "[Изменение полей]\n"
        << "7. Изменить ID\n"
        << "8. Изменить адрес\n"
        << "9. Изменить вес\n"
        << "10. Изменить район\n"
        << "0. Назад в главное меню\n";
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

static void handle_manage_vehicle(DeliveryService& service)
{
    int id = read_int("Введите ID транспорта для управления: ");
    Vehicle* v = service.get_vehicle(id);

    if (!v)
    {
        cout << "Ошибка: транспорт с таким ID не найден!" << endl;
        return;
    }

    int choice = -1;
    while (choice != 0)
    {
        print_manage_vehicle_menu(v->get_id());
        choice = read_int("Выберите действие: ");

        switch (choice)
        {
        case 1: v->print_full_info(); break;
        case 2: cout << "ID транспорта: " << v->get_id() << endl; break;
        case 3: cout << "Тип транспорта: " << v->get_type() << endl; break;
        case 4: cout << "Грузоподъемность: " << v->get_capacity() << " кг" << endl; break;
        case 5: cout << "Имя курьера: " << v->get_courier_name() << endl; break;
        case 6: cout << "Статус доступности: " << (v->get_is_available() ? "Свободен" : "Недоступен / Занят") << endl; break;

        case 7:  handle_change_vehicle_id(service, v); break;
        case 8:  handle_change_vehicle_type(v); break;
        case 9:  handle_change_vehicle_capacity(v); break;
        case 10: handle_change_vehicle_courier(v); break;
        case 11: handle_change_vehicle_status(v); break;
        case 0:  break;
        default: cout << "Неверный пункт меню!" << endl;
        }
    }
}

static void handle_manage_order(DeliveryService& service)
{
    int id = read_int("Введите ID заказа для управления: ");
    Order* o = service.get_order(id);

    if (!o)
    {
        cout << "Ошибка: заказ с таким ID не найден!" << endl;
        return;
    }

    int choice = -1;
    while (choice != 0)
    {
        print_manage_order_menu(o->get_id());
        choice = read_int("Выберите действие: ");

        switch (choice)
        {
        case 1: o->print_full_info(); break;
        case 2: cout << "ID заказа: " << o->get_id() << endl; break;
        case 3: cout << "Адрес: " << o->get_address() << endl; break;
        case 4: cout << "Вес: " << o->get_weight() << " кг" << endl; break;
        case 5: cout << "Район: " << o->get_district() << endl; break;
        case 6: cout << "Статус: " << (o->get_is_assigned() ? "Доставляется" : "Ожидает назначения") << endl; break;

        case 7:  handle_change_order_id(service, o); break;
        case 8:  handle_change_order_address(o); break;
        case 9:  handle_change_order_weight(o); break;
        case 10: handle_change_order_district(o); break;
        case 0:  break;
        default: cout << "Неверный пункт меню!" << endl;
        }
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

static void handle_change_vehicle_id(DeliveryService& service, Vehicle* v)
{
    if (v->get_current_order_id() != -1)
    {
        cout << "Ошибка: нельзя менять ID транспорта во время доставки!" << endl;
        return;
    }

    int new_id = read_int("Введите новый ID: ");
    if (service.check_vehicle_exists(new_id) && new_id != v->get_id())
    {
        cout << "Ошибка: транспорт с таким ID уже существует!" << endl;
    }
    else
    {
        v->set_id(new_id);
        cout << "ID успешно изменен." << endl;
    }
}

static void handle_change_vehicle_type(Vehicle* v)
{
    if (v->get_current_order_id() != -1)
    {
        cout << "Ошибка: нельзя менять тип транспорта во время доставки!" << endl;
        return;
    }

    cout << "Введите новый тип (Мотоцикл/Машина): ";
    string type;
    getline(cin, type);
    v->set_type(type);
}

static void handle_change_vehicle_capacity(Vehicle* v)
{
    if (v->get_current_order_id() != -1)
    {
        cout << "Ошибка: нельзя менять грузоподъемность во время доставки!" << endl;
        return;
    }

    double cap = read_double("Введите новую грузоподъемность (кг): ");
    v->set_capacity(cap);
}

static void handle_change_vehicle_courier(Vehicle* v)
{
    cout << "Введите новое имя курьера: ";
    string name;
    getline(cin, name);
    v->set_courier_name(name);
}

static void handle_change_vehicle_status(Vehicle* v)
{
    if (v->get_current_order_id() != -1)
    {
        cout << "Ошибка: транспорт сейчас выполняет заказ! Изменение статуса заблокировано." << endl;
        return;
    }

    bool current_status = v->get_is_available();
    v->set_is_available(!current_status);
    cout << "Статус изменен. Теперь транспорт: " << (v->get_is_available() ? "Свободен" : "Заблокирован") << endl;
}

static void handle_change_order_id(DeliveryService& service, Order* o)
{
    if (o->get_is_assigned())
    {
        cout << "Ошибка: нельзя менять ID заказа, который уже доставляется!" << endl;
        return;
    }

    int new_id = read_int("Введите новый ID: ");
    if (service.check_order_exists(new_id) && new_id != o->get_id())
    {
        cout << "Ошибка: заказ с таким ID уже существует!" << endl;
    }
    else
    {
        o->set_id(new_id);
        cout << "ID успешно изменен." << endl;
    }
}

static void handle_change_order_address(Order* o)
{
    cout << "Введите новый адрес: ";
    string addr;
    getline(cin, addr);
    o->set_address(addr);
}

static void handle_change_order_weight(Order* o)
{
    if (o->get_is_assigned())
    {
        cout << "Ошибка: нельзя менять вес заказа, который уже назначен на транспорт!" << endl;
        return;
    }

    double w = read_double("Введите новый вес (кг): ");
    o->set_weight(w);
}

static void handle_change_order_district(Order* o)
{
    cout << "Введите новый район: ";
    string dist;
    getline(cin, dist);
    o->set_district(dist);
}