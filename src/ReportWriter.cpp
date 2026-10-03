#include "ReportWriter.h"
#include "Vehicle.h"
#include "Order.h"
#include "Exceptions.h"
#include "TimeUtils.h"
#include <fstream>
#include <format>
#include <map>

using namespace std;

static constexpr int NO_ORDER_ID = -1;

static void write_summary(ostream& os, const DeliveryService& service);
static void write_vehicles(ostream& os, const DeliveryService& service);
static void write_orders(ostream& os, const DeliveryService& service);

ReportWriter::ReportWriter(const string& filename) : filename(filename) {}

void ReportWriter::write(const DeliveryService& service) const
{
    ofstream file(filename);
    if (!file) throw FileOpenException(filename, "записи");

    file << "ОТЧЁТ СЛУЖБЫ ДОСТАВКИ\n";
    file << "Дата формирования: " << get_current_timestamp() << "\n\n";

    write_summary(file, service);
    write_vehicles(file, service);
    write_orders(file, service);

    file.close();
    if (!file) throw FileIOException(filename, "записи");
}

static void write_summary(ostream& os, const DeliveryService& service)
{
    map<string, int> vehicles_by_type;
    int free_count = 0;
    int busy_count = 0;
    int unavailable_count = 0;
    double total_capacity = 0;

    for (size_t i = 0; i < service.get_vehicles_count(); i++)
    {
        const Vehicle& vehicle = service.get_vehicle_by_index(i);

        vehicles_by_type[vehicle.get_type()]++;
        total_capacity += vehicle.get_capacity();

        if (vehicle.get_is_available()) free_count++;
        else if (vehicle.get_current_order_id() != NO_ORDER_ID) busy_count++;
        else unavailable_count++;
    }

    map<string, int> orders_by_district;
    int waiting_count = 0;
    int delivering_count = 0;
    double total_weight = 0;
    double waiting_weight = 0;

    for (size_t i = 0; i < service.get_orders_count(); i++)
    {
        const Order& order = service.get_order_by_index(i);

        orders_by_district[order.get_district().empty() ? "(не указан)" : order.get_district()]++;
        total_weight += order.get_weight();

        if (order.get_is_assigned()) delivering_count++;
        else
        {
            waiting_count++;
            waiting_weight += order.get_weight();
        }
    }

    os << "=== СВОДКА ===\n";

    os << "Транспорт: всего " << service.get_vehicles_count() << "\n";
    for (const auto& [type_name, count] : vehicles_by_type)
        os << "  " << type_name << ": " << count << "\n";
    os << "  Свободно: " << free_count << "\n"
        << "  Выполняют заказ: " << busy_count << "\n"
        << "  Недоступно: " << unavailable_count << "\n"
        << "  Суммарная грузоподъемность: " << format("{:.1f}", total_capacity) << " кг\n";

    os << "Заказы: всего " << service.get_orders_count() << "\n"
        << "  Ожидают назначения: " << waiting_count << "\n"
        << "  Доставляются: " << delivering_count << "\n"
        << "  Суммарный вес: " << format("{:.1f}", total_weight) << " кг"
        << " (из них ожидают назначения: " << format("{:.1f}", waiting_weight) << " кг)\n";

    if (!orders_by_district.empty())
    {
        os << "  По районам:\n";
        for (const auto& [district, count] : orders_by_district)
            os << "    " << district << ": " << count << "\n";
    }

    os << "\n";
}

static void write_vehicles(ostream& os, const DeliveryService& service)
{
    os << "=== ТРАНСПОРТ ===\n";

    if (service.get_vehicles_count() == 0) os << "Список пуст.\n";

    for (size_t i = 0; i < service.get_vehicles_count(); i++)
        os << (i + 1) << ". " << service.get_vehicle_by_index(i) << "\n";

    os << "\n";
}

static void write_orders(ostream& os, const DeliveryService& service)
{
    os << "=== ЗАКАЗЫ ===\n";

    if (service.get_orders_count() == 0) os << "Список пуст.\n";

    for (size_t i = 0; i < service.get_orders_count(); i++)
        os << (i + 1) << ". " << service.get_order_by_index(i) << "\n";
}