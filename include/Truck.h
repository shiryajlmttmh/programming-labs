#pragma once
#include "Vehicle.h"
#include <string>
#include <iostream>

class Truck : public Vehicle
{
private:
    static constexpr double MAX_CAPACITY = 10000.0;

    bool has_tail_lift;

    static double validate_capacity(double capacity);

public:
    Truck();
    Truck(int id, double capacity, const std::string& courier_name, bool is_available = true, bool has_tail_lift = false);

    bool get_has_tail_lift() const;
    void set_has_tail_lift(bool new_status);

    void toggle_tail_lift();

    std::string get_type() const override;
    double get_max_capacity() const override;
    std::string get_full_info() const override;
    void perform_specific_action() override;

    friend std::istream& operator>>(std::istream& is, Truck& truck);
};

std::ostream& operator<<(std::ostream& os, const Truck& truck);