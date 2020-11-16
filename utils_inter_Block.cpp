//
// Created by JacquesdeH on 2020/11/11.
//

#include "utils_inter_Block.h"

inter::Block::Block(std::vector<inter::Quad>::iterator _begin, std::vector<inter::Quad>::iterator _end)
{
    for (auto iter = _begin; iter != _end; iter++)
    {
        this->lines.push_back(*iter);
    }
}

std::vector<inter::Quad> inter::Block::queryLines()
{
    return this->lines;
}

mips::ObjCodes inter::Block::compile(std::map<std::string, int> & memoryOffset) const
{
    mips::ObjCodes ret;
    mips::LocalRegPool localRegPool;
    for (const auto & line : lines)
    {
        mips::ObjCodes tmp = line.compile(localRegPool, memoryOffset);
        ret.mergeCodes(tmp);
    }
    return ret;
}