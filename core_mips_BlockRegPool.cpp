//
// Created by JacquesdeH on 2020/11/15.
//

#include <iostream>
#include "config.h"
#include "core_mips_BlockRegPool.h"
#include "functional_strext.h"

using std::string;

mips::BlockRegPool::BlockRegPool()
{
    for (const string & _reg : config::InitLocalRegs)
    {
        this->freePool.push(_reg);
    }
}

bool mips::BlockRegPool::hasFree() const
{
    return !(this->freePool.empty());
}

void mips::BlockRegPool::insertFree(const string &_reg)
{
    this->freePool.push(_reg);
}

std::string mips::BlockRegPool::queryReg2Mark(const string &_reg)
{
    if (reg2mark.find(_reg) == reg2mark.end())
    {
        std::cerr << "Unable to find key in regs2var!" << std::endl;
        return "";
    }
    return reg2mark.at(_reg);
}

std::string mips::BlockRegPool::queryMark2Reg(const string &_mark)
{
    if (mark2reg.find(_mark) == mark2reg.end())
    {
        std::cerr << "Unable to find key in var2regs!" << std::endl;
        return "";
    }
    return mark2reg.at(_mark);
}

mips::ObjCodes mips::BlockRegPool::allocBlockReg(std::string & _reg, const std::map<std::string, mips::SymbolInfo> & mipsTable,
                                                 const std::set<std::string> & _excludeRegs)
{
    mips::ObjCodes objCodes;
    bool writeback;
    if (!freePool.empty())
    {
        writeback = false;
        _reg = freePool.front();
        freePool.pop();
        allocPool.push(_reg);
        if (writebackRegs.find(_reg) != writebackRegs.end())
        {
            std::cerr << "Unexpected writeback status for register " + _reg << std::endl;
        }
    }
    else
    {
        writeback = true;
        do
        {
            _reg = allocPool.front();
            allocPool.pop();
            allocPool.push(_reg);
        } while (_excludeRegs.find(_reg) != _excludeRegs.end());
        if (writebackRegs.find(_reg) != writebackRegs.end())
        {
            // only writeback when modified
            objCodes.mergeCodes(_writeBack(_reg, mipsTable));
        }
    }
    return objCodes;
}

void mips::BlockRegPool::updateInfo(const string &_reg, const string &_mark)
{
    if (reg2mark.find(_reg) != reg2mark.end())
    {
        std::string _lastMark = reg2mark.at(_reg);
        if (mark2reg.find(_lastMark) != mark2reg.end())
            mark2reg.erase(_lastMark);
    }
    reg2mark[_reg] = _mark;
    mark2reg[_mark] = _reg;
}

void mips::BlockRegPool::markWriteBack(const string &_reg)
{
    if (reg2mark.find(_reg) == reg2mark.end())
        return;
    if (config::isNumeric(reg2mark.at(_reg)))
        _untieLinks(_reg);
    else
        this->writebackRegs.insert(_reg);
}

mips::ObjCodes mips::BlockRegPool::saveWriteBackRegs(const std::map<std::string, mips::SymbolInfo> & mipsTable)
{
    mips::ObjCodes ret;
    while (!writebackRegs.empty())
    {
        // only writeback when not a temp var， included below
        mips::ObjCodes tmp = _writeBack(*(writebackRegs.begin()), mipsTable, true);
        ret.mergeCodes(tmp);
    }
    return ret;
}

mips::ObjCodes mips::BlockRegPool::_writeBack(const string &_reg, const std::map<std::string, mips::SymbolInfo> &mipsTable,
                                              const bool &_readonly)
{
    // write back DO include map update of erase and need of writeback set
    if (writebackRegs.find(_reg) == writebackRegs.end())
        std::cerr << "Writing back not needed reg !" << std::endl;
    mips::ObjCodes ret;
    std::string _mark = reg2mark[_reg];
    // special early exit when writing back a temp reg
    if (_canEarlyStopInWriteBack(_mark))
    {
        if (!_readonly)
            _untieLinks(_reg);
        writebackRegs.erase(_reg);
        return ret;
    }
    // gen write back code
    if (config::isGlobal(_mark) || config::isLocal(_mark) || config::isTemp(_mark))
    {
        // ensures Global Local Temp
        int memOff = mipsTable.at(_mark).getMemOffset();
        int atomSize = mipsTable.at(_mark).getAtomSize();
        if (config::isGlobal(_mark))
            ret.genCodeInsert(atomSize2Store(atomSize), _reg, config::globalReg, toString(memOff));
        else if (config::isLocal(_mark) || config::isTemp(_mark))
            ret.genCodeInsert(atomSize2Store(atomSize), _reg, config::stackReg, toString(memOff));
        else
            std::cerr << "Unexpected _mark in _writeBack !" << std::endl;
    }
    // update
    if (!_readonly)
        _untieLinks(_reg);
    writebackRegs.erase(_reg);
    return ret;
}

bool mips::BlockRegPool::hasMark(const string &_mark) const
{
    return mark2reg.find(_mark) != mark2reg.end();
}

void mips::BlockRegPool::reset()
{
    // freePool
    while (!this->freePool.empty())
        this->freePool.pop();
    // allocPool
    while (!this->allocPool.empty())
        this->allocPool.pop();
    // two maps and one set
    this->reg2mark.clear();
    this->mark2reg.clear();
    this->writebackRegs.clear();

    // restart init
    for (const string & _reg : config::InitLocalRegs)
    {
        this->freePool.push(_reg);
    }
}

void mips::BlockRegPool::_untieLinks(const string &_reg)
{
    std::string _mark = reg2mark.at(_reg);
    reg2mark.erase(_reg);
    mark2reg.erase(_mark);
}

mips::ObjCodes mips::BlockRegPool::syncLink(const string &_reg, const string &_mark, const bool &_link,
                                            const std::map<std::string, mips::SymbolInfo> & mipsTable)
{
    mips::ObjCodes ret;
    if (_link && !hasMark(_mark))
    {
        updateInfo(_reg, _mark);
    }
    else if (!_link && hasMark(_mark))
    {
        if (writebackRegs.find(_reg) != writebackRegs.end())
        {
            ret.mergeCodes(_writeBack(_reg, mipsTable));
        }
        else
        {
            _untieLinks(_reg);
        }
    }
    return ret;
}

void mips::BlockRegPool::initLinkWithProcRegBook(mips::ProcRegBook *_procRegBook)
{
    this->procRegBook = _procRegBook;
}

bool mips::BlockRegPool::_canEarlyStopInWriteBack(const string &_mark)
{
    if (!config::isTemp(_mark))
        return false;
    if (procRegBook->isProcWide(_mark))
        return false;
    return true;
}
