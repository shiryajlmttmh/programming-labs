#include "AnalyticsUI.h"
#include "UICommon.h"
#include "InputUtils.h"
#include "DeliveryService.h"
#include "DeliveryAnalytics.h"
#include "Motorcycle.h"
#include "Car.h"
#include "Truck.h"

#include <iostream>
#include <string>
#include <format>
#include <algorithm>
#include <vector>
#include <map>
#include <set>

using namespace std;

static void print_analytics_menu();
static void print_order_list(const vector<const Order*>& orders);
static void print_vehicle_list(const vector<const Vehicle*>& vehicles);
static string choose_vehicle_type();

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

void handle_analytics_menu(const DeliveryService& delivery_service)
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