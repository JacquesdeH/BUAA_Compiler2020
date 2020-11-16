//
// Created by JacquesdeH on 2020/11/15.
//

#ifndef PROJECT_CORE_MIPS_LOCALREGPOOL_H
#define PROJECT_CORE_MIPS_LOCALREGPOOL_H

#include <queue>
#include <string>
#include <set>
#include <map>
#include <initializer_list>

#include "core_mips_ObjCodes.h"
#include "SymbolInfo.h"
#include "functional_convert.h"

namespace mips
{
    class LocalRegPool
    {
    private:
        std::queue <std::string> freePool;
        std::queue <std::string> allocPool;
        std::set <std::string> writebackRegs;
        std::unordered_map<std::string, std::string> regs2var;
        std::unordered_map<std::string, std::string> var2regs;

    public:
        LocalRegPool();

    public:
        bool hasFree() const;
        void insertFree(const std::string & _reg);
        std::string queryReg2Var(const std::string & _var);
        std::string queryVar2Reg(const std::string & _reg);
        mips::ObjCodes allocReg(std::string & ret, const std::map<std::string, mips::SymbolInfo> & mipsTable,
                             const std::set<std::string> & excludeRegs = {});
        void markWriteBack(const std::string &_reg);
        mips::ObjCodes saveWriteBackRegs(const std::map<std::string, mips::SymbolInfo> & mipsTable);
        mips::ObjCodes writeBack(const std::string & _reg, const std::map<std::string, mips::SymbolInfo> & mipsTable);
        void updateInfo(const std::string & _reg, const std::string & _var);
    };
}


#endif //PROJECT_CORE_MIPS_LOCALREGPOOL_H
