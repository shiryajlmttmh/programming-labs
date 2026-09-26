#include "CollectionDemo.h"
#include "Collection.h"
#include "Order.h"
#include "Car.h"
#include "InputUtils.h"
#include <iostream>

using namespace std;

static void seed_collection_demo_data(Collection<Order>& orders_demo, Collection<Car>& cars_demo);
static void print_demo_menu();

static void handle_print_orders(const Collection<Order>& orders_demo);
static void handle_print_cars(const Collection<Car>& cars_demo);
static void handle_add_order_demo(Collection<Order>& orders_demo);
static void handle_add_car_demo(Collection<Car>& cars_demo);
static void handle_remove_order_demo(Collection<Order>& orders_demo);
static void handle_remove_car_demo(Collection<Car>& cars_demo);
static void handle_find_heavy_order(const Collection<Order>& orders_demo);
static void handle_find_spacious_car(const Collection<Car>& cars_demo);
static void handle_find_order_by_id(const Collection<Order>& orders_demo);
static void handle_find_car_by_id(const Collection<Car>& cars_demo);
static void handle_clear_orders_demo(Collection<Order>& orders_demo);
static void handle_clear_cars_demo(Collection<Car>& cars_demo);

void run_collection_demo_menu()
{
    Collection<Order> orders_demo;
    Collection<Car> cars_demo;

    seed_collection_demo_data(orders_demo, cars_demo);

    int menu_choice = -1;
    while (menu_choice != 0)
    {
        print_demo_menu();
        menu_choice = read_int("Выберите действие: ");

        switch (menu_choice)
        {
        case 1:  handle_print_orders(orders_demo); break;
        case 2:  handle_print_cars(cars_demo); break;
        case 3:  handle_add_order_demo(orders_demo); break;
        case 4:  handle_add_car_demo(cars_demo); break;
        case 5:  handle_remove_order_demo(orders_demo); break;
        case 6:  handle_remove_car_demo(cars_demo); break;
        case 7:  handle_find_heavy_order(orders_demo); break;
        case 8:  handle_find_spacious_car(cars_demo); break;
        case 9:  handle_find_order_by_id(orders_demo); break;
        case 10: handle_find_car_by_id(cars_demo); break;
        case 11: handle_clear_orders_demo(orders_demo); break;
        case 12: handle_clear_cars_demo(cars_demo); break;
        case 0:  break;
        default: cout << "Ошибка: некорректный пункт меню!" << endl;
        }
    }
}

static void seed_collection_demo_data(Collection<Order>& orders_demo, Collection<Car>& cars_demo)
{
    orders_demo.add_item(Order(1, "ул. Ленина, 10", 5.5, "Центральный"));
    orders_demo.add_item(Order(2, "ул. Мира, 25", 120.0, "Северный"));
    orders_demo.add_item(Order(3, "пр. Победы, 7", 850.0, "Южный"));

    cars_demo.add_item(Car(1, 1200.0, "Иванов Иван", true, 250.0));
    cars_demo.add_item(Car(2, 1400.0, "Петров Пётр", true, 500.0));
    cars_demo.add_item(Car(3, 900.0, "Сидоров Сидор", true, 150.0));

    cout << "Демонстрационные данные загружены: "
        << orders_demo.get_items_count() << " заказ(ов), "
        << cars_demo.get_items_count() << " машин(ы)." << endl;
}

static void print_demo_menu()
{
    cout << "\n--- Демонстрация шаблонного контейнера Collection<T> ---" << endl;
    cout << "1. Показать все заказы (Collection<Order>)\n"
        << "2. Показать все машины (Collection<Car>)\n"
        << "3. Добавить заказ в коллекцию\n"
        << "4. Добавить машину в коллекцию\n"
        << "5. Удалить заказ по индексу\n"
        << "6. Удалить машину по индексу\n"
        << "7. Найти заказ тяжелее заданного веса (поиск по критерию)\n"
        << "8. Найти машину с вместительным багажником (поиск по критерию)\n"
        << "9. Проверить наличие заказа с указанным ID (поиск по значению)\n"
        << "10. Проверить наличие машины с указанным ID (поиск по значению)\n"
        << "11. Очистить коллекцию заказов\n"
        << "12. Очистить коллекцию машин\n"
        << "0. Назад в главное меню\n";
}

