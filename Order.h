#pragma once
#include <iostream>
#include <string>

class Order
{
private:
    int id;
    std::string address;
    double weight;
    std::string district;

public:
    Order(int id, std::string address, double weight, std::string district);

    int GetId() const;
    std::string GetAddress() const;
    double GetWeight() const;
    std::string GetDistrict() const;

    void SetId(int newId);
    void SetAddress(std::string newAddress);
    void SetWeight(double newWeight);
    void SetDistrict(std::string newDistrict);

    std::string GetFullInfo() const;
    void PrintFullInfo() const;
};