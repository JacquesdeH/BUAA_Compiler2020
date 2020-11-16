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

namespace inter
{
    class MIR
    {
    private:
        bool sealed;
        std::map<std::string, std::string> strings; // strName -> strValue
        std::map<std::string, std::pair<int, std::vector<char> > > globalChars; // name -> initList
        std::map<std::string, std::pair<int, std::vector<int> > > globalInts; // name -> initList
        std::vector<inter::Proc> procedures; // no global proc used

    public:
        MIR(const bool & _sealed = false);

    private:
        std::map<std::string, mips::SymbolInfo> getGlobalSymbols() const;

    public:
        void declareString(const std::string & _strName, const std::string & _strContent);
        void declareGlobalChars(const std::string & _name, const int & _count, const std::vector<char> & _initValues = {});
        void declareGlobalInts(const std::string & _name, const int & _count, const std::vector<int> & _initValues = {});
        void newProc();
        void doneGenerationToBlocks();
        void addQuad(const Quad & _quad);
        void assertSeal() const;
        std::map<std::string, std::string> queryStrings();
        std::vector<inter::Proc> queryProcs();
        mips::ObjCodes compileStrings() const;
        mips::ObjCodes compileGlobals() const;
        mips::ObjCodes compileProcs() const;
    };
}


#endif //PROJECT_UTILS_INTER_MIR_H
