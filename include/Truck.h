#pragma once
#include "Vehicle.h"
#include <string>
#include <iostream>

class Truck : public Vehicle
{
private:
    static constexpr double MAX_CAPACITY = 10000.0;
    static constexpr double BASE_COST = 800.0;
    static constexpr double COST_PER_KG = 2.0;
    static constexpr double TAIL_LIFT_SURCHARGE = 200.0;

    bool has_tail_lift;

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
    std::string get_specific_action_name() const override;
    double calculate_delivery_cost(double order_weight) const override;

    friend std::istream& operator>>(std::istream& is, Truck& truck);
};  