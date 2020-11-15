//
// Created by JacquesdeH on 2020/11/11.
//

#ifndef PROJECT_UTILS_MIR_H
#define PROJECT_UTILS_MIR_H

#include <string>
#include <vector>
#include <map>

#include "utils_Proc.h"
#include "utils_Quad.h"

namespace inter
{
    class MIR
    {
    private:
        bool sealed;
        std::map<std::string, std::string> strings;
        std::vector<inter::Proc> procedures; // [0] as global Proc

    public:
        MIR(const bool & _sealed = false);

    public:
        void declareString(const std::string & _strName, const std::string & _strContent);
        void newProc();
        void doneGeneration();
        void addQuad(const Quad & _quad);
        void assertSeal() const;
    };
}


#endif //PROJECT_UTILS_MIR_H
