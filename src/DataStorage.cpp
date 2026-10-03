#include "DataStorage.h"
#include "DeliveryService.h"
#include "Vehicle.h"
#include "Order.h"
#include "Car.h"
#include "Motorcycle.h"
#include "Truck.h"
#include <memory>
#include "Exceptions.h"
#include <fstream>
#include <format>
#include <charconv>
#include <system_error>
#include <cmath>
#include <vector>

using namespace std;

static constexpr char FIELD_SEPARATOR = ';';
static constexpr const char* FORMAT_HEADER = "DELIVERY_STATE";
static constexpr int FORMAT_VERSION = 1;
static constexpr const char* VEHICLES_SECTION = "VEHICLES";
static constexpr const char* ORDERS_SECTION = "ORDERS";
static constexpr const char* CAR_CODE = "CAR";
static constexpr const char* MOTORCYCLE_CODE = "MOTORCYCLE";
static constexpr const char* TRUCK_CODE = "TRUCK";
static constexpr int NO_ORDER_ID = -1;

static constexpr size_t HEADER_FIELD_COUNT = 2;
static constexpr size_t SECTION_HEADER_FIELD_COUNT = 2;
static constexpr size_t VEHICLE_FIELD_COUNT = 7;
static constexpr size_t ORDER_FIELD_COUNT = 5;

struct VehicleLink
{
    int vehicle_id;
    int order_id;
    int line_number;
};

struct OrderFlag
{
    int order_id;
    bool is_assigned;
    int line_number;
};

static void validate_text_field(const string& value, const string& field_description);
static void validate_service_text_fields(const DeliveryService& service);
static void write_vehicle(ostream& os, const Vehicle& vehicle);
static void write_order(ostream& os, const Order& order);

static vector<string> split_line(const string& line);
static void require_field_count(const vector<string>& fields, size_t expected_count, int line_number);
static int parse_int(const string& text, int line_number, const string& field_name);
static double parse_double(const string& text, int line_number, const string& field_name);
static string read_line(istream& file, const string& filename, int& line_number);
static int parse_flag(const string& text, int line_number, const string& field_name);

static void read_file_header(istream& file, const string& filename, int& line_number);
static int read_section_header(istream& file, const string& filename, int& line_number, const string& section_name);
static unique_ptr<Vehicle> parse_vehicle(const vector<string>& fields, int line_number, VehicleLink& link);
static Order parse_order(const vector<string>& fields, int line_number, OrderFlag& flag);
static void ensure_no_trailing_data(istream& file, const string& filename, int& line_number);
static void restore_links(DeliveryService& service, const vector<VehicleLink>& vehicle_links, const vector<OrderFlag>& order_flags);

DataStorage::DataStorage(const string& filename) : filename(filename) {}

void DataStorage::save(const DeliveryService& service) const
{
    validate_service_text_fields(service);

    ofstream file(filename);
    if (!file) throw FileOpenException(filename, "записи");

    file << FORMAT_HEADER << FIELD_SEPARATOR << FORMAT_VERSION << '\n';

    file << VEHICLES_SECTION << FIELD_SEPARATOR << service.get_vehicles_count() << '\n';
    for (size_t i = 0; i < service.get_vehicles_count(); i++)
        write_vehicle(file, service.get_vehicle_by_index(i));

    file << ORDERS_SECTION << FIELD_SEPARATOR << service.get_orders_count() << '\n';
    for (size_t i = 0; i < service.get_orders_count(); i++)
        write_order(file, service.get_order_by_index(i));

    file.close();
    if (!file) throw FileIOException(filename, "записи");
}

DeliveryService DataStorage::load() const
{
    ifstream file(filename);
    if (!file) throw FileOpenException(filename, "чтения");

    int line_number = 0;
    read_file_header(file, filename, line_number);

    DeliveryService service;
    vector<VehicleLink> vehicle_links;
    vector<OrderFlag> order_flags;

    int vehicle_count = read_section_header(file, filename, line_number, VEHICLES_SECTION);
    for (int i = 0; i < vehicle_count; i++)
    {
        string line = read_line(file, filename, line_number);

        try
        {
            VehicleLink link{};
            service.add_vehicle(parse_vehicle(split_line(line), line_number, link));
            vehicle_links.push_back(link);
        }
        catch (const FileFormatException&) { throw; }
        catch (const DeliveryException& e) { throw FileFormatException(line_number, e.what()); }
    }

    int order_count = read_section_header(file, filename, line_number, ORDERS_SECTION);
    for (int i = 0; i < order_count; i++)
    {
        string line = read_line(file, filename, line_number);

        try
        {
            OrderFlag flag{};
            service.add_order(parse_order(split_line(line), line_number, flag));
            order_flags.push_back(flag);
        }
        catch (const FileFormatException&) { throw; }
        catch (const DeliveryException& e) { throw FileFormatException(line_number, e.what()); }
    }

    ensure_no_trailing_data(file, filename, line_number);
    restore_links(service, vehicle_links, order_flags);

    return service;
}

