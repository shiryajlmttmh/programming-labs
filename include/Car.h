#pragma once
#include "Vehicle.h"
#include <string>
#include <iostream>

class Car : public Vehicle
{
private:
    static constexpr double MAX_CAPACITY = 1500.0;
    static constexpr double MIN_TRUNK_VOLUME = 100.0;
    static constexpr double DEFAULT_TRUNK_VOLUME = 300.0;
    static constexpr double MAX_TRUNK_VOLUME = 2000.0;
    static constexpr double SPACIOUS_TRUNK_THRESHOLD = 400.0;

    static constexpr double BASE_COST = 300.0;
    static constexpr double COST_PER_KG = 4.0;
    static constexpr double SPACIOUS_TRUNK_DISCOUNT = 0.9;

    double trunk_volume;

public:
    Car();
    Car(int id, double capacity, const std::string& courier_name, bool is_available = true, double trunk_volume = DEFAULT_TRUNK_VOLUME);

    double get_trunk_volume() const;
    void set_trunk_volume(double new_volume);

    bool is_trunk_spacious() const;

    std::string get_type() const override;
    double get_max_capacity() const override;
    std::string get_full_info() const override;
    void perform_specific_action() override;
    std::string get_specific_action_name() const override;
    double calculate_delivery_cost(double order_weight) const override;

    friend std::istream& operator>>(std::istream& is, Car& car);
};