//
// Created by JacquesdeH on 2020/11/11.
//

#include <iostream>

#include "utils_inter_Proc.h"
#include "functional_strext.h"

inter::Proc::Proc(const std::string & _procName, const bool &_isBlockForm)
{
    this->isBlockForm = _isBlockForm;
    this->procName = _procName;
}

void inter::Proc::addQuad(const inter::Quad &_quad)
{
    if (isBlockForm)
        std::cerr << "Should not modify Proc contents with block form !" << std::endl;
    lines.push_back(_quad);
}

void inter::Proc::buildBlocks()
{
    std::set<int> blockHeads;
    // Rule 1
    blockHeads.insert(0);
    // Rule 2
    std::set<std::string> allLabels;
    for (const auto &_quad : lines)
    {
        if (config::isLabeledBranchJump(_quad.op))
            allLabels.insert(_quad.out);
    }
    for (int i = 0; i < lines.size(); ++i)
    {
        const auto &_line = lines[i];
        if (_line.op == config::SETLABEL_IR)
        {
            if (allLabels.find(_line.out) != allLabels.end())
                blockHeads.insert(i);
        }
    }
    // Rule 3
    bool lastIsBranchJumpCall = false;
    for (int i = 0; i < lines.size(); ++i)
    {
        const auto &_line = lines[i];
        if (lastIsBranchJumpCall)
            blockHeads.insert(i);
        lastIsBranchJumpCall = config::isLabeledBranchJump(_line.op) || _line.op==config::CALL_IR;
    }
    // divide blocks
    std::vector<inter::Quad> subBlock;
    for (int i = 0; i < lines.size(); ++i)
    {
        const auto &_line = lines[i];
        if (blockHeads.find(i) != blockHeads.end())
        {
            if (!subBlock.empty())
            {
                blocks.emplace_back(subBlock.begin(), subBlock.end());
            }
            subBlock.clear();
            subBlock.push_back(_line);
        }
        else
        {
            if (i == 0)
                std::cerr << "Do not have a block yet before adding to a block !" << std::endl;
            subBlock.push_back(_line);
        }
    }
    // update status
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

void inter::Proc::addLocalChar(const std::string & name, const inter::InitChar &initChar)
{
    this->localChars.insert(std::make_pair(name, initChar));
}

void inter::Proc::addLocalInt(const std::string & name, const inter::InitInt &initInt)
{
    this->localInts.insert(std::make_pair(name, initInt));
}

const string &inter::Proc::queryProcName() const
{
    return procName;
}

const inter::MapDeclareChar &inter::Proc::queryLocalChars() const
{
    return localChars;
}

const inter::MapDeclareInt &inter::Proc::queryLocalInts() const
{
    return localInts;
}

void inter::Proc::addParam(const string &_name, const string &_type)
{
    parasList.push_back(std::make_pair(_name, _type));
}

const inter::ParasList &inter::Proc::queryParasList() const
{
    return parasList;
}

void inter::Proc::print(std::ofstream &_fsOut) const
{
    _fsOut << procName << ":" << std::endl;
    // paramList
    _fsOut << config::sep << "Params:" << std::endl;
    for (const auto &_entry : parasList)
    {
        _fsOut << config::sep << config::sep << "Para " << _entry.second << " " << _entry.first;
    }
    // localChars
    _fsOut << config::sep << "LocalChars:" << std::endl;
    for (const auto &_entry : localChars)
    {
        _fsOut << config::sep << config::sep << _entry.first << " = ";
        for (const char &_ch : _entry.second.second)
            _fsOut << toString(_ch) << " ";
        _fsOut << ";" << std::endl;
    }
    // localInts
    _fsOut << config::sep << "LocalInts:" << std::endl;
    for (const auto &_entry : localInts)
    {
        _fsOut << config::sep << config::sep << _entry.first << " = ";
        for (const int &_int : _entry.second.second)
            _fsOut << toString(_int) << " ";
        _fsOut << ";" << std::endl;
    }
    // blocks
    _fsOut << config::sep << "Blocks:" << std::endl;
    for (const auto &_block : blocks)
    {
        _block.print(_fsOut);
    }
}