static void handle_print_orders(const Collection<Order>& orders_demo)
{
    cout << "\n--- Заказы (" << orders_demo.get_items_count() << ") ---" << endl;
    orders_demo.print_collection();
}

static void handle_print_cars(const Collection<Car>& cars_demo)
{
    cout << "\n--- Машины (" << cars_demo.get_items_count() << ") ---" << endl;
    cars_demo.print_collection();
}

static void handle_add_order_demo(Collection<Order>& orders_demo)
{
    cout << "\n--- Добавление заказа ---" << endl;

    Order order;
    cin >> order;

    orders_demo.add_item(order);
    cout << "Заказ добавлен в коллекцию." << endl;
}

static void handle_add_car_demo(Collection<Car>& cars_demo)
{
    cout << "\n--- Добавление машины ---" << endl;

    Car car;
    cin >> car;

    cars_demo.add_item(car);
    cout << "Машина добавлена в коллекцию." << endl;
}

static void handle_remove_order_demo(Collection<Order>& orders_demo)
{
    size_t index = static_cast<size_t>(read_int("Введите индекс заказа для удаления: "));
    orders_demo.remove_item_by_index(index);
}

static void handle_remove_car_demo(Collection<Car>& cars_demo)
{
    size_t index = static_cast<size_t>(read_int("Введите индекс машины для удаления: "));
    cars_demo.remove_item_by_index(index);
}

static void handle_find_heavy_order(const Collection<Order>& orders_demo)
{
    double threshold = read_double("Введите пороговый вес (кг): ");
    const Order* found = find_if_matching(orders_demo,
        [threshold](const Order& order) { return order.get_weight() > threshold; });

    if (found != nullptr)
        cout << "Найден заказ: " << *found << endl;
    else
        cout << "Заказ тяжелее " << threshold << " кг не найден." << endl;
}

static void handle_find_spacious_car(const Collection<Car>& cars_demo)
{
    const Car* found = find_if_matching(cars_demo,
        [](const Car& car) { return car.is_trunk_spacious(); });

    if (found != nullptr)
        cout << "Найдена машина с вместительным багажником: " << found->get_full_info() << endl;
    else
        cout << "Машин с вместительным багажником не найдено." << endl;
}

static void handle_find_order_by_id(const Collection<Order>& orders_demo)
{
    int id = read_int("Введите ID заказа для поиска: ");
    Order candidate(id, "", Order().get_weight(), "");

    int index = orders_demo.find_index(candidate);
    if (index != -1)
        cout << "Заказ с ID " << id << " найден в коллекции на позиции " << index << "." << endl;
    else
        cout << "Заказ с ID " << id << " в коллекции не найден." << endl;
}

static void handle_find_car_by_id(const Collection<Car>& cars_demo)
{
    int id = read_int("Введите ID машины для поиска: ");
    Car candidate(id, 1.0, "", true, 100.0);

    int index = cars_demo.find_index(candidate);
    if (index != -1)
        cout << "Машина с ID " << id << " найдена в коллекции на позиции " << index << "." << endl;
    else
        cout << "Машина с ID " << id << " в коллекции не найдена." << endl;
}

static void handle_clear_orders_demo(Collection<Order>& orders_demo)
{
    orders_demo.clear_collection();
    cout << "Коллекция заказов очищена." << endl;
}

static void handle_clear_cars_demo(Collection<Car>& cars_demo)
{
    cars_demo.clear_collection();
    cout << "Коллекция машин очищена." << endl;
}