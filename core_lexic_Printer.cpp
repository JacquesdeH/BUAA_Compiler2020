//
// Created by JacquesdeH on 2020/9/26.
//

#include "core_lexic_Printer.h"
#include "functional_convert.h"

lexic::Printer::Printer(const string &fOut)
{
    if (enabled)
        fsOut.open(fOut);
}

lexic::Printer::~Printer()
{
    if (enabled)
        fsOut.close();
}

void lexic::Printer::print(const config::TokenCode& tkcode, const string &tkvalue)
{
    if (!enabled)
        return;
    string tkclass;
    tkclass = tkcode2output(tkcode);

    string ret = tkclass + " " + tkvalue;
    fsOut << ret << std::endl;
}
