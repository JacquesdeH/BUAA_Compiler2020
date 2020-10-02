//
// Created by JacquesdeH on 2020/10/2.
//

#ifndef PROJECT_TABLE_H
#define PROJECT_TABLE_H

#include <unordered_map>
#include <string>

#include "Info.h"

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
    };
}


#endif //PROJECT_TABLE_H
