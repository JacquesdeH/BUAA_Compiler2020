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

std::vector<inter::Block> inter::Proc::queryBlocks()
{
    assertBlockForm();
    return this->blocks;
}

mips::ObjCodes inter::Proc::compile(const std::map<std::string, mips::SymbolInfo> & globalSymbols) const
{
    assertBlockForm();
    mips::ObjCodes ret;
    std::map<std::string, mips::SymbolInfo> mipsTable;
    mipsTable.insert(globalSymbols.begin(), globalSymbols.end());
    // alloc mem
    int offset = 0;
    for (const auto & block : blocks)
    {
        for (const auto & line : block.queryLines())
        {
            if (line.getOp() == config::CONST_IR || line.getOp() == config::VAR_IR)
            {
                int addr = offset;
                offset += str2int(line.getInr()) * (4);
                mipsTable.insert(std::make_pair(line.getOut(), mips::SymbolInfo(addr, (line.getInl() == "int" ? 4 : 1))));
            }
        }
        for (const auto & line : block.queryLines())
        {
            if (config::isTemp(line.getOut()))
            {
                const std::string& temp = line.getOut();
                int addr = offset;
                offset += 4;
                mipsTable.insert(std::make_pair(temp, mips::SymbolInfo(addr, 4)));
            }
        }
    }
    ret.genCodeInsert("subu", "$sp", "$sp", toString(offset));
    for (const auto & block : blocks)
    {
        mips::ObjCodes tmp = block.compile(mipsTable);
        ret.mergeCodes(tmp);
    }
    ret.genCodeInsert("addu", "$sp", "$sp", toString(offset));
    return ret;
}

void inter::Proc::assertBlockForm() const
{
    if (!isBlockForm)
        std::cerr << "Not a block form procedure !" << std::endl;
}
