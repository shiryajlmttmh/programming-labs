#pragma once
#include <vector>
#include <iostream>
#include <stdexcept>

template <typename T>
class Collection
{
private:
    std::vector<T> items;

public:
    void add_item(const T& item);
    void remove_item_by_index(size_t index);

    const T& get_item_by_index(size_t index) const;
    T& get_item_by_index(size_t index);

    bool contains(const T& target) const;
    int find_index(const T& target) const;

    size_t get_items_count() const;
    void clear_collection();
    void print_collection() const;
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
    {
        std::cout << "Ошибка: индекс " << index << " выходит за пределы коллекции!" << std::endl;
        return;
    }

    items.erase(items.begin() + index);
}

template <typename T>
const T& Collection<T>::get_item_by_index(size_t index) const
{
    return items.at(index);
}

template <typename T>
T& Collection<T>::get_item_by_index(size_t index)
{
    return items.at(index);
}

template <typename T>
int Collection<T>::find_index(const T& target) const
{
    for (size_t i = 0; i < items.size(); i++)
        if (items[i] == target) return static_cast<int>(i);

    return -1;
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

    for (size_t i = 0; i < items.size(); i++)
    {
        std::cout << items[i] << std::endl;
    }
}

template <typename T, typename Predicate>
const T* find_if_matching(const Collection<T>& collection, Predicate predicate)
{
    for (size_t i = 0; i < collection.get_items_count(); i++)
    {
        const T& item = collection.get_item_by_index(i);
        if (predicate(item)) return &item;
    }
    return nullptr;
}