#include "PersistenceUI.h"
#include "UICommon.h"
#include "DeliveryService.h"
#include "DataStorage.h"
#include "ReportWriter.h"
#include "Motorcycle.h"
#include "Car.h"
#include "Truck.h"
#include "Order.h"

#include <iostream>
#include <memory>
#include <string>
#include <filesystem>
#include <system_error>

using namespace std;

static void seed_data(DeliveryService& delivery_service);

void handle_save_state(const DeliveryService& delivery_service)
{
    DataStorage storage(DATA_FILE_NAME);
    storage.save(delivery_service);
    cout << "Состояние сохранено в файл " << DATA_FILE_NAME << "." << endl;
    logger.log("SAVE", string("Состояние сохранено в файл ") + DATA_FILE_NAME);
}

void handle_load_state(DeliveryService& delivery_service)
{
    DataStorage storage(DATA_FILE_NAME);
    delivery_service = storage.load();
    cout << "Состояние загружено из файла " << DATA_FILE_NAME << "." << endl;
    logger.log("LOAD", string("Состояние загружено из файла ") + DATA_FILE_NAME);
}

void handle_create_report(const DeliveryService& delivery_service)
{
    ReportWriter report_writer(REPORT_FILE_NAME);
    report_writer.write(delivery_service);
    cout << "Отчёт сформирован в файле " << REPORT_FILE_NAME << "." << endl;
    logger.log("REPORT", string("Сформирован отчёт ") + REPORT_FILE_NAME);
}

bool initialize_state(DeliveryService& delivery_service)
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

void auto_save_state(const DeliveryService& delivery_service)
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

static void seed_data(DeliveryService& delivery_service)
{
    delivery_service += make_unique<Motorcycle>(1, 25.0, "Иван", true, true);
    delivery_service += make_unique<Car>(2, 450.0, "Алексей", true, 500.0);
    delivery_service += make_unique<Truck>(3, 5000.0, "Дмитрий", true, true);

    delivery_service += Order(101, "ул. Ленина, 5", 15.0, "Центральный");
    delivery_service += Order(102, "пр. Мира, 12", 250.0, "Северный");

    cout << "Тестовые данные успешно загружены!" << endl;
}