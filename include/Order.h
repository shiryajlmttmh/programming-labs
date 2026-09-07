#pragma once
#include <iostream>
#include <string>

class Order
{
private:
    static constexpr double MIN_WEIGHT = 0.1;
    static constexpr double MAX_WEIGHT = 1000.0;

    int id;
    std::string address;
    double weight;
    std::string district;
    bool is_assigned;

public:
    Order(int id, const std::string& address, double weight, const std::string& district);

    int get_id() const;
    std::string get_address() const;
    double get_weight() const;
    std::string get_district() const;
    bool get_is_assigned() const;

    void set_id(int new_id);
    void set_address(const std::string& new_address);
    void set_weight(double new_weight);
    void set_district(const std::string& new_district);
    void set_is_assigned(bool status);

    std::string get_full_info() const;
    void print_full_info() const;
};