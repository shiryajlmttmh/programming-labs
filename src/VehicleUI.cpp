#include "VehicleUI.h"
#include "UICommon.h"
#include "InputUtils.h"
#include "DeliveryService.h"
#include "Motorcycle.h"
#include "Car.h"
#include "Truck.h"

#include <iostream>
#include <memory>
#include <string>
#include <format>

using namespace std;

static void print_manage_vehicle_menu(Vehicle* vehicle);
static void print_vehicle_comparison(const Vehicle* first_vehicle, const Vehicle* second_vehicle, const string& op_symbol, bool result);

static void handle_specific_vehicle_action(Vehicle* vehicle);
static void handle_vehicle_delivery_cost(Vehicle* vehicle);

static void handle_change_vehicle_id(DeliveryService& delivery_service, Vehicle* vehicle);
static void handle_change_vehicle_capacity(Vehicle* vehicle);
static void handle_change_vehicle_courier(Vehicle* vehicle);
static void handle_change_vehicle_status(Vehicle* vehicle);

void handle_add_vehicle(DeliveryService& delivery_service)
{
    cout << "\n--- Выберите тип создаваемого транспорта ---" << endl;
    cout << "1. Мотоцикл\n"
        << "2. Легковой автомобиль\n"
        << "3. Грузовик\n";

    int type_choice;
    while (true)
    {
        type_choice = read_int("Ваш выбор: ");
        if (type_choice >= 1 && type_choice <= 3) break;
        cout << "Ошибка: введите число от 1 до 3!" << endl;
    }

    unique_ptr<Vehicle> vehicle;
    switch (type_choice)
    {
    case 1:
    {
        unique_ptr<Motorcycle> motorcycle = make_unique<Motorcycle>();
        cin >> *motorcycle;
        vehicle = move(motorcycle);
        break;
    }
    case 2:
    {
        unique_ptr<Car> car = make_unique<Car>();
        cin >> *car;
        vehicle = move(car);
        break;
    }
    case 3:
    {
        unique_ptr<Truck> truck = make_unique<Truck>();
        cin >> *truck;
        vehicle = move(truck);
        break;
    }
    }

    string vehicle_info = vehicle->get_full_info();
    delivery_service.add_vehicle(move(vehicle));
    cout << "Транспорт успешно добавлен." << endl;
    logger.log("ADD_VEHICLE", vehicle_info);
}

void handle_manage_vehicle(DeliveryService& delivery_service)
{
    int vehicle_id = read_int("Введите ID транспорта для управления: ");
    Vehicle* vehicle = delivery_service.get_vehicle(vehicle_id);

    int menu_choice = -1;
    while (menu_choice != 0)
    {
        print_manage_vehicle_menu(vehicle);
        menu_choice = read_int("Выберите действие: ");

        try
        {
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
            case 12: handle_vehicle_delivery_cost(vehicle); break;
            case 0:  break;
            default: cout << "Неверный пункт меню!" << endl;
            }
        }
        catch (...)
        {
            report_exception(current_exception());
        }
    }
}

void handle_compare_vehicles(DeliveryService& delivery_service)
{
    delivery_service.print_all_vehicles();

    int first_vehicle_id = read_int("\nВведите ID первого транспорта: ");
    Vehicle* first_vehicle = delivery_service.get_vehicle(first_vehicle_id);

    int second_vehicle_id = read_int("Введите ID второго транспорта: ");
    Vehicle* second_vehicle = delivery_service.get_vehicle(second_vehicle_id);

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

    cout << "[Расчёты]\n"
        << "12. Рассчитать стоимость доставки\n";

    cout << "0. Назад в главное меню\n";
}

static void print_vehicle_comparison(const Vehicle* first_vehicle, const Vehicle* second_vehicle, const string& op_symbol, bool result)
{
    cout << "(первый транспорт " << op_symbol << " второй транспорт) => " << (result ? "true" : "false")
        << " (Грузоподъемность номера " << first_vehicle->get_id() << ": " << first_vehicle->get_capacity() << " кг "
        << op_symbol << " номера " << second_vehicle->get_id() << ": " << second_vehicle->get_capacity() << " кг)" << endl;
}

static void handle_specific_vehicle_action(Vehicle* vehicle)
{
    vehicle->perform_specific_action();
    logger.log("SPECIFIC_ACTION", "Транспорт номер " + to_string(vehicle->get_id())
        + ": " + vehicle->get_specific_action_name());
}

static void handle_vehicle_delivery_cost(Vehicle* vehicle)
{
    double weight = read_double("Введите вес груза (кг): ");
    double cost = vehicle->calculate_delivery_cost(weight);

    cout << "Стоимость доставки груза весом " << weight << " кг транспортом номер " << vehicle->get_id()
        << " (" << vehicle->get_type() << "): "
        << format("{:.2f}", cost) << " руб." << endl;
}

static void handle_change_vehicle_id(DeliveryService& delivery_service, Vehicle* vehicle)
{
    int old_vehicle_id = vehicle->get_id();
    int new_vehicle_id = read_int("Введите новый ID: ");
    delivery_service.change_vehicle_id(old_vehicle_id, new_vehicle_id);
    cout << "ID успешно изменен." << endl;
    logger.log("CHANGE_VEHICLE_ID", "ID транспорта изменён с " + to_string(old_vehicle_id) + " на " + to_string(new_vehicle_id));
}

static void handle_change_vehicle_capacity(Vehicle* vehicle)
{
    double new_capacity = read_double("Введите новую грузоподъемность (кг): ");
    vehicle->set_capacity(new_capacity);
    cout << "Грузоподъемность успешно изменена." << endl;
    logger.log("CHANGE_VEHICLE_CAPACITY", "Транспорт номер " + to_string(vehicle->get_id())
        + ": грузоподъемность изменена на " + format("{:.1f}", new_capacity) + " кг");
}

static void handle_change_vehicle_courier(Vehicle* vehicle)
{
    cout << "Введите новое имя курьера: ";
    string courier_name;
    getline(cin, courier_name);
    vehicle->set_courier_name(courier_name);
    logger.log("CHANGE_VEHICLE_COURIER", "Транспорт номер " + to_string(vehicle->get_id())
        + ": курьер изменён на \"" + courier_name + "\"");
}

static void handle_change_vehicle_status(Vehicle* vehicle)
{
    bool current_availability_status = vehicle->get_is_available();
    vehicle->set_is_available(!current_availability_status);
    cout << "Статус изменен. Теперь транспорт: " << (vehicle->get_is_available() ? "Свободен" : "Заблокирован") << endl;
    logger.log("CHANGE_VEHICLE_STATUS", "Транспорт номер " + to_string(vehicle->get_id())
        + ": теперь " + (vehicle->get_is_available() ? "свободен" : "заблокирован"));
}