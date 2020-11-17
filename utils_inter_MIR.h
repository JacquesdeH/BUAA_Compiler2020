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
    typedef std::map<std::string, std::pair<int, std::vector<char> > > MapGlobalChar;
    typedef std::map<std::string, std::pair<int, std::vector<int> > > MapGlobalInt;
    typedef std::map<std::string, std::string> MapGlobalString;
    typedef std::map<std::string, mips::SymbolInfo> MapGlobalInfo;

    class MIR
    {
    private:
        bool sealed;
        MapGlobalString globalStrings; // strName -> strValue
        MapGlobalChar globalChars; // name -> initList
        MapGlobalInt globalInts; // name -> initList
        std::vector<inter::Proc> procedures; // no global proc used

    public:
        MIR(const bool & _sealed = false);

    public:
        const MapGlobalString &queryGlobalStrings() const;
        const MapGlobalChar &queryGlobalChars() const;
        const MapGlobalInt &queryGlobalInts() const;
        std::vector<inter::Proc> queryFunctions() const;
        const inter::Proc &queryMainProc() const;

    public:
        void declareGlobalString(const std::string & _strName, const std::string & _strContent);
        void declareGlobalChars(const std::string & _name, const int & _count, const std::vector<char> & _initValues = {});
        void declareGlobalInts(const std::string & _name, const int & _count, const std::vector<int> & _initValues = {});
        void newProc();
        void doneGenerationToBlocks();
        void addQuad(const Quad & _quad);
        void assertSeal() const;
    };
}


#endif //PROJECT_UTILS_INTER_MIR_H
