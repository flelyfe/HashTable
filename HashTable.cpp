#include "HashTable.h"
#include <iostream>

HashTable::HashTable(size_t size) noexcept
{
    _filled=0;
    _capacity=size > 0 ? size : 1;
    table.resize(_capacity);
}

HashTable::~HashTable()
{
    table.clear();
}

size_t HashTable::hash_function(const KeyType &key) const
{
    size_t summ = 0;
    for (const auto &i:key)
    {
        summ+=i;
    }

    return (summ %_capacity);
}

void HashTable::insert(const KeyType &key, const ValueType &value)
{
    ValueType t;
    if (find(key,t))
    {
        for (auto i:table[hash_function(key)])
        {
            if (key==i.first)
            {
                i.second=value;
                break;
            }
        }
    }
    else
    {
        table[hash_function(key)].push_back(std::make_pair(key, value));
        _filled++;
        if (getLoadFactor() > 0.75) 
        {
            _capacity *= 2;
            std::vector<std::list<std::pair<KeyType, ValueType>>> temp_table(_capacity);
            for (auto &list : table)
            {
                for (auto &pair : list)
                {
                    temp_table[hash_function(pair.first)].push_back(pair);
                }
            }
            table = std::move(temp_table);
        }
    }
}
    
bool HashTable::find(const KeyType &key, ValueType &value) const
{
    for (const auto &pair:table[hash_function(key)])
    {
        if (pair.first==key)
        {
            value=pair.second;
            return true;
        }
    }
    return false;
}

void HashTable::remove(const KeyType &key)
{
    for (auto i=table[hash_function(key)].begin();i!=table[hash_function(key)].end();++i)
    {
        if (key==i->first)
        {
            table[hash_function(key)].erase(i);
            _filled--;
            break;
        }
    }
}
ValueType& HashTable::operator[](const KeyType &key)
{
    for (auto i=table[hash_function(key)].begin();i!=table[hash_function(key)].end();i++)
    {
        if (key==i->first)
        {
            return i->second;
        }
    }
    insert(key, 0);
    return table[hash_function(key)].back().second;
}

double HashTable::getLoadFactor()
{
    return _capacity > 0 ? static_cast<double>(_filled) / _capacity : 0.0;
}