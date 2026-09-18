#pragma once
#include "Vehicle.h"
#include <string>
#include <iostream>

class Motorcycle : public Vehicle
{
private:
    bool has_thermal_box;

public:
    Motorcycle();
    Motorcycle(int id, double capacity, const std::string& courier_name, bool is_available = true, bool has_thermal_box = false);

    bool get_has_thermal_box() const;
    void set_has_thermal_box(bool new_status);

    void toggle_thermal_box();
    std::string get_full_info() const;

    friend std::ostream& operator<<(std::ostream& os, const Motorcycle& motorcycle);
    friend std::istream& operator>>(std::istream& is, Motorcycle& motorcycle);
};