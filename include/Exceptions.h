#pragma once
#include <stdexcept>
#include <string>

class DeliveryException : public std::runtime_error
{
public:
    explicit DeliveryException(const std::string& msg) : std::runtime_error(msg) {}
};

class InvalidDataException : public DeliveryException
{
public:
    explicit InvalidDataException(const std::string& msg) : DeliveryException(msg) {}
};

class NotFoundException : public DeliveryException
{
    int id;
public:
    NotFoundException(const std::string& entity, int id)
        : DeliveryException(entity + " с номером " + std::to_string(id) + " не найден"), id(id) {}
    int get_id() const { return id; }
};

class DuplicateIdException : public DeliveryException
{
    int id;
public:
    DuplicateIdException(const std::string& entity, int id)
        : DeliveryException(entity + " с номером " + std::to_string(id) + " уже существует"), id(id) {}
    int get_id() const { return id; }
};

class ConstraintViolationException : public DeliveryException
{
public:
    explicit ConstraintViolationException(const std::string& msg) : DeliveryException(msg) {}
};

class InvalidOperationException : public DeliveryException
{
public:
    explicit InvalidOperationException(const std::string& msg) : DeliveryException(msg) {}
};

class FileOpenException : public DeliveryException
{
    std::string filename;
public:
    FileOpenException(const std::string& filename, const std::string& purpose)
        : DeliveryException("Не удалось открыть файл \"" + filename + "\" для " + purpose), filename(filename) {}
    const std::string& get_filename() const { return filename; }
};

class FileIOException : public DeliveryException
{
    std::string filename;
public:
    FileIOException(const std::string& filename, const std::string& operation)
        : DeliveryException("Ошибка " + operation + " файла \"" + filename + "\""), filename(filename) {}
    const std::string& get_filename() const { return filename; }
};

class FileFormatException : public DeliveryException
{
    int line;
public:
    FileFormatException(int line, const std::string& msg)
        : DeliveryException("Ошибка формата данных в строке " + std::to_string(line) + ": " + msg), line(line) {}
    int get_line() const { return line; }
};