#include "UICommon.h"
#include "InputUtils.h"
#include "Exceptions.h"

#include <iostream>
#include <stdexcept>
#include <exception>
#include <string>

using namespace std;

Logger logger(LOG_FILE_NAME);

static void log_exception(exception_ptr ep);

void report_exception(exception_ptr ep)
{
    log_exception(ep);

    try
    {
        if (ep) rethrow_exception(ep);
    }
    catch (const NotFoundException& e)
    {
        cout << "Ошибка поиска: " << e.what() << ". Проверьте номер (" << e.get_id() << ")." << endl;
    }
    catch (const DuplicateIdException& e)
    {
        cout << "Ошибка уникальности: " << e.what() << ". Выберите другой номер." << endl;
    }
    catch (const InvalidDataException& e)
    {
        cout << "Некорректные данные: " << e.what() << "." << endl;
    }
    catch (const ConstraintViolationException& e)
    {
        cout << "Нарушено ограничение: " << e.what() << "." << endl;
    }
    catch (const InvalidOperationException& e)
    {
        cout << "Недопустимая операция: " << e.what() << "." << endl;
    }
    catch (const FileOpenException& e)
    {
        cout << "Ошибка файла: " << e.what() << ". Проверьте путь и права доступа." << endl;
    }
    catch (const FileIOException& e)
    {
        cout << "Сбой при работе с файлом: " << e.what() << "." << endl;
    }
    catch (const FileFormatException& e)
    {
        cout << "Повреждённый файл данных: " << e.what() << "." << endl;
    }
    catch (const DeliveryException& e)
    {
        cout << "Ошибка службы доставки: " << e.what() << "." << endl;
    }
    catch (const out_of_range& e)
    {
        cout << "Выход за пределы допустимого диапазона: " << e.what() << "." << endl;
    }
    catch (const exception& e)
    {
        cout << "Непредвиденная ошибка: " << e.what() << "." << endl;
    }
    catch (...)
    {
        cout << "Неизвестная ошибка." << endl;
    }
}

static void log_exception(exception_ptr ep)
{
    try
    {
        if (ep) rethrow_exception(ep);
    }
    catch (const exception& e)
    {
        logger.log("ERROR", e.what());
    }
    catch (...)
    {
        logger.log("ERROR", "Неизвестная ошибка");
    }
}

int read_choice(const string& prompt, int min_value, int max_value)
{
    while (true)
    {
        int choice = read_int(prompt);
        if (choice >= min_value && choice <= max_value) return choice;
        cout << "Ошибка: введите число от " << min_value << " до " << max_value << "!" << endl;
    }
}