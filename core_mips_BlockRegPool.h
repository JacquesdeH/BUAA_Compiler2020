//
// Created by JacquesdeH on 2020/11/15.
//

#ifndef PROJECT_CORE_MIPS_BLOCKREGPOOL_H
#define PROJECT_CORE_MIPS_BLOCKREGPOOL_H

#include <queue>
#include <string>
#include <set>
#include <map>
#include <initializer_list>

#include "core_mips_ObjCodes.h"
#include "core_mips_SymbolInfo.h"
#include "functional_convert.h"

namespace mips
{
    class BlockRegPool
    {
    private:
        std::queue <std::string> freePool;
        std::queue <std::string> allocPool;
        std::set <std::string> writebackRegs;
        std::unordered_map<std::string, std::string> reg2mark;
        std::unordered_map<std::string, std::string> mark2reg;

    public:
        BlockRegPool();

    private:
        mips::ObjCodes _writeBack(const std::string & _reg, const std::map<std::string, mips::SymbolInfo> & mipsTable);
        void _untieLinks(const std::string & _reg);

    public:
        void reset();
        bool hasFree() const;
        void insertFree(const std::string & _reg);
        std::string queryReg2Mark(const std::string & _reg);
        std::string queryMark2Reg(const std::string & _mark);
        bool hasMark(const std::string & _mark) const;
        mips::ObjCodes allocBlockReg(std::string & _reg, const std::map<std::string, mips::SymbolInfo> & mipsTable,
                                     const std::set<std::string> & _excludeRegs = {});
        void markWriteBack(const std::string &_reg);
        mips::ObjCodes saveWriteBackRegs(const std::map<std::string, mips::SymbolInfo> & mipsTable);
        void updateInfo(const std::string & _reg, const std::string & _mark);
        mips::ObjCodes syncLink(const std::string &_reg, const std::string &_mark, const bool &_link,
                                const std::map<std::string, mips::SymbolInfo> & mipsTable);
    };
}


#endif //PROJECT_CORE_MIPS_BLOCKREGPOOL_H