static void validate_text_field(const string& value, const string& field_description)
{
    if (value.find_first_of(string(1, FIELD_SEPARATOR) + "\r\n") != string::npos)
        throw InvalidDataException("Поле \"" + field_description + "\" содержит запрещённый символ ('"
            + string(1, FIELD_SEPARATOR) + "' или перевод строки): \"" + value + "\"");
}

static void validate_service_text_fields(const DeliveryService& service)
{
    for (size_t i = 0; i < service.get_vehicles_count(); i++)
    {
        const Vehicle& vehicle = service.get_vehicle_by_index(i);
        validate_text_field(vehicle.get_courier_name(), "имя курьера, транспорт номер " + to_string(vehicle.get_id()));
    }

    for (size_t i = 0; i < service.get_orders_count(); i++)
    {
        const Order& order = service.get_order_by_index(i);
        validate_text_field(order.get_address(), "адрес, заказ номер " + to_string(order.get_id()));
        validate_text_field(order.get_district(), "район, заказ номер " + to_string(order.get_id()));
    }
}

static void write_vehicle(ostream& os, const Vehicle& vehicle)
{
    os << vehicle.get_type_code() << FIELD_SEPARATOR
        << vehicle.get_id() << FIELD_SEPARATOR
        << format("{}", vehicle.get_capacity()) << FIELD_SEPARATOR
        << vehicle.get_courier_name() << FIELD_SEPARATOR
        << (vehicle.get_is_available() ? 1 : 0) << FIELD_SEPARATOR
        << vehicle.get_current_order_id() << FIELD_SEPARATOR
        << vehicle.get_specific_field() << '\n';
}

static void write_order(ostream& os, const Order& order)
{
    os << order.get_id() << FIELD_SEPARATOR
        << order.get_address() << FIELD_SEPARATOR
        << format("{}", order.get_weight()) << FIELD_SEPARATOR
        << order.get_district() << FIELD_SEPARATOR
        << (order.get_is_assigned() ? 1 : 0) << '\n';
}

static vector<string> split_line(const string& line)
{
    vector<string> fields;
    size_t start = 0;

    while (true)
    {
        size_t separator_pos = line.find(FIELD_SEPARATOR, start);
        if (separator_pos == string::npos)
        {
            fields.push_back(line.substr(start));
            break;
        }

        fields.push_back(line.substr(start, separator_pos - start));
        start = separator_pos + 1;
    }

    return fields;
}

static void require_field_count(const vector<string>& fields, size_t expected_count, int line_number)
{
    if (fields.size() != expected_count)
        throw FileFormatException(line_number, "ожидалось полей: " + to_string(expected_count)
            + ", получено: " + to_string(fields.size()));
}

static int parse_int(const string& text, int line_number, const string& field_name)
{
    int value = 0;
    const char* begin = text.data();
    const char* end = begin + text.size();

    auto [parsed_end, error] = from_chars(begin, end, value);
    if (text.empty() || error != errc() || parsed_end != end)
        throw FileFormatException(line_number, "поле \"" + field_name + "\" должно быть целым числом, получено: \"" + text + "\"");

    return value;
}

static double parse_double(const string& text, int line_number, const string& field_name)
{
    double value = 0;
    const char* begin = text.data();
    const char* end = begin + text.size();

    auto [parsed_end, error] = from_chars(begin, end, value);
    if (text.empty() || error != errc() || parsed_end != end || !isfinite(value))
        throw FileFormatException(line_number, "поле \"" + field_name + "\" должно быть числом, получено: \"" + text + "\"");

    return value;
}

static string read_line(istream& file, const string& filename, int& line_number)
{
    string line;
    if (!getline(file, line))
    {
        if (!file.eof()) throw FileIOException(filename, "чтения");
        throw FileFormatException(line_number + 1, "неожиданный конец файла");
    }

    line_number++;

    if (line_number == 1 && line.starts_with("\xEF\xBB\xBF")) line.erase(0, 3);
    if (!line.empty() && line.back() == '\r') line.pop_back();

    return line;
}

static int parse_flag(const string& text, int line_number, const string& field_name)
{
    int value = parse_int(text, line_number, field_name);
    if (value != 0 && value != 1)
        throw FileFormatException(line_number, "поле \"" + field_name + "\" должно быть 0 или 1, получено: " + to_string(value));

    return value;
}

