#pragma once
#include <string>
#include <iostream>

class Order;

class Vehicle
{
private:
    int id;
    double capacity;
    std::string courier_name;
    bool is_available;
    int current_order_id;

protected:
    Vehicle(int id, double capacity, const std::string& courier_name, bool is_available = true);

    static double validate_capacity(double capacity, double max_capacity, const std::string& type_name);

public:
    virtual ~Vehicle() = default;

    bool assign_order(const Order& order);
    void complete_delivery();

    bool can_carry(double cargo_weight) const;

    int get_id() const;
    double get_capacity() const;
    std::string get_courier_name() const;
    bool get_is_available() const;
    int get_current_order_id() const;

    void set_id(int new_id);
    void set_capacity(double new_capacity);
    void set_courier_name(const std::string& new_courier_name);
    void set_is_available(bool new_status);

    virtual std::string get_type() const = 0;
    virtual double get_max_capacity() const = 0;
    virtual std::string get_full_info() const;
    virtual void perform_specific_action() = 0;
    virtual std::string get_specific_action_name() const = 0;
    virtual double calculate_delivery_cost(double order_weight) const = 0;

    bool operator==(const Vehicle& other) const;
    bool operator!=(const Vehicle& other) const;

    bool operator<(const Vehicle& other) const;
    bool operator>(const Vehicle& other) const;
    bool operator<=(const Vehicle& other) const;
    bool operator>=(const Vehicle& other) const;

    friend std::istream& operator>>(std::istream& is, Vehicle& vehicle);
};

std::ostream& operator<<(std::ostream& os, const Vehicle& vehicle);