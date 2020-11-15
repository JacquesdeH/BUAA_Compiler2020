//
// Created by JacquesdeH on 2020/11/15.
//

#ifndef PROJECT_CORE_MIPS_MIPS_H
#define PROJECT_CORE_MIPS_MIPS_H

#include <vector>
#include <string>

#include "core_mips_ObjCodes.h"

namespace mips
{
    class Mips
    {
    private:
        ObjCodes mipscode;

    public:
        Mips();

    public:
        void insertCodePiece(const ObjCodes & piece);
    };
}


#endif //PROJECT_CORE_MIPS_MIPS_H