static void read_file_header(istream& file, const string& filename, int& line_number)
{
    string line = read_line(file, filename, line_number);
    vector<string> fields = split_line(line);

    if (fields[0] != FORMAT_HEADER)
        throw FileFormatException(line_number, "файл не является сохранением службы доставки (ожидался заголовок \""
            + string(FORMAT_HEADER) + "\")");

    require_field_count(fields, HEADER_FIELD_COUNT, line_number);

    int version = parse_int(fields[1], line_number, "версия формата");
    if (version != FORMAT_VERSION)
        throw FileFormatException(line_number, "неподдерживаемая версия формата: " + to_string(version)
            + " (поддерживается: " + to_string(FORMAT_VERSION) + ")");
}

static int read_section_header(istream& file, const string& filename, int& line_number, const string& section_name)
{
    string line = read_line(file, filename, line_number);
    vector<string> fields = split_line(line);

    if (fields[0] != section_name)
        throw FileFormatException(line_number, "ожидалась секция \"" + section_name + "\", получено: \"" + line + "\"");

    require_field_count(fields, SECTION_HEADER_FIELD_COUNT, line_number);

    int count = parse_int(fields[1], line_number, "количество записей");
    if (count < 0)
        throw FileFormatException(line_number, "количество записей не может быть отрицательным: " + to_string(count));

    return count;
}

static unique_ptr<Vehicle> parse_vehicle(const vector<string>& fields, int line_number, VehicleLink& link)
{
    const string& type_code = fields[0];
    if (type_code != CAR_CODE && type_code != MOTORCYCLE_CODE && type_code != TRUCK_CODE)
        throw FileFormatException(line_number, "неизвестный тип транспорта: \"" + type_code + "\"");

    require_field_count(fields, VEHICLE_FIELD_COUNT, line_number);

    int id = parse_int(fields[1], line_number, "ID транспорта");
    double capacity = parse_double(fields[2], line_number, "грузоподъемность");
    const string& courier_name = fields[3];
    bool is_available = parse_flag(fields[4], line_number, "доступность") == 1;
    int order_id = parse_int(fields[5], line_number, "ID текущего заказа");

    if (order_id != NO_ORDER_ID && is_available)
        throw FileFormatException(line_number, "транспорт номер " + to_string(id)
            + " отмечен свободным, но выполняет заказ номер " + to_string(order_id));

    link = { id, order_id, line_number };

    if (type_code == CAR_CODE)
        return make_unique<Car>(id, capacity, courier_name, is_available,
            parse_double(fields[6], line_number, "объём багажника"));

    if (type_code == MOTORCYCLE_CODE)
        return make_unique<Motorcycle>(id, capacity, courier_name, is_available,
            parse_flag(fields[6], line_number, "наличие термокороба") == 1);

    return make_unique<Truck>(id, capacity, courier_name, is_available,
        parse_flag(fields[6], line_number, "наличие гидроборта") == 1);
}

static Order parse_order(const vector<string>& fields, int line_number, OrderFlag& flag)
{
    require_field_count(fields, ORDER_FIELD_COUNT, line_number);

    int id = parse_int(fields[0], line_number, "ID заказа");
    double weight = parse_double(fields[2], line_number, "вес");
    bool is_assigned = parse_flag(fields[4], line_number, "статус назначения") == 1;

    flag = { id, is_assigned, line_number };

    return Order(id, fields[1], weight, fields[3]);
}

static void ensure_no_trailing_data(istream& file, const string& filename, int& line_number)
{
    string line;
    while (getline(file, line))
    {
        line_number++;
        if (!line.empty() && line.back() == '\r') line.pop_back();

        if (!line.empty())
            throw FileFormatException(line_number, "лишние данные после последней записи");
    }

    if (!file.eof()) throw FileIOException(filename, "чтения");
}

static void restore_links(DeliveryService& service, const vector<VehicleLink>& vehicle_links, const vector<OrderFlag>& order_flags)
{
    for (const VehicleLink& link : vehicle_links)
    {
        if (link.order_id == NO_ORDER_ID) continue;

        try
        {
            service.restore_assignment(link.vehicle_id, link.order_id);
        }
        catch (const DeliveryException& e)
        {
            throw FileFormatException(link.line_number, "связь транспорта номер " + to_string(link.vehicle_id)
                + " с заказом номер " + to_string(link.order_id) + ": " + e.what());
        }
    }

    for (const OrderFlag& flag : order_flags)
    {
        bool actually_assigned = service.get_order(flag.order_id)->get_is_assigned();
        if (actually_assigned == flag.is_assigned) continue;

        if (flag.is_assigned)
            throw FileFormatException(flag.line_number, "заказ номер " + to_string(flag.order_id)
                + " отмечен как доставляемый, но ни один транспорт его не выполняет");

        throw FileFormatException(flag.line_number, "заказ номер " + to_string(flag.order_id)
            + " выполняется транспортом, но отмечен как не назначенный");
    }
}