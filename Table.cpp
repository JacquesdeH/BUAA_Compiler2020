//
// Created by JacquesdeH on 2020/10/2.
//

#include "Table.h"

symbol::Table::Table()
{
    this->data.clear();
}

bool symbol::Table::hasKey(const string &_key) const
{
    return data.end() != data.find(_key);
}
