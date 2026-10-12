#include "ConsoleUI.h"
#include "InputUtils.h"
#include "DeliveryService.h"
#include "CollectionDemo.h"
#include "UICommon.h"
#include "VehicleUI.h"
#include "OrderUI.h"
#include "DeliveryOpsUI.h"
#include "PersistenceUI.h"
#include "AnalyticsUI.h"

#include <iostream>
#include <windows.h>

using namespace std;

static void setup_console_encoding();
static void print_menu();

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

static void setup_console_encoding()
{
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
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