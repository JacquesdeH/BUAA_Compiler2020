//
// Created by JacquesdeH on 2020/9/25.
//
// Created by JacquesdeH on 2020/9/25.
//

#ifndef PROJECT_CORE_LEXIC_LEXIC_H
#define PROJECT_CORE_LEXIC_LEXIC_H

#include <string>
#include <fstream>
#include <initializer_list>

#include "core_lexic_Reader.h"
#include "core_lexic_Printer.h"
#include "utils_PeekableQueue.h"
#include "ErrorManager.h"

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
        error::ErrorManager* errorManager;

    public:
        Lexic(const string& fIn, const string& fOut, PeekableQueue* _queue, error::ErrorManager* _errorManager);
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
        void _logtoken(const config::TokenCode& tkcode, const string& value, const int& row, const int& column);
        bool _parseTk();
        void _skipUntil(const std::unordered_set<char> & successors, const std::unordered_set<char> & stopwords, const bool & keepCur = false);

    public:
        void run();
    };
}

#endif //PROJECT_CORE_LEXIC_LEXIC_H
