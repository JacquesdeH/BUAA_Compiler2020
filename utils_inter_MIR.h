//
// Created by JacquesdeH on 2020/11/11.
//

#ifndef PROJECT_UTILS_INTER_MIR_H
#define PROJECT_UTILS_INTER_MIR_H

#include <string>
#include <vector>
#include <map>
#include <initializer_list>

#include "utils_inter_Proc.h"
#include "utils_inter_Quad.h"
#include "core_mips_ObjCodes.h"
#include "utils_inter_typedef.h"

namespace inter
{
    class MIR
    {
    private:
        bool sealed;
        MapDeclareString globalStrings; // strName -> strValue
        MapDeclareChar globalChars; // name -> initList
        MapDeclareInt globalInts; // name -> initList
        std::vector<inter::Proc> procedures; // no global proc used

    public:
        MIR(const bool & _sealed = false);

    public:
        const MapDeclareString &queryGlobalStrings() const;
        const MapDeclareChar &queryGlobalChars() const;
        const MapDeclareInt &queryGlobalInts() const;
        std::vector<inter::Proc> queryFunctions() const;
        const inter::Proc &queryMainProc() const;

    public:
        void declareGlobalString(const std::string & _strName, const std::string & _strContent);
        void declareGlobalChar(const std::string & _name, const int & _count, const std::vector<char> & _initValues = {});
        void declareGlobalInt(const std::string & _name, const int & _count, const std::vector<int> & _initValues = {});
        void declareLocalChar(const std::string & name, const InitChar & initChar);
        void declareLocalInt(const std::string & name, const InitInt & initInt);
        void newProc(const std::string & _procName);
        void doneGenerationToBlocks();
        void addQuad(const Quad & _quad);
        void assertNotSeal() const;
        void assertIsSeal() const;
    };
}


#endif //PROJECT_UTILS_INTER_MIR_H
