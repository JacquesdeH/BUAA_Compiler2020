//
// Created by JacquesdeH on 2020/10/2.
//

#ifndef PROJECT_SYMBOL_TABLE_H
#define PROJECT_SYMBOL_TABLE_H

#include <unordered_map>
#include <string>

#include "symbol_Info.h"

using std::unordered_map;
using std::string;

namespace symbol
{
    class Table
    {
    private:
        unordered_map<string, Info> data;

    public:
        Table();

    public:
        bool hasKey(const string& _key) const;
        Info getInfo(const string& key) const;
        bool insertRecord(const string& key, const Info& info);
    };
}


#endif //PROJECT_SYMBOL_TABLE_H
