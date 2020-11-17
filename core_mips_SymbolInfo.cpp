//
// Created by JacquesdeH on 2020/11/16.
//

#include "core_mips_SymbolInfo.h"

mips::SymbolInfo::SymbolInfo(const int &_memOffset, const int &_atomSize)
{
    this->memOffset = _memOffset;
    this->atomSize = _atomSize;
}

int mips::SymbolInfo::getMemOffset() const
{
    return memOffset;
}

int mips::SymbolInfo::getAtomSize() const
{
    return atomSize;
}
