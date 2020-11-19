//
// Created by JacquesdeH on 2020/10/2.
//

#include <iostream>

#include "symbol_SymbolManager.h"
#include "functional_strext.h"

symbol::SymbolManager::SymbolManager()
{
    this->tables.clear();
    this->tables.emplace_back();
    this->curTable = 0;
}

bool symbol::SymbolManager::hasSymbolInScope(const string &symbol) const
{
    return tables[curTable].hasKey(toLower(symbol));
}

bool symbol::SymbolManager::hasSymbolInAll(const string &symbol) const
{
    string lowerSymbol = toLower(symbol);
    int pTable = curTable;
    while (pTable >= 0)
    {
        if (tables[pTable].hasKey(lowerSymbol))
            return true;
        pTable--;
    }
    return false;
}

bool symbol::SymbolManager::declareSymbol(const string& symbol, const symbol::Info &info)
{
    string lowerSymbol = toLower(symbol);
    if (hasSymbolInScope(lowerSymbol))
    {
        std::cerr << "Declaring existing idenfr in scope dup definition" << std::endl;
        return false;
    }
    tables[curTable].insertRecord(lowerSymbol, info);
    return true;
}

symbol::Info& symbol::SymbolManager::getInfoInAll(const string &symbol) const
{
    string lowerSymbol = toLower(symbol);
    static Info ret(config::SymbolType::SYMBOL_DEFAULT, config::DataType::DATA_DEFAULT, 0);
    int pTable = curTable;
    while (pTable >= 0)
    {
        if (tables[pTable].hasKey(lowerSymbol))
        {
            return tables[pTable].getInfo(lowerSymbol);
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

void symbol::SymbolManager::pushNewScope()
{
    tables.emplace_back();
    curTable++;
}

void symbol::SymbolManager::popCurScope()
{
    tables.erase(tables.begin() + curTable);
    curTable--;
}

symbol::Info &symbol::SymbolManager::getInfoFromLastScope(const string &symbol) const
{
    string lowerSymbol = toLower(symbol);
    static Info ret(config::SymbolType::SYMBOL_DEFAULT, config::DataType::DATA_DEFAULT, 0);
    int pTable = curTable - 1;
    while (pTable >= 0)
    {
        if (tables[pTable].hasKey(lowerSymbol))
        {
            return tables[pTable].getInfo(lowerSymbol);
            break;
        }
        pTable--;
    }
    if (pTable < 0)
    {
        // Error!
        std::cerr << "Unable to find " << symbol << " in getInfoFromLastScope() " << std::endl;
    }
    return ret;
}
