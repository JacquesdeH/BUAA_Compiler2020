//
// Created by JacquesdeH on 2020/11/11.
//

#include <iostream>

#include "utils_inter_MIR.h"
#include "functional_strext.h"

inter::MIR::MIR(const bool & _sealed)
{
    this->sealed = _sealed;
}

void inter::MIR::declareGlobalString(const std::string &_strName, const std::string &_strContent)
{
    assertSeal();
    globalStrings[_strName] = _strContent;
}

void inter::MIR::newProc()
{
    assertSeal();
    this->procedures.emplace_back();
}

void inter::MIR::doneGenerationToBlocks()
{
    assertSeal();
    for (auto & procedure : procedures)
    {
        procedure.buildBlocks();
    }
    this->sealed = true;
}

void inter::MIR::addQuad(const inter::Quad &_quad)
{
    assertSeal();
    if (procedures.empty())
    {
        std::cerr << "No sub procedures allowed before addQuad is called !" << std::endl;
    }
    this->procedures.back().addQuad(_quad);
}

void inter::MIR::assertSeal() const
{
    if (this->sealed)
        std::cerr << "Modifying sealed MIR !" << std::endl;
}

void inter::MIR::declareGlobalChars(const std::string &_name, const int & _count, const std::vector<char> &_initValues)
{
    this->globalChars[_name] = std::make_pair(_count, _initValues);
}

void inter::MIR::declareGlobalInts(const std::string &_name, const int & _count, const std::vector<int> &_initValues)
{
    this->globalInts[_name] = std::make_pair(_count, _initValues);
}

const inter::MapGlobalString &inter::MIR::queryGlobalStrings() const
{
    return globalStrings;
}

const inter::MapGlobalChar &inter::MIR::queryGlobalChars() const
{
    return globalChars;
}

const inter::MapGlobalInt &inter::MIR::queryGlobalInts() const
{
    return globalInts;
}

std::vector<inter::Proc> inter::MIR::queryFunctions() const
{
    std::vector<inter::Proc> ret = procedures;
    ret.pop_back();
    return ret;
}

const inter::Proc &inter::MIR::queryMainProc() const
{
    return procedures.back();
}
