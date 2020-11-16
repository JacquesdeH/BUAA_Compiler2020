//
// Created by JacquesdeH on 2020/11/11.
//

#ifndef PROJECT_UTILS_INTER_BLOCK_H
#define PROJECT_UTILS_INTER_BLOCK_H

#include <vector>
#include <map>
#include <string>

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
        std::vector<inter::Quad> queryLines() const;
        mips::ObjCodes compile(std::map<std::string, mips::SymbolInfo> & mipsTable) const;
    };
}


#endif //PROJECT_UTILS_INTER_BLOCK_H
