//
// Created by JacquesdeH on 2020/11/11.
//

#ifndef PROJECT_UTILS_INTER_BLOCK_H
#define PROJECT_UTILS_INTER_BLOCK_H

#include <vector>

#include "utils_inter_Quad.h"
#include "core_mips_ObjCodes.h"

namespace inter
{
    class Block
    {
    private:
        std::vector<inter::Quad> lines;

    public:
        Block(std::vector<inter::Quad>::iterator _begin, std::vector<inter::Quad>::iterator _end);

    public:
        std::vector<inter::Quad> queryLines();
        mips::ObjCodes compile() const;
    };
}


#endif //PROJECT_UTILS_INTER_BLOCK_H
