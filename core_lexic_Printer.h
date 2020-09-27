//
// Created by JacquesdeH on 2020/9/26.
//

#ifndef PROJECT_CORE_LEXIC_PRINTER_H
#define PROJECT_CORE_LEXIC_PRINTER_H

#include <iostream>
#include <fstream>
#include <string>

#include "config.h"

using std::string;

namespace lexic
{
    class Printer
    {
    private:
        const bool enabled = config::PRINT_LEXIC;
        std::ofstream fsOut;

    public:
        Printer(const string& fOut);
        ~Printer();

    public:
        void print(const config::TokenCode tkcode, const string& tkvalue);
    };
}


#endif //PROJECT_CORE_LEXIC_PRINTER_H
