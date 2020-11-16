//
// Created by JacquesdeH on 2020/11/15.
//

#include <iostream>
#include "config.h"
#include "core_mips_LocalRegPool.h"

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

std::string mips::LocalRegPool::queryReg2Var(const string &_var)
{
    if (regs2var.find(_var) == regs2var.end())
    {
        std::cerr << "Unable to find key in regs2var!" << std::endl;
        return "";
    }
    return regs2var.at(_var);
}

std::string mips::LocalRegPool::queryVar2Reg(const string &_reg)
{
    if (var2regs.find(_reg) == var2regs.end())
    {
        std::cerr << "Unable to find key in var2regs!" << std::endl;
        return "";
    }
    return var2regs.at(_reg);
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
            // TODO
            // update writeback status for a new link of reg-var
            writebackRegs.erase(ret);
        }
        // un-tie original reg-var
        // TODO
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
    // TODO: mips save back wb regs!
    return mips::ObjCodes();
}

mips::ObjCodes mips::LocalRegPool::writeBack(const string &_reg, const std::map<std::string, mips::SymbolInfo> &mipsTable)
{
    // TODO: implement writeback single reg!
    return mips::ObjCodes();
}
