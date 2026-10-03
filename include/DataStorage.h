#pragma once
#include <string>
#include "DeliveryService.h"

class DataStorage
{
private:
    std::string filename;

public:
    explicit DataStorage(const std::string& filename);

    void save(const DeliveryService& service) const;
    DeliveryService load() const;
};