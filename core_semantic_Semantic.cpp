//
// Created by JacquesdeH on 2020/11/11.
//

#include "core_semantic_Semantic.h"
#include "functional_strext.h"

semantic::Semantic::Semantic()
{
    this->errored = false;
    this->tempIdx = 0;
    this->labelIdx = 0;
    this->stringIdx = 0;
}

void semantic::Semantic::error()
{
    this->errored = true;
}

bool semantic::Semantic::noError() const
{
    return !(this->errored);
}

void semantic::Semantic::addMIR(const config::IRCode &_op, const std::string &_out, const std::string &_inl,
                                const std::string &_inr)
{
    if (this->errored)
        return;
    mir.addQuad(inter::Quad(_op, _out, _inl, _inr));
}

std::string semantic::Semantic::genTemp()
{
    tempIdx++;
    return config::tempHead + toString(tempIdx);
}

std::string semantic::Semantic::genLabel()
{
    labelIdx++;
    return config::labelHead + toString(labelIdx);
}

std::string semantic::Semantic::genGlobalString(const std::string & _strContent)
{
    stringIdx++;
    string name = config::stringHead + toString(stringIdx);
    mir.declareGlobalString(name, _strContent);
    return name;
}

void semantic::Semantic::newProc(const std::string & _procName)
{
    mir.newProc(config::procHead + toLower(_procName));
}

void semantic::Semantic::doneGenerationToBlocks()
{
    mir.doneGenerationToBlocks();
}

std::string
semantic::Semantic::genGlobalChar(const string &_name, const int &_count, const std::vector<char> &_initValues)
{
    string _mark = config::globalHead + toLower(_name);
    this->mir.declareGlobalChar(_mark, _count, _initValues);
    return _mark;
}

std::string semantic::Semantic::genGlobalInt(const string &_name, const int &_count, const std::vector<int> &_initValues)
{
    string _mark = config::globalHead + toLower(_name);
    this->mir.declareGlobalInt(_mark, _count, _initValues);
    return _mark;
}

const inter::MIR &semantic::Semantic::getMir() const
{
    return mir;
}

std::string
semantic::Semantic::genLocalChar(const string &_name, const int &_count, const std::vector<char> &_initValues)
{
    string _mark = config::localHead + toLower(_name);
    this->mir.declareLocalChar(_mark, std::make_pair(_count, _initValues));
    return _mark;
}

std::string semantic::Semantic::genLocalInt(const string &_name, const int &_count, const std::vector<int> &_initValues)
{
    string _mark = config::localHead + toLower(_name);
    this->mir.declareLocalInt(_mark, std::make_pair(_count, _initValues));
    return _mark;
}

std::string semantic::Semantic::generateExtended(const string &_name, const string &_type)
{
    std::string lowerName = toLower(_name);
    if (_type == "global")
        return config::globalHead + lowerName;
    else if (_type == "local")
        return config::localHead + lowerName;
    else if (_type == "proc")
        return config::procHead + lowerName;
    else if (_type == "temp")
        return config::tempHead + lowerName;
    else if (_type == "label")
        return config::labelHead + lowerName;
    else if (_type == "string")
        return config::stringHead + lowerName;
    return lowerName;
}

