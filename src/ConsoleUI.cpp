#include "ConsoleUI.h"
#include "InputUtils.h"
#include "DeliveryService.h"
#include "DataStorage.h"
#include "Logger.h"
#include "ReportWriter.h"
#include "Motorcycle.h"
#include "Car.h"
#include "Truck.h"
#include "CollectionDemo.h"
#include "DeliveryAnalytics.h"
#include "Exceptions.h"

#include <iostream>
#include <windows.h>
#include <string>
#include <memory>
#include <format>
#include <stdexcept>
#include <exception>
#include <filesystem>
#include <algorithm>
#include <vector>
#include <map>
#include <set>

using namespace std;

static constexpr const char* DATA_FILE_NAME = "delivery_state.txt";
static constexpr const char* LOG_FILE_NAME = "delivery.log";
static constexpr const char* REPORT_FILE_NAME = "delivery_report.txt";

static Logger logger(LOG_FILE_NAME);

static void setup_console_encoding();
static void report_exception(exception_ptr ep);
static void log_exception(exception_ptr ep);
static void seed_data(DeliveryService& delivery_service);
static void print_menu();
static void print_manage_vehicle_menu(Vehicle* vehicle);
static void print_manage_order_menu(int order_id);

static void handle_add_vehicle(DeliveryService& delivery_service);
static void handle_add_order(DeliveryService& delivery_service);
static void handle_remove_order(DeliveryService& delivery_service);
static void handle_manage_vehicle(DeliveryService& delivery_service);
static void handle_specific_vehicle_action(Vehicle* vehicle);
static void handle_vehicle_delivery_cost(Vehicle* vehicle);
static void handle_manage_order(DeliveryService& delivery_service);
static void handle_assign_order(DeliveryService& delivery_service);
static void handle_complete_delivery(DeliveryService& delivery_service);
static void handle_all_specific_actions(DeliveryService& delivery_service);
static void handle_delivery_costs(DeliveryService& delivery_service);
static void handle_save_state(const DeliveryService& delivery_service);
static void handle_load_state(DeliveryService& delivery_service);
static void handle_create_report(const DeliveryService& delivery_service);
static bool initialize_state(DeliveryService& delivery_service);
static void auto_save_state(const DeliveryService& delivery_service);

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

static void print_analytics_menu();
static int read_choice(const string& prompt, int min_value, int max_value);
static void print_order_list(const vector<const Order*>& orders);
static void print_vehicle_list(const vector<const Vehicle*>& vehicles);
static string choose_vehicle_type();
static void handle_analytics_menu(const DeliveryService& delivery_service);
static void handle_analytics_find_orders(const DeliveryAnalytics& analytics);
static void handle_analytics_find_vehicles(const DeliveryAnalytics& analytics);
static void handle_analytics_sort_orders(const DeliveryAnalytics& analytics);
static void handle_analytics_sort_vehicles(const DeliveryAnalytics& analytics);
static void handle_analytics_order_extremes(const DeliveryAnalytics& analytics);
static void handle_analytics_vehicle_extremes(const DeliveryAnalytics& analytics);
static void handle_analytics_count_waiting_orders(const DeliveryAnalytics& analytics);
static void handle_analytics_count_heavy_orders(const DeliveryAnalytics& analytics);
static void handle_analytics_count_available_vehicles(const DeliveryAnalytics& analytics);
static void handle_analytics_group_orders(const DeliveryAnalytics& analytics);
static void handle_analytics_group_vehicles(const DeliveryAnalytics& analytics);
static void handle_analytics_districts(const DeliveryAnalytics& analytics);
static void handle_analytics_order_weight_stats(const DeliveryAnalytics& analytics);
static void handle_analytics_weight_by_district(const DeliveryAnalytics& analytics);
static void handle_analytics_vehicle_capacity_stats(const DeliveryAnalytics& analytics);
static void handle_analytics_load_ratio(const DeliveryAnalytics& analytics);

