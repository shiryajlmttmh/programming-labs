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
    bool isAssigned;

public:
    Order(int id, const std::string& address, double weight, const std::string& district);

    int GetId() const;
    std::string GetAddress() const;
    double GetWeight() const;
    std::string GetDistrict() const;
    bool GetIsAssigned() const;

    void SetId(int newId);
    void SetAddress(const string& newAddress);
    void SetWeight(double newWeight);
    void SetDistrict(const string& newDistrict);
    void SetIsAssigned(bool status);

    std::string GetFullInfo() const;
    void PrintFullInfo() const;
};