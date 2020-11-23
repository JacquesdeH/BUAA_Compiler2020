//
// Created by JacquesdeH on 2020/11/11.
//

#include <iostream>
#include <fstream>

#include "utils_inter_MIR.h"
#include "functional_strext.h"

inter::MIR::MIR(const bool & _sealed)
{
    this->sealed = _sealed;
}

void inter::MIR::declareGlobalString(const std::string &_strName, const std::string &_strContent)
{
    assertNotSeal();
    globalStrings[_strName] = _strContent;
}

void inter::MIR::newProc(const std::string & _procName)
{
    assertNotSeal();
    this->procedures.emplace_back(_procName);
}

void inter::MIR::doneGenerationToBlocks()
{
    assertNotSeal();
    for (auto & procedure : procedures)
    {
        procedure.buildBlocks();
    }
    this->sealed = true;
}

void inter::MIR::addQuad(const inter::Quad &_quad)
{
    assertNotSeal();
    if (procedures.empty())
    {
        std::cerr << "No sub procedures allowed before addQuad is called !" << std::endl;
    }
    this->procedures.back().addQuad(_quad);
}

void inter::MIR::assertNotSeal() const
{
    if (this->sealed)
        std::cerr << "Modifying sealed MIR !" << std::endl;
}

void inter::MIR::assertIsSeal() const
{
    if (!(this->sealed))
        std::cerr << "GenObjCode on not sealed MIR !" << std::endl;
}

void inter::MIR::declareGlobalChar(const std::string &_name, const int & _count, const std::vector<char> &_initValues)
{
    this->globalChars[_name] = std::make_pair(_count, _initValues);
}

void inter::MIR::declareGlobalInt(const std::string &_name, const int & _count, const std::vector<int> &_initValues)
{
    this->globalInts[_name] = std::make_pair(_count, _initValues);
}

const inter::MapDeclareString &inter::MIR::queryGlobalStrings() const
{
    return globalStrings;
}

const inter::MapDeclareChar &inter::MIR::queryGlobalChars() const
{
    return globalChars;
}

const inter::MapDeclareInt &inter::MIR::queryGlobalInts() const
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

void inter::MIR::declareLocalChar(const string &name, const inter::InitChar &initChar)
{
    procedures.back().addLocalChar(name, initChar);
}

void inter::MIR::declareLocalInt(const string &name, const inter::InitInt &initInt)
{
    procedures.back().addLocalInt(name, initInt);
}

void inter::MIR::print(const string &_fOut) const
{
    std::ofstream fsOut;
    fsOut.open(_fOut);
    // globalStrings
    fsOut << "# Global Strings :" << std::endl;
    for (const auto &_entry : globalStrings)
        fsOut << config::sep << _entry.first << " = \"" << _entry.second << "\";" << std::endl;
    fsOut << std::endl;
    // globalChars
    fsOut << "# Global Chars :" << std::endl;
    for (const auto &_entry : globalChars)
    {
        fsOut << config::sep << _entry.first << " = ";
        for (const char &_ch : _entry.second.second)
            fsOut << toString(_ch) + " ";
        fsOut << ";" << std::endl;
    }
    fsOut << std::endl;
    // globalInts
    fsOut << "# Global Ints :" << std::endl;
    for (const auto &_entry : globalInts)
    {
        fsOut << config::sep << _entry.first << " = ";
        for (const int &_int : _entry.second.second)
            fsOut << toString(_int) + " ";
        fsOut << ";" << std::endl;
    }
    fsOut << std::endl;
    // procedures
    for (const auto &_proc : procedures)
    {
        _proc.print(fsOut);
        fsOut << std::endl;
    }
    fsOut.close();
}