void run_delivery_app()
{
    setup_console_encoding();
    logger.log("START", "Программа запущена");

    DeliveryService delivery_service;
    bool auto_save_enabled = initialize_state(delivery_service);

    int menu_choice = -1;
    while (menu_choice != 0)
    {
        print_menu();
        menu_choice = read_int("Выберите действие: ");

        try
        {
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
            case 14: run_collection_demo_menu(); break;
            case 15: handle_save_state(delivery_service); break;
            case 16: handle_load_state(delivery_service); break;
            case 17: handle_create_report(delivery_service); break;
            case 18: handle_analytics_menu(delivery_service); break;
            case 0: cout << "Завершение работы." << endl; break;
            default: cout << "Неверный пункт меню!" << endl;
            }
        }
        catch (...)
        {
            report_exception(current_exception());
        }
    }

    if (auto_save_enabled) auto_save_state(delivery_service);
    logger.log("EXIT", "Программа завершена");
}

static void report_exception(exception_ptr ep)
{
    log_exception(ep);

    try
    {
        if (ep) rethrow_exception(ep);
    }
    catch (const NotFoundException& e)
    {
        cout << "Ошибка поиска: " << e.what() << ". Проверьте номер (" << e.get_id() << ")." << endl;
    }
    catch (const DuplicateIdException& e)
    {
        cout << "Ошибка уникальности: " << e.what() << ". Выберите другой номер." << endl;
    }
    catch (const InvalidDataException& e)
    {
        cout << "Некорректные данные: " << e.what() << "." << endl;
    }
    catch (const ConstraintViolationException& e)
    {
        cout << "Нарушено ограничение: " << e.what() << "." << endl;
    }
    catch (const InvalidOperationException& e)
    {
        cout << "Недопустимая операция: " << e.what() << "." << endl;
    }
    catch (const FileOpenException& e)
    {
        cout << "Ошибка файла: " << e.what() << ". Проверьте путь и права доступа." << endl;
    }
    catch (const FileIOException& e)
    {
        cout << "Сбой при работе с файлом: " << e.what() << "." << endl;
    }
    catch (const FileFormatException& e)
    {
        cout << "Повреждённый файл данных: " << e.what() << "." << endl;
    }
    catch (const DeliveryException& e)
    {
        cout << "Ошибка службы доставки: " << e.what() << "." << endl;
    }
    catch (const out_of_range& e)
    {
        cout << "Выход за пределы допустимого диапазона: " << e.what() << "." << endl;
    }
    catch (const exception& e)
    {
        cout << "Непредвиденная ошибка: " << e.what() << "." << endl;
    }
    catch (...)
    {
        cout << "Неизвестная ошибка." << endl;
    }
}

