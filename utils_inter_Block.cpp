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

const std::vector<inter::Quad> & inter::Block::queryLines() const
{
    return this->lines;
}
