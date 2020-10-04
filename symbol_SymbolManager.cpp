//
// Created by JacquesdeH on 2020/10/2.
//

#include <iostream>

#include "symbol_SymbolManager.h"

symbol::SymbolManager::SymbolManager()
{
    this->tables.clear();
    this->tables.emplace_back();
    this->curTable = 0;
}

bool symbol::SymbolManager::hasSymbolInScope(const string &symbol) const
{
    return tables[curTable].hasKey(symbol);
}

bool symbol::SymbolManager::hasSymbolInAll(const string &symbol) const
{
    int pTable = curTable;
    while (pTable >= 0)
    {
        if (tables[pTable].hasKey(symbol))
            return true;
        pTable--;
    }
    return false;
}

bool symbol::SymbolManager::declareSymbol(const string& symbol, const symbol::Info &info)
{
    if (hasSymbolInScope(symbol))
    {
        // TODO: ErrorManager
        return false;
    }
    tables[curTable].insertRecord(symbol, info);
    return true;
}

symbol::Info symbol::SymbolManager::getInfoInAll(const string &symbol) const
{
    Info ret;
    int pTable = curTable;
    while (pTable >= 0)
    {
        if (tables[pTable].hasKey(symbol))
        {
            ret = tables[pTable].getInfo(symbol);
            break;
        }
        pTable--;
    }
    if (pTable < 0)
    {
        // Error!
        std::cerr << "Unable to find " << symbol << " in getInfoInAll() " << std::endl;
    }
    return ret;
}