static void log_exception(exception_ptr ep)
{
    try
    {
        if (ep) rethrow_exception(ep);
    }
    catch (const exception& e)
    {
        logger.log("ERROR", e.what());
    }
    catch (...)
    {
        logger.log("ERROR", "Неизвестная ошибка");
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
        << "12. Выполнить специфическое действие для всего транспорта\n"
        << "13. Рассчитать стоимость доставки для всего транспорта\n"
        << "14. Демонстрация шаблонного контейнера (Collection<T>)\n"
        << "15. Сохранить состояние в файл\n"
        << "16. Загрузить состояние из файла\n"
        << "17. Сформировать текстовый отчёт\n"
        << "18. Аналитика (поиск, сортировка, группировка, статистика)\n"
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

    cout << "[Расчёты]\n"
        << "12. Рассчитать стоимость доставки\n";

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

static void handle_add_order(DeliveryService& delivery_service)
{
    Order order;
    cin >> order;
    delivery_service += order;
    logger.log("ADD_ORDER", order.get_full_info());
}

static void handle_remove_order(DeliveryService& delivery_service)
{
    int order_id = read_int("Введите ID заказа для удаления: ");
    delivery_service -= order_id;
    logger.log("REMOVE_ORDER", "Заказ номер " + to_string(order_id) + " удалён");
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

    int second_order_id = read_int("Введите ID второго заказа: ");
    Order* second_order = delivery_service.get_order(second_order_id);

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

static void handle_manage_vehicle(DeliveryService& delivery_service)
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

static void handle_manage_order(DeliveryService& delivery_service)
{
    int order_id = read_int("Введите ID заказа для управления: ");
    Order* order = delivery_service.get_order(order_id);

    int menu_choice = -1;
    while (menu_choice != 0)
    {
        print_manage_order_menu(order->get_id());
        menu_choice = read_int("Выберите действие: ");

        try
        {
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
        catch (...)
        {
            report_exception(current_exception());
        }
    }
}

static void handle_assign_order(DeliveryService& delivery_service)
{
    int order_id = read_int("Введите ID заказа: ");
    delivery_service.assign_order_to_vehicle(order_id);

    for (size_t i = 0; i < delivery_service.get_vehicles_count(); i++)
    {
        const Vehicle& vehicle = delivery_service.get_vehicle_by_index(i);
        if (vehicle.get_current_order_id() == order_id)
            logger.log("ASSIGN_ORDER", "Заказ номер " + to_string(order_id)
                + " назначен на транспорт номер " + to_string(vehicle.get_id()));
    }
}

static void handle_complete_delivery(DeliveryService& delivery_service)
{
    int vehicle_id = read_int("Введите ID транспорта, завершившего доставку: ");
    int order_id = delivery_service.get_vehicle(vehicle_id)->get_current_order_id();
    delivery_service.complete_delivery(vehicle_id);
    logger.log("COMPLETE_DELIVERY", "Транспорт номер " + to_string(vehicle_id)
        + " завершил доставку заказа номер " + to_string(order_id));
}

static void handle_all_specific_actions(DeliveryService& delivery_service)
{
    delivery_service.perform_all_specific_actions();
    logger.log("SPECIFIC_ACTION", "Выполнены специфические действия всего транспорта");
}

static void handle_delivery_costs(DeliveryService& delivery_service)
{
    double weight = read_double("Введите вес груза (кг): ");
    delivery_service.print_delivery_costs(weight);
}

static void handle_save_state(const DeliveryService& delivery_service)
{
    DataStorage storage(DATA_FILE_NAME);
    storage.save(delivery_service);
    cout << "Состояние сохранено в файл " << DATA_FILE_NAME << "." << endl;
    logger.log("SAVE", string("Состояние сохранено в файл ") + DATA_FILE_NAME);
}

static void handle_load_state(DeliveryService& delivery_service)
{
    DataStorage storage(DATA_FILE_NAME);
    delivery_service = storage.load();
    cout << "Состояние загружено из файла " << DATA_FILE_NAME << "." << endl;
    logger.log("LOAD", string("Состояние загружено из файла ") + DATA_FILE_NAME);
}

static void handle_create_report(const DeliveryService& delivery_service)
{
    ReportWriter report_writer(REPORT_FILE_NAME);
    report_writer.write(delivery_service);
    cout << "Отчёт сформирован в файле " << REPORT_FILE_NAME << "." << endl;
    logger.log("REPORT", string("Сформирован отчёт ") + REPORT_FILE_NAME);
}

static bool initialize_state(DeliveryService& delivery_service)
{
    error_code error;
    if (!filesystem::exists(DATA_FILE_NAME, error))
    {
        try
        {
            seed_data(delivery_service);
            logger.log("SEED", "Файла состояния нет, загружены тестовые данные");
        }
        catch (...)
        {
            cout << "Не удалось полностью загрузить тестовые данные." << endl;
            report_exception(current_exception());
        }
        return true;
    }

    try
    {
        DataStorage storage(DATA_FILE_NAME);
        delivery_service = storage.load();
        cout << "Состояние загружено из файла " << DATA_FILE_NAME << "." << endl;
        logger.log("AUTOLOAD", string("Состояние загружено из файла ") + DATA_FILE_NAME + " при запуске");
        return true;
    }
    catch (...)
    {
        cout << "Не удалось загрузить сохранённое состояние." << endl;
        report_exception(current_exception());
        cout << "Программа запущена с пустым состоянием. Автосохранение при выходе отключено, чтобы не затереть файл "
            << DATA_FILE_NAME << ". Для ручного сохранения используйте пункт 15." << endl;
        logger.log("AUTOSAVE_DISABLED", "Файл состояния не удалось загрузить, автосохранение при выходе отключено");
        return false;
    }
}

static void auto_save_state(const DeliveryService& delivery_service)
{
    try
    {
        DataStorage storage(DATA_FILE_NAME);
        storage.save(delivery_service);
        cout << "Состояние автоматически сохранено в файл " << DATA_FILE_NAME << "." << endl;
        logger.log("AUTOSAVE", string("Состояние сохранено в файл ") + DATA_FILE_NAME + " при выходе");
    }
    catch (...)
    {
        cout << "Не удалось сохранить состояние при выходе." << endl;
        report_exception(current_exception());
    }
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

static void handle_change_order_id(DeliveryService& delivery_service, Order* order)
{
    int old_order_id = order->get_id();
    int new_order_id = read_int("Введите новый ID: ");
    delivery_service.change_order_id(old_order_id, new_order_id);
    cout << "ID успешно изменен." << endl;
    logger.log("CHANGE_ORDER_ID", "ID заказа изменён с " + to_string(old_order_id) + " на " + to_string(new_order_id));
}

static void handle_change_order_address(Order* order)
{
    cout << "Введите новый адрес: ";
    string new_address;
    getline(cin, new_address);
    order->set_address(new_address);
    logger.log("CHANGE_ORDER_ADDRESS", "Заказ номер " + to_string(order->get_id())
        + ": адрес изменён на \"" + new_address + "\"");
}

static void handle_change_order_weight(Order* order)
{
    double new_weight = read_double("Введите новый вес (кг): ");
    order->set_weight(new_weight);
    cout << "Вес успешно изменен." << endl;
    logger.log("CHANGE_ORDER_WEIGHT", "Заказ номер " + to_string(order->get_id())
        + ": вес изменён на " + format("{:.1f}", new_weight) + " кг");
}

static void handle_change_order_district(Order* order)
{
    cout << "Введите новый район: ";
    string new_district;
    getline(cin, new_district);
    order->set_district(new_district);
    logger.log("CHANGE_ORDER_DISTRICT", "Заказ номер " + to_string(order->get_id())
        + ": район изменён на \"" + new_district + "\"");
}

static void print_analytics_menu()
{
    cout << "\n--- АНАЛИТИКА ---" << endl;
    cout << "[Поиск]\n"
        << "1. Найти заказы по критериям (район, вес, статус)\n"
        << "2. Найти транспорт по критериям (тип, грузоподъемность, доступность)\n"
        << "[Сортировка]\n"
        << "3. Отсортировать заказы\n"
        << "4. Отсортировать транспорт\n"
        << "[Минимум и максимум]\n"
        << "5. Самый лёгкий и самый тяжёлый заказ\n"
        << "6. Транспорт с наименьшей и наибольшей грузоподъемностью\n"
        << "[Подсчёт]\n"
        << "7. Количество заказов, ожидающих назначения\n"
        << "8. Количество заказов тяжелее заданного веса\n"
        << "9. Количество свободного транспорта\n"
        << "[Группировка]\n"
        << "10. Заказы по районам\n"
        << "11. Транспорт по типам\n"
        << "12. Список районов\n"
        << "[Статистика]\n"
        << "13. Вес заказов (суммарный и средний)\n"
        << "14. Вес заказов по районам\n"
        << "15. Грузоподъемность транспорта (общая и свободная)\n"
        << "16. Загрузка свободного транспорта ожидающими заказами\n"
        << "0. Назад в главное меню\n";
}

static int read_choice(const string& prompt, int min_value, int max_value)
{
    while (true)
    {
        int choice = read_int(prompt);
        if (choice >= min_value && choice <= max_value) return choice;
        cout << "Ошибка: введите число от " << min_value << " до " << max_value << "!" << endl;
    }
}

static void print_order_list(const vector<const Order*>& orders)
{
    if (orders.empty())
    {
        cout << "Ничего не найдено." << endl;
        return;
    }

    cout << "Заказов: " << orders.size() << endl;
    for_each(orders.begin(), orders.end(), [](const Order* order) { cout << *order << endl; });
}

static void print_vehicle_list(const vector<const Vehicle*>& vehicles)
{
    if (vehicles.empty())
    {
        cout << "Ничего не найдено." << endl;
        return;
    }

    cout << "Транспорта: " << vehicles.size() << endl;
    for_each(vehicles.begin(), vehicles.end(), [](const Vehicle* vehicle) { cout << *vehicle << endl; });
}

static string choose_vehicle_type()
{
    const string type_names[] = { Motorcycle().get_type(), Car().get_type(), Truck().get_type() };

    cout << "Тип транспорта:\n0. Любой\n";
    for (int i = 0; i < 3; i++) cout << (i + 1) << ". " << type_names[i] << "\n";

    int choice = read_choice("Выберите тип: ", 0, 3);
    return choice == 0 ? string() : type_names[choice - 1];
}

static void handle_analytics_menu(const DeliveryService& delivery_service)
{
    DeliveryAnalytics analytics(delivery_service);

    int menu_choice = -1;
    while (menu_choice != 0)
    {
        print_analytics_menu();
        menu_choice = read_int("Выберите действие: ");

        try
        {
            switch (menu_choice)
            {
            case 1:  handle_analytics_find_orders(analytics); break;
            case 2:  handle_analytics_find_vehicles(analytics); break;
            case 3:  handle_analytics_sort_orders(analytics); break;
            case 4:  handle_analytics_sort_vehicles(analytics); break;
            case 5:  handle_analytics_order_extremes(analytics); break;
            case 6:  handle_analytics_vehicle_extremes(analytics); break;
            case 7:  handle_analytics_count_waiting_orders(analytics); break;
            case 8:  handle_analytics_count_heavy_orders(analytics); break;
            case 9:  handle_analytics_count_available_vehicles(analytics); break;
            case 10: handle_analytics_group_orders(analytics); break;
            case 11: handle_analytics_group_vehicles(analytics); break;
            case 12: handle_analytics_districts(analytics); break;
            case 13: handle_analytics_order_weight_stats(analytics); break;
            case 14: handle_analytics_weight_by_district(analytics); break;
            case 15: handle_analytics_vehicle_capacity_stats(analytics); break;
            case 16: handle_analytics_load_ratio(analytics); break;
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

static void handle_analytics_find_orders(const DeliveryAnalytics& analytics)
{
    OrderFilter filter;

    cout << "Район (Enter - любой): ";
    getline(cin, filter.district);

    filter.min_weight = read_double("Минимальный вес, кг (0 - без ограничения): ");

    double max_weight = read_double("Максимальный вес, кг (0 - без ограничения): ");
    if (max_weight != 0) filter.max_weight = max_weight;

    int status = read_choice("Статус (1 - ожидают назначения, 2 - доставляются, 0 - любой): ", 0, 2);
    if (status != 0) filter.is_assigned = (status == 2);

    print_order_list(analytics.find_orders(filter));
}

static void handle_analytics_find_vehicles(const DeliveryAnalytics& analytics)
{
    VehicleFilter filter;

    filter.type = choose_vehicle_type();
    filter.min_capacity = read_double("Минимальная грузоподъемность, кг (0 - без ограничения): ");

    int availability = read_choice("Доступность (1 - свободен, 2 - занят или недоступен, 0 - любая): ", 0, 2);
    if (availability != 0) filter.is_available = (availability == 1);

    print_vehicle_list(analytics.find_vehicles(filter));
}

static void handle_analytics_sort_orders(const DeliveryAnalytics& analytics)
{
    cout << "Сортировать заказы по:\n1. ID\n2. Весу\n3. Району\n";
    int key_choice = read_choice("Выберите критерий: ", 1, 3);
    int order_choice = read_choice("Порядок (1 - по возрастанию, 2 - по убыванию): ", 1, 2);

    OrderSortKey key = OrderSortKey::Id;
    if (key_choice == 2) key = OrderSortKey::Weight;
    else if (key_choice == 3) key = OrderSortKey::District;

    print_order_list(analytics.sort_orders(key, order_choice == 1));
}

static void handle_analytics_sort_vehicles(const DeliveryAnalytics& analytics)
{
    cout << "Сортировать транспорт по:\n1. ID\n2. Грузоподъемности\n3. Имени курьера\n";
    int key_choice = read_choice("Выберите критерий: ", 1, 3);
    int order_choice = read_choice("Порядок (1 - по возрастанию, 2 - по убыванию): ", 1, 2);

    VehicleSortKey key = VehicleSortKey::Id;
    if (key_choice == 2) key = VehicleSortKey::Capacity;
    else if (key_choice == 3) key = VehicleSortKey::CourierName;

    print_vehicle_list(analytics.sort_vehicles(key, order_choice == 1));
}

static void handle_analytics_order_extremes(const DeliveryAnalytics& analytics)
{
    auto [lightest, heaviest] = analytics.find_lightest_and_heaviest_order();

    cout << "Самый лёгкий заказ: " << *lightest << endl;
    cout << "Самый тяжёлый заказ: " << *heaviest << endl;
}

static void handle_analytics_vehicle_extremes(const DeliveryAnalytics& analytics)
{
    auto [smallest, largest] = analytics.find_smallest_and_largest_vehicle();

    cout << "Наименьшая грузоподъемность: " << *smallest << endl;
    cout << "Наибольшая грузоподъемность: " << *largest << endl;
}

static void handle_analytics_count_waiting_orders(const DeliveryAnalytics& analytics)
{
    cout << "Заказов, ожидающих назначения: " << analytics.count_waiting_orders() << endl;
}

static void handle_analytics_count_heavy_orders(const DeliveryAnalytics& analytics)
{
    double weight = read_double("Введите вес (кг): ");
    cout << "Заказов тяжелее " << weight << " кг: " << analytics.count_orders_heavier_than(weight) << endl;
}

static void handle_analytics_count_available_vehicles(const DeliveryAnalytics& analytics)
{
    cout << "Свободного транспорта: " << analytics.count_available_vehicles() << endl;
}

static void handle_analytics_group_orders(const DeliveryAnalytics& analytics)
{
    DeliveryAnalytics::OrderGroups groups = analytics.group_orders_by_district();
    if (groups.empty())
    {
        cout << "Заказов нет." << endl;
        return;
    }

    cout << "\n--- Заказы по районам ---" << endl;
    for_each(groups.begin(), groups.end(),
        [](const DeliveryAnalytics::OrderGroups::value_type& group)
        {
            cout << group.first << " (заказов: " << group.second.size() << "):" << endl;
            for_each(group.second.begin(), group.second.end(),
                [](const Order* order) { cout << "  " << *order << endl; });
        });
}

static void handle_analytics_group_vehicles(const DeliveryAnalytics& analytics)
{
    DeliveryAnalytics::VehicleGroups groups = analytics.group_vehicles_by_type();
    if (groups.empty())
    {
        cout << "Транспорта нет." << endl;
        return;
    }

    cout << "\n--- Транспорт по типам ---" << endl;
    for_each(groups.begin(), groups.end(),
        [](const DeliveryAnalytics::VehicleGroups::value_type& group)
        {
            cout << group.first << " (единиц: " << group.second.size() << "):" << endl;
            for_each(group.second.begin(), group.second.end(),
                [](const Vehicle* vehicle) { cout << "  " << *vehicle << endl; });
        });
}

static void handle_analytics_districts(const DeliveryAnalytics& analytics)
{
    set<string> districts = analytics.get_unique_districts();
    if (districts.empty())
    {
        cout << "Районов нет." << endl;
        return;
    }

    cout << "Районов: " << districts.size() << endl;
    for_each(districts.begin(), districts.end(), [](const string& district) { cout << "  " << district << endl; });
}

static void handle_analytics_order_weight_stats(const DeliveryAnalytics& analytics)
{
    double average = analytics.get_average_order_weight();

    cout << "Суммарный вес всех заказов: " << format("{:.1f}", analytics.get_total_order_weight()) << " кг" << endl;
    cout << "Из них ожидают назначения: " << format("{:.1f}", analytics.get_waiting_order_weight()) << " кг" << endl;
    cout << "Средний вес заказа: " << format("{:.1f}", average) << " кг" << endl;
}

static void handle_analytics_weight_by_district(const DeliveryAnalytics& analytics)
{
    map<string, double> weights = analytics.get_weight_by_district();
    if (weights.empty())
    {
        cout << "Заказов нет." << endl;
        return;
    }

    cout << "\n--- Вес заказов по районам ---" << endl;
    for_each(weights.begin(), weights.end(),
        [](const map<string, double>::value_type& entry)
        {
            cout << entry.first << ": " << format("{:.1f}", entry.second) << " кг" << endl;
        });
}

static void handle_analytics_vehicle_capacity_stats(const DeliveryAnalytics& analytics)
{
    cout << "Общая грузоподъемность всего транспорта: "
        << format("{:.1f}", analytics.get_total_vehicle_capacity()) << " кг" << endl;
    cout << "Грузоподъемность свободного транспорта: "
        << format("{:.1f}", analytics.get_available_vehicle_capacity()) << " кг" << endl;
}

static void handle_analytics_load_ratio(const DeliveryAnalytics& analytics)
{
    double ratio = analytics.get_waiting_load_ratio();

    cout << "Вес ожидающих заказов: " << format("{:.1f}", analytics.get_waiting_order_weight()) << " кг" << endl;
    cout << "Грузоподъемность свободного транспорта: "
        << format("{:.1f}", analytics.get_available_vehicle_capacity()) << " кг" << endl;
    cout << "Загрузка: " << format("{:.1f}", ratio * 100) << "%" << endl;

    if (ratio > 1) cout << "Свободного транспорта не хватит на все ожидающие заказы." << endl;
}