#pragma once
#include <string>
#include <exception>
#include "Logger.h"

inline constexpr const char* DATA_FILE_NAME = "delivery_state.txt";
inline constexpr const char* LOG_FILE_NAME = "delivery.log";
inline constexpr const char* REPORT_FILE_NAME = "delivery_report.txt";

extern Logger logger;

void report_exception(std::exception_ptr ep);
int read_choice(const std::string& prompt, int min_value, int max_value);