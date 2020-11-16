//
// Created by JacquesdeH on 2020/11/15.
//

#ifndef PROJECT_CORE_MIPS_MIPS_H
#define PROJECT_CORE_MIPS_MIPS_H

#include <vector>
#include <string>

#include "core_mips_ObjCodes.h"
#include "utils_inter_MIR.h"

namespace mips
{
    class Mips
    {
    private:

    public:
        Mips();

    public:
        static mips::ObjCodes compile(const inter::MIR & mir);
    };
}


#endif //PROJECT_CORE_MIPS_MIPS_H
