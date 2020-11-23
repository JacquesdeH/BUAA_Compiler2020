//
// Created by JacquesdeH on 2020/11/11.
//

#include <iostream>
#include <cmath>
#include <iomanip>

#include "utils_inter_Quad.h"
#include "functional_strext.h"

inter::Quad::Quad(const config::IRCode &_op, const std::string & _out, const std::string & _inl, const std::string & _inr)
{
    this->op = _op;
    this->out = _out;
    this->inl = _inl;
    this->inr = _inr;
}

void inter::Quad::print(std::ofstream &_fsOut) const
{
    _fsOut << config::sep << config::sep;
    _fsOut << std::left << std::setw(config::_printQuadWidth) << toString(op);
    _fsOut << std::left << std::setw(config::_printQuadWidth) << out;
    _fsOut << std::left << std::setw(config::_printQuadWidth) << inl;
    _fsOut << std::left << std::setw(config::_printQuadWidth) << inr;
    _fsOut << std::endl;
}

