//
// Created by JacquesdeH on 2020/9/25.
//
// Created by JacquesdeH on 2020/9/25.
//

#ifndef PROJECT_CORE_LEXIC_LEXIC_H
#define PROJECT_CORE_LEXIC_LEXIC_H

#include <string>
#include <fstream>

#include "core_lexic_Reader.h"
#include "core_lexic_Printer.h"
#include "PeekableQueue.h"

using std::string;

namespace lexic
{
    class Lexic
    {
    private:
        Reader* reader;
        Printer* printer;
        char ch;
        PeekableQueue* queue;

    public:
        Lexic(const string& fIn, const string& fOut);
        ~Lexic();

    private:
        static bool _isBlank(const char& c);
        static bool _isLetter(const char& c);
        static bool _isDigit(const char& c);
        static bool _isCharLetter(const char& c);
        static bool _isStringLetter(const char& c);
        static bool _isReservedToken(const char* buffer);
        void _readNext();
        void _skipBlank();
        void _logtoken(const config::TokenCode& tkcode, const string& value);
        bool _parseTk();

    public:
        PeekableQueue * run();
    };
}

#endif //PROJECT_CORE_LEXIC_LEXIC_H
