//
// Created by JacquesdeH on 2020/9/25.
//

#include "core_lexic_Reader.h"

#define DELIMITER '\n'

using std::getline;

lexic::Reader::Reader(const string &fIn)
{
    fsIn.open(fIn);
    while (!buffer.empty()) buffer.pop();
    row = config::ROW_INIT;
    column = config::COLUMN_INIT;
}

lexic::Reader::~Reader()
{
    fsIn.close();
}

char lexic::Reader::next()
{
    while (buffer.empty())
    {
        if (fsIn.eof())
            return EOF;
        string s;
        getline(fsIn, s, DELIMITER);
        for (char & c : s)
        {
            buffer.push(c);
        }
        row++;
        column = config::COLUMN_INIT;
    }
    char ret = buffer.front();
    buffer.pop();
    column++;
    return ret;
}

int lexic::Reader::getRow() const
{
    return row;
}

int lexic::Reader::getColumn() const
{
    return column;
}
