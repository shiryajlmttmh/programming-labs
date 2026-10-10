#pragma once
#include <vector>
#include <iostream>
#include <stdexcept>
#include <string>
#include <algorithm>
#include <iterator>

template <typename T>
class Collection
{
private:
    std::vector<T> items;

public:
    using iterator = typename std::vector<T>::iterator;
    using const_iterator = typename std::vector<T>::const_iterator;

    void add_item(const T& item);
    void remove_item_by_index(size_t index);

    const T& get_item_by_index(size_t index) const;
    T& get_item_by_index(size_t index);

    bool contains(const T& target) const;
    int find_index(const T& target) const;

    size_t get_items_count() const;
    void clear_collection();
    void print_collection() const;

    iterator begin() { return items.begin(); }
    iterator end() { return items.end(); }
    const_iterator begin() const { return items.begin(); }
    const_iterator end() const { return items.end(); }
};

template <typename T>
void Collection<T>::add_item(const T& item)
{
    items.push_back(item);
}

template <typename T>
void Collection<T>::remove_item_by_index(size_t index)
{
    if (index >= items.size())
        throw std::out_of_range("Индекс " + std::to_string(index) + " выходит за пределы коллекции (размер: "
            + std::to_string(items.size()) + ")");

    items.erase(items.begin() + index);
}

template <typename T>
const T& Collection<T>::get_item_by_index(size_t index) const
{
    if (index >= items.size())
        throw std::out_of_range("Индекс " + std::to_string(index) + " выходит за пределы коллекции (размер: "
            + std::to_string(items.size()) + ")");

    return items[index];
}

template <typename T>
T& Collection<T>::get_item_by_index(size_t index)
{
    if (index >= items.size())
        throw std::out_of_range("Индекс " + std::to_string(index) + " выходит за пределы коллекции (размер: "
            + std::to_string(items.size()) + ")");

    return items[index];
}

template <typename T>
int Collection<T>::find_index(const T& target) const
{
    auto it = std::find(items.begin(), items.end(), target);
    if (it == items.end()) return -1;

    return static_cast<int>(std::distance(items.begin(), it));
}

template <typename T>
bool Collection<T>::contains(const T& target) const
{
    return find_index(target) != -1;
}

template <typename T>
size_t Collection<T>::get_items_count() const
{
    return items.size();
}

template <typename T>
void Collection<T>::clear_collection()
{
    items.clear();
}

template <typename T>
void Collection<T>::print_collection() const
{
    if (items.empty())
    {
        std::cout << "Коллекция пуста." << std::endl;
        return;
    }

    std::for_each(items.begin(), items.end(),
        [](const T& item) { std::cout << item << std::endl; });
}

template <typename T, typename Predicate>
const T* find_if_matching(const Collection<T>& collection, Predicate predicate)
{
    auto it = std::find_if(collection.begin(), collection.end(), predicate);
    return it != collection.end() ? &*it : nullptr;
}