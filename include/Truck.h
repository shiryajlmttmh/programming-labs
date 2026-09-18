#pragma once
#include "Vehicle.h"
#include <string>
#include <iostream>

class Truck : public Vehicle
{
private:
    bool has_tail_lift;

public:
    Truck();
    Truck(int id, double capacity, const std::string& courier_name, bool is_available = true, bool has_tail_lift = false);

    bool get_has_tail_lift() const;
    void set_has_tail_lift(bool new_status);

    void toggle_tail_lift();
    std::string get_full_info() const;

    friend std::istream& operator>>(std::istream& is, Truck& truck);
};

std::ostream& operator<<(std::ostream& os, const Truck& truck);