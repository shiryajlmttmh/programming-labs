#pragma once
#include <string>
#include "DeliveryService.h"

class ReportWriter
{
private:
    std::string filename;

public:
    explicit ReportWriter(const std::string& filename);

    void write(const DeliveryService& service) const;
};