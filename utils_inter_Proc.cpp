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
