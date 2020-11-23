//
// Created by JacquesdeH on 2020/11/11.
//

#include <iostream>
#include <cmath>

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
    _fsOut << config::sep << config::sep << toString(op) << config::sep << out << config::sep << inl << config::sep << inr << std::endl;
}

