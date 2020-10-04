//
// Created by JacquesdeH on 2020/10/2.
//

#include <iostream>

#include "symbol_Table.h"

symbol::Table::Table()
{
    this->data.clear();
}

bool symbol::Table::hasKey(const string &_key) const
{
    return data.end() != data.find(_key);
}

bool symbol::Table::insertRecord(const string &key, const symbol::Info &info)
{
    if (hasKey(key))
    {
        // Error!
        return false;
    }
    data[key] = info;
    return true;
}

symbol::Info symbol::Table::getInfo(const string &key) const
{
    if (!hasKey(key))
    {
        std::cerr << "Missing key of " << key << " in Table::getInfo()!" << std::endl;
    }
    return data.at(key);
}
