//
// Created by JacquesdeH on 2020/9/26.
//

#include "core_lexic_Printer.h"
#include "utils_convert.h"

lexic::Printer::Printer(const string &fOut)
{
    fsOut.open(fOut);
}

lexic::Printer::~Printer()
{
    fsOut.close();
}

void lexic::Printer::print(const config::TokenCode tkcode, const string &tkvalue)
{
    if (!enabled)
        return;
    string tkclass;
    tkclass = tkcode2output(tkcode);

    string ret = tkclass + " " + tkvalue;
    fsOut << ret << std::endl;
}
