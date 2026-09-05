#pragma once
#include <iostream>
#include <string>

class Order
{
private:
    const double MIN_WEIGHT = 0.1;
    const double MAX_WEIGHT = 1000;

    int id;
    std::string address;
    double weight;
    std::string district;

public:
    Order(int id, const std::string& address, double weight, const std::string& district);

    int GetId() const;
    std::string GetAddress() const;
    double GetWeight() const;
    std::string GetDistrict() const;

    void SetId(int newId);
    void SetAddress(const std::string& newAddress);
    void SetWeight(double newWeight);
    void SetDistrict(const std::string& newDistrict);

    std::string GetFullInfo() const;
    void PrintFullInfo() const;
};