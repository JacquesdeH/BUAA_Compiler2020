//
// Created by JacquesdeH on 2020/11/15.
//

#ifndef PROJECT_MIPS_H
#define PROJECT_MIPS_H

#include <vector>
#include <string>

#include "ObjCodes.h"

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


#endif //PROJECT_MIPS_H
