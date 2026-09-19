#pragma once
#include "Vehicle.h"
#include <string>
#include <iostream>

class Car : public Vehicle
{
private:
    static constexpr double MAX_CAPACITY = 1500.0;
    static constexpr double MIN_TRUNK_VOLUME = 0.0;
    static constexpr double MAX_TRUNK_VOLUME = 2000.0;
    static constexpr double SPACIOUS_TRUNK_THRESHOLD = 400.0;

    double trunk_volume;

    static double validate_capacity(double capacity);

public:
    Car();
    Car(int id, double capacity, const std::string& courier_name, bool is_available = true, double trunk_volume = MIN_TRUNK_VOLUME);

    double get_trunk_volume() const;
    void set_trunk_volume(double new_volume);

    bool is_trunk_spacious() const;

    std::string get_type() const override;
    double get_max_capacity() const override;
    std::string get_full_info() const override;
    void perform_specific_action() override;

    friend std::istream& operator>>(std::istream& is, Car& car);
};

std::ostream& operator<<(std::ostream& os, const Car& car);