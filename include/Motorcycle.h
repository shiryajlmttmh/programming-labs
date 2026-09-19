#pragma once
#include "Vehicle.h"
#include <string>
#include <iostream>

class Motorcycle : public Vehicle
{
private:
    static constexpr double MAX_CAPACITY = 50.0;

    bool has_thermal_box;

    static double validate_capacity(double capacity);

public:
    Motorcycle();
    Motorcycle(int id, double capacity, const std::string& courier_name, bool is_available = true, bool has_thermal_box = false);

    bool get_has_thermal_box() const;
    void set_has_thermal_box(bool new_status);

    void toggle_thermal_box();

    std::string get_type() const override;
    double get_max_capacity() const override;
    std::string get_full_info() const override;
    void perform_specific_action() override;

    friend std::istream& operator>>(std::istream& is, Motorcycle& motorcycle);
};

std::ostream& operator<<(std::ostream& os, const Motorcycle& motorcycle);