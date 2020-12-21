//
// Created by JacquesdeH on 2020/12/21.
//

#include "core_mips_ProcRegBook.h"

mips::ProcRegBook::ProcRegBook()
{
    this->procWideMarks.clear();
}

void mips::ProcRegBook::reset()
{
    this->procWideMarks.clear();
}

bool mips::ProcRegBook::isProcWide(const std::string &_mark)
{
    return procWideMarks.find(_mark) != procWideMarks.end();
}

void mips::ProcRegBook::insertMarkAsProcWide(const std::string &_mark)
{
    procWideMarks.insert(_mark);
}

void mips::ProcRegBook::insertMarksAsProcWide(const std::set<std::string> &_marks)
{
    for (const std::string &_mark : _marks)
    {
        insertMarkAsProcWide(_mark);
    }
}
