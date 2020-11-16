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

std::string mips::LocalRegPool::allocReg(bool &writeback, const std::set<std::string> & excludeRegs)
{
    // TODO: mips new alloc strategies required
    string ret;
    if (!freePool.empty())
    {
        writeback = false;
        ret = freePool.front();
        freePool.pop();
        allocPool.push(ret);
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
    }
    return ret;
}

void mips::LocalRegPool::updateInfo(const string &_reg, const string &_var)
{
    regs2var[_reg] = _var;
    var2regs[_var] = _reg;
}
