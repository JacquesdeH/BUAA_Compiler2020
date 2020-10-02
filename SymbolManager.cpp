//
// Created by JacquesdeH on 2020/10/2.
//

#include "SymbolManager.h"

symbol::SymbolManager::SymbolManager()
{
    this->tables.clear();
    this->tables.emplace_back();
    this->curTable = 0;
}

bool symbol::SymbolManager::hasSymbolInScope(const string &symbol)
{
    Table& table = tables[curTable];
    return table.hasKey(symbol);
}
