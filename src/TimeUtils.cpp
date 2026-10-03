#include "TimeUtils.h"
#include <chrono>
#include <format>
#include <exception>

using namespace std;

string get_current_timestamp()
{
    auto now = chrono::floor<chrono::seconds>(chrono::system_clock::now());

    try
    {
        chrono::zoned_time local_time(chrono::current_zone(), now);
        return format("{:%Y-%m-%d %H:%M:%S}", local_time);
    }
    catch (const exception&)
    {
        return format("{:%Y-%m-%d %H:%M:%S}", now) + " UTC";
    }
}