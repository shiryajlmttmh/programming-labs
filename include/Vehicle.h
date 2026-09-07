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
    int currentOrderId;

    static constexpr double MAX_MOTORCYCLE_CAPACITY = 50.0;
    static constexpr double MAX_CAR_CAPACITY = 1500.0;

    bool IsValidType(const std::string& checkType) const;
    double GetMaxCapacityForType(const std::string& checkType) const;

public:
    Vehicle(int id, const std::string& type, double capacity, const std::string& courierName, bool isAvailable = true);

    bool AssignOrder(const Order& order);
    void CompleteDelivery();

    int GetId() const;
    std::string GetType() const;
    double GetCapacity() const;
    std::string GetCourierName() const;
    bool GetIsAvailable() const;
    int GetCurrentOrderId() const;

    void SetId(int newId);
    void SetType(const std::string& newType);
    void SetCapacity(double newCapacity);
    void SetCourierName(const std::string& newCourierName);
    void SetIsAvailable(bool newStatus);

    std::string GetFullInfo() const;
    void PrintFullInfo() const;
};