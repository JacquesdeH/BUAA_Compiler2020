//
// Created by JacquesdeH on 2020/11/15.
//

#include <initializer_list>
#include <fstream>

#include "core_mips_ObjCodes.h"
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

void mips::ObjCodes::genCodeInsert(const std::string &op, const std::string &out, const std::string &in1, const std::string &in2)
{
    std::string cmd;
    if (_isIn(op, {"lw", "sw", "lh", "sh", "lb", "sb"}))
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

bool mips::ObjCodes::_isIn(const std::string &target, const std::set<std::string> &options)
{
    return options.find(target) != options.end();
}

void mips::ObjCodes::print(const std::string &fOut)
{
    std::ofstream fsOut;
    fsOut.open(fOut);
    for (const auto & cmd : codes)
    {
        fsOut << cmd << std::endl;
    }
    fsOut.close();
}

void mips::ObjCodes::nextLine()
{
    codes.emplace_back("\n");
}

void mips::ObjCodes::insertLabel(const std::string &_label)
{
    codes.emplace_back(_label + ":");
}
