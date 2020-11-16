//
// Created by JacquesdeH on 2020/11/15.
//

#include <iostream>
#include "config.h"
#include "core_mips_LocalRegPool.h"
#include "functional_strext.h"

using std::string;

mips::LocalRegPool::LocalRegPool()
{
    for (const string & _reg : config::InitLocalRegs)
    {
        this->freePool.push(_reg);
    }
}

bool mips::LocalRegPool::hasFree() const
{
    return !(this->freePool.empty());
}

void mips::LocalRegPool::insertFree(const string &_reg)
{
    this->freePool.push(_reg);
}

std::string mips::LocalRegPool::queryReg2Var(const string &_reg)
{
    if (regs2var.find(_reg) == regs2var.end())
    {
        std::cerr << "Unable to find key in regs2var!" << std::endl;
        return "";
    }
    return regs2var.at(_reg);
}

std::string mips::LocalRegPool::queryVar2Reg(const string &_var)
{
    if (var2regs.find(_var) == var2regs.end())
    {
        std::cerr << "Unable to find key in var2regs!" << std::endl;
        return "";
    }
    return var2regs.at(_var);
}

mips::ObjCodes mips::LocalRegPool::allocReg(std::string & ret, const std::map<std::string, mips::SymbolInfo> & mipsTable,
                                         const std::set<std::string> & excludeRegs)
{
    // TODO: mips new alloc strategies required
    mips::ObjCodes objCodes;
    bool writeback;
    if (!freePool.empty())
    {
        writeback = false;
        ret = freePool.front();
        freePool.pop();
        allocPool.push(ret);
        if (writebackRegs.find(ret) != writebackRegs.end())
        {
            std::cerr << "Unexpected writeback status for register " + ret << std::endl;
        }
    }
    else
    {
        writeback = true;
        do
        {
            ret = allocPool.front();
            allocPool.pop();
            allocPool.push(ret);
        } while (excludeRegs.find(ret) != excludeRegs.end());
        if (writebackRegs.find(ret) != writebackRegs.end())
        {
            // only writeback when modified
            objCodes.mergeCodes(writeBack(ret, mipsTable));
            // update writeback status for a new link of reg-var
            writebackRegs.erase(ret);
        }
        // un-tie original reg-var
        std::string _name = regs2var[ret];
        regs2var.erase(ret);
        var2regs.erase(_name);
    }
    return objCodes;
}

void mips::LocalRegPool::updateInfo(const string &_reg, const string &_var)
{
    regs2var[_reg] = _var;
    var2regs[_var] = _reg;
}

void mips::LocalRegPool::markWriteBack(const string &_reg)
{
    this->writebackRegs.insert(_reg);
}

mips::ObjCodes mips::LocalRegPool::saveWriteBackRegs(const std::map<std::string, mips::SymbolInfo> & mipsTable)
{
    mips::ObjCodes ret;
    for (const auto & _reg : writebackRegs)
    {
        mips::ObjCodes tmp = writeBack(_reg, mipsTable);
        ret.mergeCodes(tmp);
    }
    return ret;
}

mips::ObjCodes mips::LocalRegPool::writeBack(const string &_reg, const std::map<std::string, mips::SymbolInfo> &mipsTable)
{
    if (writebackRegs.find(_reg) == writebackRegs.end())
        std::cerr << "Writing back not needed reg !" << std::endl;
    mips::ObjCodes ret;
    std::string _name = regs2var[_reg];
    int memOff = mipsTable.at(_name).getMemOffset();
    int atomSize = mipsTable.at(_name).getAtomSize();
    ret.genCodeInsert(atomSize2Store(atomSize), _reg, "$sp", toString(memOff));
    return ret;
}

bool mips::LocalRegPool::hasVar(const string &_var) const
{
    return var2regs.find(_var) != var2regs.end();
}
