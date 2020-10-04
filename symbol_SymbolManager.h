//
// Created by JacquesdeH on 2020/10/2.
//

#ifndef PROJECT_SYMBOL_SYMBOLMANAGER_H
#define PROJECT_SYMBOL_SYMBOLMANAGER_H

#include <vector>

#include "symbol_Table.h"

using std::vector;

namespace symbol
{
    class SymbolManager
    {
    private:
        vector<Table> tables;
        int curTable;

    public:
        SymbolManager();

    public:
        bool hasSymbolInScope(const string& symbol) const;
        bool hasSymbolInAll(const string& symbol) const;
        Info getInfoInAll(const string& symbol) const;
        bool declareSymbol(const string& symbol, const Info& info);
    };
}


#endif //PROJECT_SYMBOL_SYMBOLMANAGER_H
