//
// Created by JacquesdeH on 2020/11/11.
//

#include <iostream>
#include <map>
#include <string>

#include "utils_inter_Proc.h"
#include "functional_strext.h"

inter::Proc::Proc(const bool &_isBlockForm)
{
    this->isBlockForm = _isBlockForm;
}

void inter::Proc::addQuad(const inter::Quad &_quad)
{
    if (isBlockForm)
        std::cerr << "Should not modify Proc contents with block form !" << std::endl;
    lines.push_back(_quad);
}

void inter::Proc::buildBlocks()
{
    // TODO: build by control flows
    // Building simply
    {
        Block _block(lines.begin(), lines.end());
        blocks.push_back(_block);
    }
    this->isBlockForm = true;
}

void inter::Proc::assertBlockForm() const
{
    if (!isBlockForm)
        std::cerr << "Not a block form procedure !" << std::endl;
}

const std::vector<inter::Quad> &inter::Proc::queryLines() const
{
    return lines;
}

const std::vector<inter::Block> &inter::Proc::queryBlocks() const
{
    assertBlockForm();
    return blocks;
}
