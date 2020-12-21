//
// Created by JacquesdeH on 2020/12/21.
//

#ifndef PROJECT_CORE_MIPS_PROCREGBOOK_H
#define PROJECT_CORE_MIPS_PROCREGBOOK_H


#include <set>
#include <string>

namespace mips
{
    class ProcRegBook
    {
    private:
        std::set<std::string> procWideMarks;

    public:
        ProcRegBook();

    public:
        void reset();
        bool isProcWide(const std::string &_mark);
        void insertMarkAsProcWide(const std::string &_mark);
        void insertMarksAsProcWide(const std::set<std::string> &_marks);
    };
}


#endif //PROJECT_CORE_MIPS_PROCREGBOOK_H
