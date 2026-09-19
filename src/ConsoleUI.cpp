#include "ConsoleUI.h"
#include "InputUtils.h"
#include "DeliveryService.h"
#include "Motorcycle.h"
#include "Car.h"
#include "Truck.h"
#include <iostream>
#include <windows.h>
#include <string>
#include <memory>

using namespace std;

static void setup_console_encoding();
static void seed_data(DeliveryService& delivery_service);
static void print_menu();
static void print_manage_vehicle_menu(Vehicle* vehicle);
static void print_manage_order_menu(int order_id);

static void handle_add_vehicle(DeliveryService& delivery_service);
static void handle_add_order(DeliveryService& delivery_service);
static void handle_remove_order(DeliveryService& delivery_service);
static void handle_manage_vehicle(DeliveryService& delivery_service);
static void handle_specific_vehicle_action(Vehicle* vehicle);
static void handle_manage_order(DeliveryService& delivery_service);
static void handle_assign_order(DeliveryService& delivery_service);
static void handle_complete_delivery(DeliveryService& delivery_service);
static void handle_all_specific_actions(DeliveryService& delivery_service);
static void handle_delivery_costs(DeliveryService& delivery_service);

static void handle_compare_orders(DeliveryService& delivery_service);
static void handle_compare_vehicles(DeliveryService& delivery_service);

static void print_order_comparison(const Order* first_order, const Order* second_order, const string& op_symbol, bool result);
static void print_vehicle_comparison(const Vehicle* first_vehicle, const Vehicle* second_vehicle, const string& op_symbol, bool result);

static void handle_change_vehicle_id(DeliveryService& delivery_service, Vehicle* vehicle);
static void handle_change_vehicle_capacity(Vehicle* vehicle);
static void handle_change_vehicle_courier(Vehicle* vehicle);
static void handle_change_vehicle_status(Vehicle* vehicle);

static void handle_change_order_id(DeliveryService& delivery_service, Order* order);
static void handle_change_order_address(Order* order);
static void handle_change_order_weight(Order* order);
static void handle_change_order_district(Order* order);

