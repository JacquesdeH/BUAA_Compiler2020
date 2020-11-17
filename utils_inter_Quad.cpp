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
            if (_isNumeric(inl))
            {
                ret.genCodeInsert("ori", "$t0", config::zeroReg, inl);
            }
            else if (config::isGlobal(inl))
            {
                ret.genCodeInsert((mipsTable.at(inl).getAtomSize() == 1) ? "lb" : "lw",
                                  "$t0", config::zeroReg, inl);
            }
            else
            {
                ret.genCodeInsert((mipsTable.at(inl).getAtomSize() == 1) ? "lb" : "lw",
                                  "$t0", "$sp", toString(mipsTable.at(inl).getMemOffset()));
            }
            if (_isNumeric(inr))
            {
                ret.genCodeInsert("ori", "$t1", config::zeroReg, inr);
            }
            else if (config::isGlobal(inr))
            {
                ret.genCodeInsert((mipsTable.at(inr).getAtomSize() == 1) ? "lb" : "lw",
                                  "$t1", config::zeroReg, inr);
            }
            else
            {
                ret.genCodeInsert((mipsTable.at(inr).getAtomSize() == 1) ? "lb" : "lw",
                                  "$t1", "$sp", toString(mipsTable.at(inr).getMemOffset()));
            }
            ret.genCodeInsert("addu", "$t2", "$t0", "$t1");
            if (config::isGlobal(out))
            {
                ret.genCodeInsert((mipsTable.at(out).getAtomSize() == 1) ? "sb" : "sw", "$t2", config::zeroReg, out);
            }
            else
            {
                ret.genCodeInsert((mipsTable.at(out).getAtomSize() == 1) ? "sb" : "sw",
                                  "$t2", "$sp", toString(mipsTable.at(out).getMemOffset()));
            }
            break;
        case config::MINUS_IR:
            if (_isNumeric(inl))
            {
                ret.genCodeInsert("ori", "$t0", config::zeroReg, inl);
            }
            else if (config::isGlobal(inl))
            {
                ret.genCodeInsert((mipsTable.at(inl).getAtomSize() == 1) ? "lb" : "lw",
                                  "$t0", config::zeroReg, inl);
            }
            else
            {
                ret.genCodeInsert((mipsTable.at(inl).getAtomSize() == 1) ? "lb" : "lw",
                                  "$t0", "$sp", toString(mipsTable.at(inl).getMemOffset()));
            }
            if (_isNumeric(inr))
            {
                ret.genCodeInsert("ori", "$t1", config::zeroReg, inr);
            }
            else if (config::isGlobal(inr))
            {
                ret.genCodeInsert((mipsTable.at(inr).getAtomSize() == 1) ? "lb" : "lw",
                                  "$t1", config::zeroReg, inr);
            }
            else
            {
                ret.genCodeInsert((mipsTable.at(inr).getAtomSize() == 1) ? "lb" : "lw",
                                  "$t1", "$sp", toString(mipsTable.at(inr).getMemOffset()));
            }
            ret.genCodeInsert("subu", "$t2", "$t0", "$t1");
            if (config::isGlobal(out))
            {
                ret.genCodeInsert((mipsTable.at(out).getAtomSize() == 1) ? "sb" : "sw", "$t2", config::zeroReg, out);
            }
            else
            {
                ret.genCodeInsert((mipsTable.at(out).getAtomSize() == 1) ? "sb" : "sw",
                                  "$t2", "$sp", toString(mipsTable.at(out).getMemOffset()));
            }
            break;
        case config::MULT_IR:
            if (_isNumeric(inl))
            {
                ret.genCodeInsert("ori", "$t0", config::zeroReg, inl);
            }
            else if (config::isGlobal(inl))
            {
                ret.genCodeInsert((mipsTable.at(inl).getAtomSize() == 1) ? "lb" : "lw",
                                  "$t0", config::zeroReg, inl);
            }
            else
            {
                ret.genCodeInsert((mipsTable.at(inl).getAtomSize() == 1) ? "lb" : "lw",
                                  "$t0", "$sp", toString(mipsTable.at(inl).getMemOffset()));
            }
            if (_isNumeric(inr))
            {
                ret.genCodeInsert("ori", "$t1", config::zeroReg, inr);
            }
            else if (config::isGlobal(inr))
            {
                ret.genCodeInsert((mipsTable.at(inr).getAtomSize() == 1) ? "lb" : "lw",
                                  "$t1", config::zeroReg, inr);
            }
            else
            {
                ret.genCodeInsert((mipsTable.at(inr).getAtomSize() == 1) ? "lb" : "lw",
                                  "$t1", "$sp", toString(mipsTable.at(inr).getMemOffset()));
            }
            ret.genCodeInsert("mul", "$t2", "$t0", "$t1");
            if (config::isGlobal(out))
            {
                ret.genCodeInsert((mipsTable.at(out).getAtomSize() == 1) ? "sb" : "sw", "$t2", config::zeroReg, out);
            }
            else
            {
                ret.genCodeInsert((mipsTable.at(out).getAtomSize() == 1) ? "sb" : "sw",
                                  "$t2", "$sp", toString(mipsTable.at(out).getMemOffset()));
            }
            break;
        case config::DIV_IR:
            if (_isNumeric(inl))
            {
                ret.genCodeInsert("ori", "$t0", config::zeroReg, inl);
            }
            else if (config::isGlobal(inl))
            {
                ret.genCodeInsert((mipsTable.at(inl).getAtomSize() == 1) ? "lb" : "lw",
                                  "$t0", config::zeroReg, inl);
            }
            else
            {
                ret.genCodeInsert((mipsTable.at(inl).getAtomSize() == 1) ? "lb" : "lw",
                                  "$t0", "$sp", toString(mipsTable.at(inl).getMemOffset()));
            }
            if (_isNumeric(inr))
            {
                ret.genCodeInsert("ori", "$t1", config::zeroReg, inr);
            }
            else if (config::isGlobal(inr))
            {
                ret.genCodeInsert((mipsTable.at(inr).getAtomSize() == 1) ? "lb" : "lw",
                                  "$t1", config::zeroReg, inr);
            }
            else
            {
                ret.genCodeInsert((mipsTable.at(inr).getAtomSize() == 1) ? "lb" : "lw",
                                  "$t1", "$sp", toString(mipsTable.at(inr).getMemOffset()));
            }
            ret.genCodeInsert("div", "", "$t0", "$t1");
            ret.genCodeInsert("mflo", "$t2");
            if (config::isGlobal(out))
            {
                ret.genCodeInsert((mipsTable.at(out).getAtomSize() == 1) ? "sb" : "sw", "$t2", config::zeroReg, out);
            }
            else
            {
                ret.genCodeInsert((mipsTable.at(out).getAtomSize() == 1) ? "sb" : "sw",
                                  "$t2", "$sp", toString(mipsTable.at(out).getMemOffset()));
            }
            break;
        case config::LOAD_IR:
            // TODO: compile
            break;
        case config::STORE_IR:
            ret.genCodeInsert("ori", "$t0", config::zeroReg, toString(mipsTable.at(inr).getMemOffset()));
            if (mipsTable.at(inr).getAtomSize() == 4)
                ret.genCodeInsert("sll", "$t0", "$t0", toString(2));
            if (_isNumeric(out))
            {
                ret.genCodeInsert("ori", "$t1", config::zeroReg, out);
            }
            else
            {
                ret.genCodeInsert((mipsTable.at(out).getAtomSize() == 1) ? "lb" : "lw", "$t1", "$sp", toString(mipsTable.at(out).getMemOffset()));
            }
            ret.genCodeInsert("addiu", "$t0", "$t0", toString(mipsTable.at(inl).getMemOffset()));
            ret.genCodeInsert("addu", "$t0", "$t0", "$sp");
            ret.genCodeInsert((mipsTable.at(inl).getAtomSize() == 1) ? "sb" : "sw", "$t1", "$t0", toString(0));
            break;
        case config::MOVE_IR:
            if (_isNumeric(inl))
            {
                ret.genCodeInsert("ori", "$t0", config::zeroReg, inl);
            }
            else if (config::isGlobal(inl))
            {
                ret.genCodeInsert((mipsTable.at(inl).getAtomSize() == 1) ? "lb" : "lw",
                                  "$t0", config::zeroReg, inl);
            }
            else
            {
                ret.genCodeInsert((mipsTable.at(inl).getAtomSize() == 1) ? "lb" : "lw",
                                  "$t0", "$sp", toString(mipsTable.at(inl).getMemOffset()));
            }
            if (config::isGlobal(out))
            {
                ret.genCodeInsert((mipsTable.at(out).getAtomSize() == 1) ? "sb" : "sw", "$t0", config::zeroReg, out);
            }
            else
            {
                ret.genCodeInsert((mipsTable.at(out).getAtomSize() == 1) ? "sb" : "sw",
                                  "$t0", "$sp", toString(mipsTable.at(out).getMemOffset()));
            }
            break;
        case config::BEQ_IR:
            // TODO: compile
            break;
        case config::BNE_IR:
            // TODO: compile
            break;
        case config::BLE_IR:
            // TODO: compile
            break;
        case config::BLT_IR:
            // TODO: compile
            break;
        case config::FUNC_IR:
            // TODO: compile
            break;
        case config::FUNCEND_IR:
            // TODO: compile
            break;
        case config::PARA_IR:
            // TODO: compile
            break;
        case config::PUSH_IR:
            // TODO: compile
            break;
        case config::CALL_IR:
            // TODO: compile
            break;
        case config::RET_IR:
            // TODO: compile
            break;
        case config::MOVERET_IR:
            // TODO: compile
            break;
        case config::READ_IR:
            ret.genCodeInsert("li", "$v0", toString((inl == "int") ? 5 : 12));
            ret.genCodeInsert("syscall");
            if (config::isGlobal(out))
            {
                ret.genCodeInsert((mipsTable.at(out).getAtomSize() == 1) ? "sb" : "sw", "$v0", config::zeroReg, out);
            }
            else
            {
                ret.genCodeInsert((mipsTable.at(out).getAtomSize() == 1) ? "sb" : "sw",
                                  "$v0", "$sp", toString(mipsTable.at(out).getMemOffset()));
            }
            if (inl == "char")
            {
                // '\n'
                ret.genCodeInsert("li", "$v0", toString(12));
                ret.genCodeInsert("syscall");
            }
            break;
        case config::WRITE_IR:
            if (_isNumeric(out))
            {
                ret.genCodeInsert("ori", "$a0", config::zeroReg, out);
            }
            else if (config::isGlobal(out))
            {
                ret.genCodeInsert((mipsTable.at(out).getAtomSize() == 1) ? "lb" : "lw",
                                  "$a0", config::zeroReg, out);
            }
            else
            {
                ret.genCodeInsert((mipsTable.at(out).getAtomSize() == 1) ? "lb" : "lw",
                                  "$a0", "$sp", toString(mipsTable.at(out).getMemOffset()));
            }
            ret.genCodeInsert("li", "$v0", toString((inl == "int") ? 1 : 11));
            ret.genCodeInsert("syscall");
            break;
        case config::STRING_IR:
            ret.genCodeInsert("la", "$a0", out);
            ret.genCodeInsert("li", "$v0", toString(4));
            ret.genCodeInsert("syscall");
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
    char *p = nullptr;
    strtol(_str.c_str(), &p, 10);
    return (*p) == '\0';
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
