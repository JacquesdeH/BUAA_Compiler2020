//
// Created by JacquesdeH on 2020/11/11.
//

#ifndef PROJECT_CORE_SEMANTIC_SEMANTIC_H
#define PROJECT_CORE_SEMANTIC_SEMANTIC_H

#include <string>

#include "utils_MIR.h"

namespace semantic
{
    class Semantic
    {
    private:
        bool errored;
        int tempIdx;
        int labelIdx;
        int stringIdx;
        inter::MIR mir;

    public:
        Semantic();

    public:
        void error();
        bool noError() const;
        std::string genTemp();
        std::string genLabel();
        std::string genString(const std::string & _strContent);
        void addMIR(const config::IRCode & _op, const std::string & _out = "", const std::string & _inl = "", const std::string & _inr = "");
        void newProc();
        void doneGeneration();
    };
}


#endif //PROJECT_CORE_SEMANTIC_SEMANTIC_H
