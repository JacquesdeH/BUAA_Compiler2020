//
// Created by JacquesdeH on 2020/10/2.
//

#ifndef PROJECT_SYNTACTIC_PRINTER_H
#define PROJECT_SYNTACTIC_PRINTER_H

#include <fstream>
#include <string>

#include "utils_Token.h"

using std::ofstream;
using std::string;

namespace syntactic
{
    class Printer
    {
    private:
        bool enabled;
        ofstream fsOut;

    public:
        Printer(const string& fOut);

    public:
        void printToken(const Token& token);
        void printComponent(const string& s);
    };
}


#endif //PROJECT_SYNTACTIC_PRINTER_H
