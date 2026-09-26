#include "CollectionDemo.h"
#include "Collection.h"
#include "Order.h"
#include "Car.h"
#include "InputUtils.h"
#include <iostream>

using namespace std;

static void seed_collection_demo_data(Collection<Order>& orders_demo, Collection<Car>& cars_demo)
{
    orders_demo.add_item(Order(1, "ул. Ленина, 10", 5.5, "Центральный"));
    orders_demo.add_item(Order(2, "ул. Мира, 25", 120.0, "Северный"));
    orders_demo.add_item(Order(3, "пр. Победы, 7", 850.0, "Южный"));

    cars_demo.add_item(Car(1, 1200.0, "Иванов Иван", true, 250.0));
    cars_demo.add_item(Car(2, 1400.0, "Петров Пётр", true, 500.0));
    cars_demo.add_item(Car(3, 900.0, "Сидоров Сидор", false, 150.0));

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
        << "7. Найти заказ тяжелее заданного веса\n"
        << "8. Найти машину с вместительным багажником\n"
        << "9. Очистить коллекцию заказов\n"
        << "10. Очистить коллекцию машин\n"
        << "0. Назад в главное меню\n";
}

void run_collection_demo_menu()
{
    Collection<Order> orders_demo;
    Collection<Car> cars_demo;

    seed_collection_demo_data(orders_demo, cars_demo);

    bool running = true;
    while (running)
    {
        print_demo_menu();
        int choice = read_int("Выберите действие: ");

        switch (choice)
        {
        case 1:
            cout << "\n--- Заказы (" << orders_demo.get_items_count() << ") ---" << endl;
            orders_demo.print_collection();
            break;

        case 2:
            cout << "\n--- Машины (" << cars_demo.get_items_count() << ") ---" << endl;
            cars_demo.print_collection();
            break;

        case 3:
        {
            int id = read_int("Введите ID заказа: ");
            double weight = read_double("Введите вес (кг): ");
            orders_demo.add_item(Order(id, "Демо-адрес", weight, "Демо-район"));
            cout << "Заказ добавлен в коллекцию." << endl;
            break;
        }

        case 4:
        {
            int id = read_int("Введите ID машины: ");
            double capacity = read_double("Введите грузоподъёмность (кг): ");
            double trunk = read_double("Введите объём багажника (л): ");
            cars_demo.add_item(Car(id, capacity, "Демо-курьер", true, trunk));
            cout << "Машина добавлена в коллекцию." << endl;
            break;
        }

        case 5:
        {
            size_t index = static_cast<size_t>(read_int("Введите индекс заказа для удаления: "));
            orders_demo.remove_item_by_index(index);
            break;
        }

        case 6:
        {
            size_t index = static_cast<size_t>(read_int("Введите индекс машины для удаления: "));
            cars_demo.remove_item_by_index(index);
            break;
        }

        case 7:
        {
            double threshold = read_double("Введите пороговый вес (кг): ");
            const Order* found = find_if_matching(orders_demo, [threshold](const Order& order) { return order.get_weight() > threshold; });

            if (found != nullptr)
                cout << "Найден заказ: " << *found << endl;
            else
                cout << "Заказ тяжелее " << threshold << " кг не найден." << endl;
            break;
        }

        case 8:
        {
            const Car* found = find_if_matching(cars_demo, [](const Car& car) { return car.is_trunk_spacious(); });

            if (found != nullptr)
                cout << "Найдена машина с вместительным багажником: " << found->get_full_info() << endl;
            else
                cout << "Машин с вместительным багажником не найдено." << endl;
            break;
        }

        case 9:
            orders_demo.clear_collection();
            cout << "Коллекция заказов очищена." << endl;
            break;

        case 10:
            cars_demo.clear_collection();
            cout << "Коллекция машин очищена." << endl;
            break;

        case 0:
            running = false;
            break;

        default:
            cout << "Ошибка: некорректный пункт меню!" << endl;
            break;
        }
    }
}