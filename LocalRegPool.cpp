//
// Created by JacquesdeH on 2020/11/15.
//

#include "config.h"
#include "LocalRegPool.h"

mips::LocalRegPool::LocalRegPool()
{
    for (const string & _reg : config::InitLocalRegs)
    {
        this->freeRegs.push(_reg);
    }
}
