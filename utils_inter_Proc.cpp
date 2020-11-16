//
// Created by JacquesdeH on 2020/11/11.
//

#include <iostream>
#include "utils_inter_Proc.h"

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

std::vector<inter::Block> inter::Proc::queryBlocks()
{
    assertBlockForm();
    return this->blocks;
}

mips::ObjCodes inter::Proc::compile() const
{
    assertBlockForm();
    mips::ObjCodes ret;
    for (const auto & block : blocks)
    {
        mips::ObjCodes tmp = block.compile();
        ret.mergeCodes(tmp);
    }
    return ret;
}

void inter::Proc::assertBlockForm() const
{
    if (!isBlockForm)
        std::cerr << "Not a block form procedure !" << std::endl;
}
