#include "Logger.h"
#include "TimeUtils.h"
#include <fstream>
#include <iostream>

using namespace std;

static string make_single_line(string text);

Logger::Logger(const string& filename) : filename(filename), failure_reported(false) {}

void Logger::log(const string& event, const string& details) const
{
    ofstream file(filename, ios::app);
    if (file)
    {
        file << get_current_timestamp() << " | " << event;
        if (!details.empty()) file << " | " << make_single_line(details);
        file << '\n';
        file.close();
    }

    if (!file && !failure_reported)
    {
        failure_reported = true;
        cerr << "Предупреждение: не удалось записать в журнал \"" << filename << "\"." << endl;
    }
}

static string make_single_line(string text)
{
    for (char& symbol : text)
        if (symbol == '\n' || symbol == '\r') symbol = ' ';

    return text;
}