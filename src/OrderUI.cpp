#include "OrderUI.h"
#include "UICommon.h"
#include "InputUtils.h"
#include "DeliveryService.h"
#include "Order.h"

#include <iostream>
#include <string>
#include <format>

using namespace std;

static void print_manage_order_menu(int order_id);
static void print_order_comparison(const Order* first_order, const Order* second_order, const string& op_symbol, bool result);

static void handle_change_order_id(DeliveryService& delivery_service, Order* order);
static void handle_change_order_address(Order* order);
static void handle_change_order_weight(Order* order);
static void handle_change_order_district(Order* order);

void handle_add_order(DeliveryService& delivery_service)
{
    Order order;
    cin >> order;
    delivery_service += order;
    logger.log("ADD_ORDER", order.get_full_info());
}

void handle_remove_order(DeliveryService& delivery_service)
{
    int order_id = read_int("Введите ID заказа для удаления: ");
    delivery_service -= order_id;
    logger.log("REMOVE_ORDER", "Заказ номер " + to_string(order_id) + " удалён");
}

void handle_manage_order(DeliveryService& delivery_service)
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

void handle_compare_orders(DeliveryService& delivery_service)
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

static void print_order_comparison(const Order* first_order, const Order* second_order, const string& op_symbol, bool result)
{
    cout << "(первый заказ " << op_symbol << " второй заказ) => " << (result ? "true" : "false")
        << " (Вес номера " << first_order->get_id() << ": " << first_order->get_weight() << " кг "
        << op_symbol << " Вес номера " << second_order->get_id() << ": " << second_order->get_weight() << " кг)" << endl;
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