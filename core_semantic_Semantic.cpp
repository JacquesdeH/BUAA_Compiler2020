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

std::string semantic::Semantic::genString(const std::string & _strContent)
{
    stringIdx++;
    string name = config::stringHead + toString(stringIdx);
    mir.declareString(name, _strContent);
    return name;
}

void semantic::Semantic::newProc()
{
    mir.newProc();
}

void semantic::Semantic::doneGeneration()
{
    mir.doneGeneration();
}

