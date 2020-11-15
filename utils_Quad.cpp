//
// Created by JacquesdeH on 2020/11/11.
//

#include "utils_Quad.h"

inter::Quad::Quad(const config::IRCode &_op, const std::string & _out, const std::string & _inl, const std::string & _inr)
{
    this->op = _op;
    this->out = _out;
    this->inl = _inl;
    this->inr = _inr;
}
