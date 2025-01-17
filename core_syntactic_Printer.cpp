//
// Created by JacquesdeH on 2020/10/2.
//

#include "config.h"
#include "core_syntactic_Printer.h"
#include "functional_convert.h"

syntactic::Printer::Printer(const string &fOut)
{
    this->enabled = config::PRINT_SYNTACTIC;
    if (enabled)
        this->fsOut.open(fOut);
}

syntactic::Printer::~Printer()
{
    if (enabled)
        fsOut.close();
}

void syntactic::Printer::printToken(const Token &token)
{
    if (!enabled)
        return;
    string tkclass = tkcode2output(token.getTkcode());
    fsOut << tkclass << ' ' << token.getTkvalue() << std::endl;
}

void syntactic::Printer::printComponent(const string &s)
{
    if (!enabled)
        return;
    fsOut << '<' << s << '>' << std::endl;
}
