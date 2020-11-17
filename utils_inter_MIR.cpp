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

void inter::MIR::declareString(const std::string &_strName, const std::string &_strContent)
{
    assertSeal();
    strings[_strName] = _strContent;
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

std::map<std::string, std::string> inter::MIR::queryStrings()
{
    return this->strings;
}

std::vector<inter::Proc> inter::MIR::queryProcs()
{
    return this->procedures;
}

mips::ObjCodes inter::MIR::compileStrings() const
{
    mips::ObjCodes ret;
    ret.insertCode(".align 2"); // align with word
    for (const auto & entry : strings)
    {
        std::string _code = entry.first + ": .asciiz " + "\"" + entry.second + "\"";
        ret.insertCode(_code);
    }
    return ret;
}

mips::ObjCodes inter::MIR::compileProcs() const
{
    mips::ObjCodes ret;
    std::map<std::string, mips::SymbolInfo> globalSymbols = getGlobalSymbols();
    for (const auto & procedure : procedures)
    {
        mips::ObjCodes tmp = procedure.compile(globalSymbols);
        ret.mergeCodes(tmp);
    }
    return ret;
}

mips::ObjCodes inter::MIR::compileGlobals() const
{
    mips::ObjCodes ret;
    int sizeBase;

    sizeBase = 1;
    ret.insertCode(".align 2"); // align with word
    for (const auto & entry : globalChars)
    {
        const auto & _name = entry.first;
        const auto & _pair = entry.second;
        const auto & _count = _pair.first;
        const auto & _initValues = _pair.second;
        std::string line;
        if (_initValues.empty())
        {
            line = config::globalHead + _name + ": .space " + toString(_count*sizeBase);
        }
        else
        {
            line = config::globalHead + _name + ": .byte";
            for (const auto & _char : _initValues)
            {
                line += " \'" + toString(_char) + "\'";
            }
        }
        ret.insertCode(line);
    }

    sizeBase = 4;
    ret.insertCode(".align 2"); // align with word
    for (const auto & entry : globalInts)
    {
        const auto & _name = entry.first;
        const auto & _pair = entry.second;
        const auto & _count = _pair.first;
        const auto & _initValues = _pair.second;
        std::string line;
        if (_initValues.empty())
        {
            line = config::globalHead + _name + ": .space " + toString(_count*sizeBase);
        }
        else
        {
            line = config::globalHead + _name + ": .word";
            for (const auto & _int : _initValues)
            {
                line += " " + toString(_int);
            }
        }
        ret.insertCode(line);
    }

    return ret;
}

void inter::MIR::declareGlobalChars(const std::string &_name, const int & _count, const std::vector<char> &_initValues)
{
    this->globalChars[_name] = std::make_pair(_count, _initValues);
}

void inter::MIR::declareGlobalInts(const std::string &_name, const int & _count, const std::vector<int> &_initValues)
{
    this->globalInts[_name] = std::make_pair(_count, _initValues);
}

std::map<std::string, mips::SymbolInfo> inter::MIR::getGlobalSymbols() const
{
    std::map<std::string, mips::SymbolInfo> ret;

    for (const auto & entry : globalChars)
    {
        const auto & _name = entry.first;
        const auto & _pair = entry.second;
        const auto & _count = _pair.first;
        const auto & _initValues = _pair.second;
        ret.insert(std::make_pair(_name, mips::SymbolInfo(0, 1)));
    }
    for (const auto & entry : globalInts)
    {
        const auto & _name = entry.first;
        const auto & _pair = entry.second;
        const auto & _count = _pair.first;
        const auto & _initValues = _pair.second;
        ret.insert(std::make_pair(_name, mips::SymbolInfo(0, 4)));
    }

    return ret;
}

void inter::MIR::buildBlocks()
{
    for (auto & proc : procedures)
    {
        proc.buildBlocks();
    }
}
