//
// Created by JacquesdeH on 2020/11/15.
//

#include "ObjCodes.h"
#include "config.h"

mips::ObjCodes::ObjCodes()
{

}

void mips::ObjCodes::insertCode(const std::string &_code)
{
    this->codes.push_back(_code);
}

void mips::ObjCodes::mergeCodes(const mips::ObjCodes &_other)
{
    for (const std::string & _newcode : _other.codes)
    {
        this->codes.push_back(_newcode);
    }
}

void mips::ObjCodes::genCode(const std::string &op, const std::string &out, const std::string &in1, const std::string &in2)
{
    std::string cmd;
    if (op == "lw" || op == "sw")
    {
        cmd = op + config::sep + out + config::sep + in2 + "(" + in1 + ")";
    }
    else
    {
        cmd = op;
        if (!out.empty())
            cmd += config::sep + out;
        if (!in1.empty())
            cmd += config::sep + in1;
        if (!in2.empty())
            cmd += config::sep + in2;
    }
    this->codes.push_back(cmd);
}
