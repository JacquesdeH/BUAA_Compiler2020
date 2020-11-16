//
// Created by JacquesdeH on 2020/11/11.
//

#include <iostream>
#include <cmath>

#include "utils_inter_Quad.h"

inter::Quad::Quad(const config::IRCode &_op, const std::string & _out, const std::string & _inl, const std::string & _inr)
{
    this->op = _op;
    this->out = _out;
    this->inl = _inl;
    this->inr = _inr;
}

config::IRCode inter::Quad::getOp() const
{
    return op;
}

const std::string &inter::Quad::getOut() const
{
    return out;
}

const std::string &inter::Quad::getInl() const
{
    return inl;
}

const std::string &inter::Quad::getInr() const
{
    return inr;
}

mips::ObjCodes inter::Quad::compile(mips::LocalRegPool & localRegPool, std::map<std::string, mips::SymbolInfo> & mipsTable) const
{
    mips::ObjCodes ret;
    switch (this->op)
    {
        case config::ADD_IR:
            // TODO: mips translation
            break;
        case config::MINUS_IR:
            break;
        case config::MULT_IR:
            break;
        case config::DIV_IR:
            break;
        case config::LOAD_IR:
            break;
        case config::STORE_IR:
            break;
        case config::MOVE_IR:
            break;
        case config::BEQ_IR:
            break;
        case config::BNE_IR:
            break;
        case config::BLE_IR:
            break;
        case config::BLT_IR:
            break;
        case config::FUNC_IR:
            break;
        case config::FUNCEND_IR:
            break;
        case config::PARA_IR:
            break;
        case config::PUSH_IR:
            break;
        case config::CALL_IR:
            break;
        case config::RET_IR:
            break;
        case config::MOVERET_IR:
            break;
        case config::READ_IR:
            break;
        case config::WRITE_IR:
            break;
        case config::STRING_IR:
            break;
        case config::SETLABEL_IR:
            break;
        case config::EXIT_IR:
            break;
        case config::CONST_IR:
            break;
        case config::VAR_IR:
            break;
        default: ;
    }
    return ret;
}

bool inter::Quad::_isNumeric(const std::string &_str)
{
    for (const char _ch : _str)
    {
        if (!isdigit(_ch))
            return false;
    }
    return true;
}

int inter::Quad::_toNumeric(const std::string &_str)
{
    if (!_isNumeric(_str))
    {
        std::cerr << "Not a numeric when converting to numeric!" << std::endl;
    }
    char *p = nullptr;
    return strtol(_str.c_str(), &p, 10);
}
