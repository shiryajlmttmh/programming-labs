#pragma once
#include <iostream>
#include <string>

class Order; 

class Vehicle
{
private:
    int id;
    std::string type;
    double capacity;
    std::string courierName;
    bool isAvailable;
public:
    Vehicle(int id, const std::string& type, double capacity, const std::string& courierName, bool isAvailable);

    bool AssignOrder(const Order& order);
    void CompleteDelivery(const Order& order);

    int GetId() const;
    std::string GetType() const;
    double GetCapacity() const;
    std::string GetCourierName() const;
    bool GetIsAvailable() const;

    void SetId(int newId);
    void SetType(const std::string& newType);
    void SetCapacity(double newCapacity);
    void SetCourierName(const std::string& newCourierName);
    void SetIsAvailable(bool newStatus);

    std::string GetFullInfo() const;
    void PrintFullInfo() const;
};