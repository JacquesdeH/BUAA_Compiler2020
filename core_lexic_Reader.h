//
// Created by JacquesdeH on 2020/9/25.
//

#ifndef PROJECT_CORE_LEXIC_READER_H
#define PROJECT_CORE_LEXIC_READER_H

#include <string>
#include <queue>
#include <fstream>
#include <iostream>

#include "config.h"

using std::string;
using std::queue;

namespace lexic
{
    class Reader
    {
    private:
        std::ifstream fsIn;
        queue<char> buffer;
        int row;
        int column;

    public:
        Reader(const string& fIn);
        ~Reader();

    public:
        char next();
        int getRow() const;
        int getColumn() const;
    };
}


#endif //PROJECT_CORE_LEXIC_READER_H
