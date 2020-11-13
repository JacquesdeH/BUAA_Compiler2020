//
// Created by JacquesdeH on 2020/11/11.
//

#include <iostream>
#include "MIR.h"

inter::MIR::MIR(const bool & _sealed)
{
    this->sealed = _sealed;
    this->procedures.emplace_back();
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

void inter::MIR::doneGeneration()
{
    assertSeal();
    this->sealed = true;
}

void inter::MIR::addQuad(const inter::Quad &_quad)
{
    assertSeal();
    this->procedures.back().addQuad(_quad);
}

void inter::MIR::assertSeal() const
{
    if (this->sealed)
        std::cerr << "Modifying sealed MIR !" << std::endl;
}
