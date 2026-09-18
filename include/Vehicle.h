#pragma once
#include <string>
#include <iostream>

class Order;

class Vehicle
{
protected:
    static constexpr double MAX_MOTORCYCLE_CAPACITY = 50.0;
    static constexpr double MAX_CAR_CAPACITY = 1500.0;
    static constexpr double MAX_TRUCK_CAPACITY = 10000.0;

private:
    int id;
    std::string type;
    double capacity;
    std::string courier_name;
    bool is_available;
    int current_order_id;

    bool is_valid_type(const std::string& check_type) const;
    double get_max_capacity_for_type(const std::string& check_type) const;

public:
    Vehicle();
    Vehicle(int id, const std::string& type, double capacity, const std::string& courier_name, bool is_available = true);

    bool assign_order(const Order& order);
    void complete_delivery();

    int get_id() const;
    std::string get_type() const;
    double get_capacity() const;
    std::string get_courier_name() const;
    bool get_is_available() const;
    int get_current_order_id() const;

    void set_id(int new_id);
    void set_type(const std::string& new_type);
    void set_capacity(double new_capacity);
    void set_courier_name(const std::string& new_courier_name);
    void set_is_available(bool new_status);

    std::string get_full_info() const;
    void print_full_info() const;

    bool operator==(const Vehicle& other) const;
    bool operator!=(const Vehicle& other) const;

    bool operator<(const Vehicle& other) const;
    bool operator>(const Vehicle& other) const;
    bool operator<=(const Vehicle& other) const;
    bool operator>=(const Vehicle& other) const;

    friend std::istream& operator>>(std::istream& is, Vehicle& vehicle);
};

std::ostream& operator<<(std::ostream& os, const Vehicle& vehicle);