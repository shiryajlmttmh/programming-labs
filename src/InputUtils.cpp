#include "InputUtils.h"
#include <iostream>

using namespace std;

static void clear_buffer();

int read_int(const string& prompt)
{
    int value;
    while (true)
    {
        cout << prompt;
        if (cin >> value)
        {
            char next_char = cin.get();

            while (next_char == ' ' || next_char == '\t') next_char = cin.get();

            if (next_char == '\n' || next_char == EOF) return value;
        }
        else cin.clear();

        clear_buffer();
        cout << "Ошибка: введите корректное целое число без посторонних символов!" << endl;
    }
}

double read_double(const string& prompt)
{
    double value;
    while (true)
    {
        cout << prompt;
        if (cin >> value)
        {
            char next_char = cin.get();

            while (next_char == ' ' || next_char == '\t') next_char = cin.get();

            if (next_char == '\n' || next_char == EOF) return value;
        }
        else cin.clear();

        clear_buffer();
        cout << "Ошибка: введите корректное число без посторонних символов!" << endl;
    }
}

static void clear_buffer()
{
    while (cin.get() != '\n' && cin);
}