void run_delivery_app()
{
    setup_console_encoding();

    DeliveryService delivery_service;
    seed_data(delivery_service);

    int menu_choice = -1;
    while (menu_choice != 0)
    {
        print_menu();
        menu_choice = read_int("Выберите действие: ");

        switch (menu_choice)
        {
        case 1: handle_add_vehicle(delivery_service); break;
        case 2: handle_add_order(delivery_service); break;
        case 3: handle_remove_order(delivery_service); break;
        case 4: handle_compare_orders(delivery_service); break;
        case 5: handle_compare_vehicles(delivery_service); break;
        case 6: handle_manage_vehicle(delivery_service); break;
        case 7: handle_manage_order(delivery_service); break;
        case 8: delivery_service.print_all_vehicles(); break;
        case 9: delivery_service.print_all_orders(); break;
        case 10: handle_assign_order(delivery_service); break;
        case 11: handle_complete_delivery(delivery_service); break;
        case 12: handle_all_specific_actions(delivery_service); break;
        case 13: handle_delivery_costs(delivery_service); break;
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

static void seed_data(DeliveryService& delivery_service)
{
    delivery_service += make_unique<Motorcycle>(1, 25.0, "Иван", true, true);
    delivery_service += make_unique<Car>(2, 450.0, "Алексей", true, 500.0);
    delivery_service += make_unique<Truck>(3, 5000.0, "Дмитрий", true, true);

    delivery_service += Order(101, "ул. Ленина, 5", 15.0, "Центральный");
    delivery_service += Order(102, "пр. Мира, 12", 250.0, "Северный");

    cout << "Тестовые данные успешно загружены!" << endl;
}

static void print_menu()
{
    cout << "\n--- МЕНЮ СЛУЖБЫ ДОСТАВКИ ---" << endl;
    cout << "1. Добавить транспорт вручную\n"
        << "2. Добавить заказ вручную\n"
        << "3. Удалить заказ по ID\n"
        << "4. Сравнение двух заказов (==, !=, <, >, <=, >=)\n"
        << "5. Сравнение двух транспортов (==, !=, <, >, <=, >=)\n"
        << "6. Управление транспортом (Просмотр и изменение)\n"
        << "7. Управление заказом (Просмотр и изменение)\n"
        << "8. Показать весь транспорт\n"
        << "9. Показать все заказы\n"
        << "10. Назначить заказ на транспорт\n"
        << "11. Завершить доставку по ID транспорта\n"
        << "12. Выполнить специфическое действие для всего транспорта (полиморфизм)\n"
        << "13. Рассчитать стоимость доставки для всего транспорта (полиморфизм)\n"
        << "0. Выход\n";
}

static void print_manage_vehicle_menu(Vehicle* vehicle)
{
    cout << "\n--- Управление транспортом ID " << vehicle->get_id() << " (" << vehicle->get_type() << ") ---" << endl;
    cout << "[Просмотр полей]\n"
        << "1. Показать полную информацию\n"
        << "2. Показать ID\n"
        << "3. Показать тип\n"
        << "4. Показать грузоподъемность\n"
        << "5. Показать имя курьера\n"
        << "6. Показать статус доступности\n"
        << "[Изменение полей]\n"
        << "7. Изменить ID\n"
        << "8. Изменить грузоподъемность\n"
        << "9. Изменить имя курьера\n"
        << "10. Изменить статус доступности\n";

    cout << "[Специфические действия]\n"
        << "11. " << vehicle->get_specific_action_name() << "\n";

    cout << "0. Назад в главное меню\n";
}

static void print_manage_order_menu(int order_id)
{
    cout << "\n--- Управление заказом ID " << order_id << " ---" << endl;
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

static void handle_add_vehicle(DeliveryService& delivery_service)
{
    cout << "\n--- Выберите тип создаваемого транспорта ---" << endl;
    cout << "1. Мотоцикл\n"
        << "2. Легковой автомобиль\n"
        << "3. Грузовик\n";

    int type_choice;
    while (true)
    {
        type_choice = read_int("Ваш выбор: ");
        if (type_choice >= 1 && type_choice <= 3)
        {
            break;
        }
        cout << "Ошибка: введите число от 1 до 3!" << endl;
    }

    unique_ptr<Vehicle> vehicle;
    switch (type_choice)
    {
    case 1:
    {
        auto motorcycle = make_unique<Motorcycle>();
        cin >> *motorcycle;
        vehicle = std::move(motorcycle);
        break;
    }
    case 2:
    {
        auto car = make_unique<Car>();
        cin >> *car;
        vehicle = std::move(car);
        break;
    }
    case 3:
    {
        auto truck = make_unique<Truck>();
        cin >> *truck;
        vehicle = std::move(truck);
        break;
    }
    }

    if (delivery_service.add_vehicle(std::move(vehicle)))
        cout << "Транспорт успешно добавлен." << endl;
}

static void handle_add_order(DeliveryService& delivery_service)
{
    Order order;
    cin >> order;
    delivery_service += order;
}

static void handle_remove_order(DeliveryService& delivery_service)
{
    int order_id = read_int("Введите ID заказа для удаления: ");
    delivery_service -= order_id;
}

static void print_order_comparison(const Order* first_order, const Order* second_order, const string& op_symbol, bool result)
{
    cout << "(первый заказ " << op_symbol << " второй заказ) => " << (result ? "true" : "false")
        << " (Вес номера " << first_order->get_id() << ": " << first_order->get_weight() << " кг "
        << op_symbol << " Вес номера " << second_order->get_id() << ": " << second_order->get_weight() << " кг)" << endl;
}

static void print_vehicle_comparison(const Vehicle* first_vehicle, const Vehicle* second_vehicle, const string& op_symbol, bool result)
{
    cout << "(первый транспорт " << op_symbol << " второй транспорт) => " << (result ? "true" : "false")
        << " (Грузоподъемность номера " << first_vehicle->get_id() << ": " << first_vehicle->get_capacity() << " кг "
        << op_symbol << " номера " << second_vehicle->get_id() << ": " << second_vehicle->get_capacity() << " кг)" << endl;
}

static void handle_compare_orders(DeliveryService& delivery_service)
{
    delivery_service.print_all_orders();

    int first_order_id = read_int("\nВведите ID первого заказа: ");
    Order* first_order = delivery_service.get_order(first_order_id);
    if (!first_order)
    {
        cout << "Ошибка: заказ с ID " << first_order_id << " не найден!" << endl;
        return;
    }

    int second_order_id = read_int("Введите ID второго заказа: ");
    Order* second_order = delivery_service.get_order(second_order_id);
    if (!second_order)
    {
        cout << "Ошибка: заказ с ID " << second_order_id << " не найден!" << endl;
        return;
    }

    cout << "\n--- Выберите оператор сравнения для заказов ---" << endl;
    cout << "1. == (Равенство по ID)\n"
        << "2. != (Неравенство по ID)\n"
        << "3. <  (Меньше по весу)\n"
        << "4. >  (Больше по весу)\n"
        << "5. <= (Меньше или равно по весу)\n"
        << "6. >= (Больше или равно по весу)\n";

    int operation_choice = read_int("Выберите операцию: ");

    switch (operation_choice)
    {
    case 1:
        cout << "(первый заказ == второй заказ) => " << ((*first_order == *second_order) ? "true (ID одинаковые)" : "false (ID разные)") << endl;
        break;
    case 2:
        cout << "(первый заказ != второй заказ) => " << ((*first_order != *second_order) ? "true (ID разные)" : "false (ID одинаковые)") << endl;
        break;
    case 3:
        print_order_comparison(first_order, second_order, "<", *first_order < *second_order);
        break;
    case 4:
        print_order_comparison(first_order, second_order, ">", *first_order > *second_order);
        break;
    case 5:
        print_order_comparison(first_order, second_order, "<=", *first_order <= *second_order);
        break;
    case 6:
        print_order_comparison(first_order, second_order, ">=", *first_order >= *second_order);
        break;
    default:
        cout << "Неверный пункт!" << endl;
    }
}

static void handle_compare_vehicles(DeliveryService& delivery_service)
{
    delivery_service.print_all_vehicles();

    int first_vehicle_id = read_int("\nВведите ID первого транспорта: ");
    Vehicle* first_vehicle = delivery_service.get_vehicle(first_vehicle_id);
    if (!first_vehicle)
    {
        cout << "Ошибка: транспорт с ID " << first_vehicle_id << " не найден!" << endl;
        return;
    }

    int second_vehicle_id = read_int("Введите ID второго транспорта: ");
    Vehicle* second_vehicle = delivery_service.get_vehicle(second_vehicle_id);
    if (!second_vehicle)
    {
        cout << "Ошибка: транспорт с ID " << second_vehicle_id << " не найден!" << endl;
        return;
    }

    cout << "\n--- Выберите оператор сравнения для транспорта ---" << endl;
    cout << "1. == (Равенство по ID)\n"
        << "2. != (Неравенство по ID)\n"
        << "3. <  (Меньше по грузоподъемности)\n"
        << "4. >  (Больше по грузоподъемности)\n"
        << "5. <= (Меньше или равно по грузоподъемности)\n"
        << "6. >= (Больше или равно по грузоподъемности)\n";

    int operation_choice = read_int("Выберите операцию: ");

    switch (operation_choice)
    {
    case 1:
        cout << "(первый транспорт == второй транспорт) => " << ((*first_vehicle == *second_vehicle) ? "true (ID одинаковые)" : "false (ID разные)") << endl;
        break;
    case 2:
        cout << "(первый транспорт != второй транспорт) => " << ((*first_vehicle != *second_vehicle) ? "true (ID разные)" : "false (ID одинаковые)") << endl;
        break;
    case 3:
        print_vehicle_comparison(first_vehicle, second_vehicle, "<", *first_vehicle < *second_vehicle);
        break;
    case 4:
        print_vehicle_comparison(first_vehicle, second_vehicle, ">", *first_vehicle > *second_vehicle);
        break;
    case 5:
        print_vehicle_comparison(first_vehicle, second_vehicle, "<=", *first_vehicle <= *second_vehicle);
        break;
    case 6:
        print_vehicle_comparison(first_vehicle, second_vehicle, ">=", *first_vehicle >= *second_vehicle);
        break;
    default:
        cout << "Неверный пункт!" << endl;
    }
}

static void handle_manage_vehicle(DeliveryService& delivery_service)
{
    int vehicle_id = read_int("Введите ID транспорта для управления: ");
    Vehicle* vehicle = delivery_service.get_vehicle(vehicle_id);

    if (!vehicle)
    {
        cout << "Ошибка: транспорт с таким ID не найден!" << endl;
        return;
    }

    int menu_choice = -1;
    while (menu_choice != 0)
    {
        print_manage_vehicle_menu(vehicle);
        menu_choice = read_int("Выберите действие: ");

        switch (menu_choice)
        {
        case 1: cout << *vehicle << endl; break;
        case 2: cout << "ID транспорта: " << vehicle->get_id() << endl; break;
        case 3: cout << "Тип транспорта: " << vehicle->get_type() << endl; break;
        case 4: cout << "Грузоподъемность: " << vehicle->get_capacity() << " кг" << endl; break;
        case 5: cout << "Имя курьера: " << vehicle->get_courier_name() << endl; break;
        case 6: cout << "Статус доступности: " << (vehicle->get_is_available() ? "Свободен" : "Недоступен / Занят") << endl; break;

        case 7:  handle_change_vehicle_id(delivery_service, vehicle); break;
        case 8:  handle_change_vehicle_capacity(vehicle); break;
        case 9:  handle_change_vehicle_courier(vehicle); break;
        case 10: handle_change_vehicle_status(vehicle); break;
        case 11: handle_specific_vehicle_action(vehicle); break;
        case 0:  break;
        default: cout << "Неверный пункт меню!" << endl;
        }
    }
}

static void handle_specific_vehicle_action(Vehicle* vehicle)
{
    vehicle->perform_specific_action();
}

static void handle_manage_order(DeliveryService& delivery_service)
{
    int order_id = read_int("Введите ID заказа для управления: ");
    Order* order = delivery_service.get_order(order_id);

    if (!order)
    {
        cout << "Ошибка: заказ с таким ID не найден!" << endl;
        return;
    }

    int menu_choice = -1;
    while (menu_choice != 0)
    {
        print_manage_order_menu(order->get_id());
        menu_choice = read_int("Выберите действие: ");

        switch (menu_choice)
        {
        case 1: cout << *order << endl; break;
        case 2: cout << "ID заказа: " << order->get_id() << endl; break;
        case 3: cout << "Адрес: " << order->get_address() << endl; break;
        case 4: cout << "Вес: " << order->get_weight() << " кг" << endl; break;
        case 5: cout << "Район: " << order->get_district() << endl; break;
        case 6: cout << "Статус: " << (order->get_is_assigned() ? "Доставляется" : "Ожидает назначения") << endl; break;

        case 7:  handle_change_order_id(delivery_service, order); break;
        case 8:  handle_change_order_address(order); break;
        case 9:  handle_change_order_weight(order); break;
        case 10: handle_change_order_district(order); break;
        case 0:  break;
        default: cout << "Неверный пункт меню!" << endl;
        }
    }
}

static void handle_assign_order(DeliveryService& delivery_service)
{
    int order_id = read_int("Введите ID заказа: ");
    delivery_service.assign_order_to_vehicle(order_id);
}

static void handle_complete_delivery(DeliveryService& delivery_service)
{
    int vehicle_id = read_int("Введите ID транспорта, завершившего доставку: ");
    delivery_service.complete_delivery(vehicle_id);
}

static void handle_all_specific_actions(DeliveryService& delivery_service)
{
    delivery_service.perform_all_specific_actions();
}

static void handle_delivery_costs(DeliveryService& delivery_service)
{
    double weight = read_double("Введите вес груза (кг): ");
    if (weight <= 0)
    {
        cout << "Ошибка: вес должен быть положительным!" << endl;
        return;
    }
    delivery_service.print_delivery_costs(weight);
}

static void handle_change_vehicle_id(DeliveryService& delivery_service, Vehicle* vehicle)
{
    if (vehicle->get_current_order_id() != -1)
    {
        cout << "Ошибка: нельзя менять ID транспорта во время доставки!" << endl;
        return;
    }

    int new_vehicle_id = read_int("Введите новый ID: ");
    if (delivery_service.check_vehicle_exists(new_vehicle_id) && new_vehicle_id != vehicle->get_id())
    {
        cout << "Ошибка: транспорт с таким ID уже существует!" << endl;
    }
    else
    {
        vehicle->set_id(new_vehicle_id);
        cout << "ID успешно изменен." << endl;
    }
}

static void handle_change_vehicle_capacity(Vehicle* vehicle)
{
    if (vehicle->get_current_order_id() != -1)
    {
        cout << "Ошибка: нельзя менять грузоподъемность во время доставки!" << endl;
        return;
    }

    double new_capacity = read_double("Введите новую грузоподъемность (кг): ");
    vehicle->set_capacity(new_capacity);
}

static void handle_change_vehicle_courier(Vehicle* vehicle)
{
    cout << "Введите новое имя курьера: ";
    string courier_name;
    getline(cin, courier_name);
    vehicle->set_courier_name(courier_name);
}

static void handle_change_vehicle_status(Vehicle* vehicle)
{
    if (vehicle->get_current_order_id() != -1)
    {
        cout << "Ошибка: транспорт сейчас выполняет заказ! Изменение статуса заблокировано." << endl;
        return;
    }

    bool current_availability_status = vehicle->get_is_available();
    vehicle->set_is_available(!current_availability_status);
    cout << "Статус изменен. Теперь транспорт: " << (vehicle->get_is_available() ? "Свободен" : "Заблокирован") << endl;
}

static void handle_change_order_id(DeliveryService& delivery_service, Order* order)
{
    if (order->get_is_assigned())
    {
        cout << "Ошибка: нельзя менять ID заказа, который уже доставляется!" << endl;
        return;
    }

    int new_order_id = read_int("Введите новый ID: ");
    if (delivery_service.check_order_exists(new_order_id) && new_order_id != order->get_id())
    {
        cout << "Ошибка: заказ с таким ID уже существует!" << endl;
    }
    else
    {
        order->set_id(new_order_id);
        cout << "ID успешно изменен." << endl;
    }
}

static void handle_change_order_address(Order* order)
{
    cout << "Введите новый адрес: ";
    string new_address;
    getline(cin, new_address);
    order->set_address(new_address);
}

static void handle_change_order_weight(Order* order)
{
    if (order->get_is_assigned())
    {
        cout << "Ошибка: нельзя менять вес заказа, который уже назначен на транспорт!" << endl;
        return;
    }

    double new_weight = read_double("Введите новый вес (кг): ");
    order->set_weight(new_weight);
}

static void handle_change_order_district(Order* order)
{
    cout << "Введите новый район: ";
    string new_district;
    getline(cin, new_district);
    order->set_district(new_district);
}