//
// Created by JacquesdeH on 2020/9/25.
//

#include <cstring>
#include <unordered_set>

#include "core_lexic_Lexic.h"
#include "functional_convert.h"
#include "functional_strext.h"

lexic::Lexic::Lexic(const string &fIn, const string &fOut, PeekableQueue* _queue, error::ErrorManager* _errorManager)
{
    this->reader = new Reader(fIn);
    this->printer = new Printer(fOut);
    this->ch = '\0';
    this->queue = _queue;
    this->errorManager = _errorManager;
}

lexic::Lexic::~Lexic()
{
    delete this->reader;
    delete this->printer;
}

bool lexic::Lexic::_isBlank(const char& c)
{
    const static char blanks[] = {' ', '\n', '\t', '\0'};
//    const static int len_blanks = sizeof(blanks);

    for (const char& blank : blanks)
    {
        if (c == blank)
            return true;
    }
    return false;
}

bool lexic::Lexic::_isLetter(const char &c)
{
    return (c == '_') || ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
}

bool lexic::Lexic::_isDigit(const char &c)
{
    return ('0' <= c && c <= '9');
}

bool lexic::Lexic::_isCharLetter(const char &c)
{
    return (c == '+' || c == '-') || (c == '*' || c == '/') || _isLetter(c) || _isDigit(c);
}

bool lexic::Lexic::_isStringLetter(const char &c)
{
    return c == 32 || c == 33 || (35 <= c && c <= 126);
}

bool lexic::Lexic::_isReservedToken(const char *buffer)
{
    string s(buffer);
    s = toLower(s);
    for (const string& token : config::RESERVED)
    {
        if (s == token)
            return true;
    }
    return false;
}

void lexic::Lexic::_readNext()
{
    ch = this->reader->next();
}

void lexic::Lexic::_skipBlank()
{
    while (_isBlank(ch))
        _readNext();
}

void lexic::Lexic::_logtoken(const config::TokenCode &tkcode, const string &value, const int &row, const int& column)
{
    // TODO: update Table
    this->queue->push(Token(tkcode, value, row, column));
    this->printer->print(tkcode, value);
}

bool lexic::Lexic::_parseTk()
{
    static char buffer[config::BUFFER_SIZE];
    int p = 0;
    int row;
    int column;

    _skipBlank();

    row = this->reader->getRow();
    column = this->reader->getColumn();

    if (ch == EOF)
    {
        return false;
    }

    // IDENFR or reserved token
    if (_isLetter(ch))
    {
        buffer[p++] = ch;
        _readNext();
        while (_isLetter(ch) || _isDigit(ch))
        {
            buffer[p++] = ch;
            _readNext();
        }
        buffer[p] = '\0';
        if (_isReservedToken(buffer))
            _logtoken(text2token(buffer), buffer, row, column);
        else
            _logtoken(config::IDENFR, buffer, row, column);
    }
    // INTCON
    else if (_isDigit(ch))
    {
        buffer[p++] = ch;
        _readNext();
        while (_isDigit(ch))
        {
            buffer[p++] = ch;
            _readNext();
        }
        buffer[p] = '\0';
        _logtoken(config::INTCON, buffer, row, column);
    }
    // CHARCON
    else if (ch == '\'')
    {
        _readNext();
        if (_isCharLetter(ch))
        {
            buffer[p++] = ch;
            _readNext();
        }
        buffer[p] = '\0';
        if (ch == '\'')
        {
            _readNext();
            if (strlen(buffer) == 0)
            {
                // ErrorManager
                // charcon is empty
                errorManager->insertError(row, column, config::ErrorType::EmptyCharOrString, "Empty char constant");
                // no skip
            }
            else
                _logtoken(config::CHARCON, buffer, row, column);
        }
        else
        {
            // ErrorManager
            // illegal char
            if (!_isCharLetter(ch))
                errorManager->insertError(row, column, config::ErrorType::IllegalLetterChar, "Encountered illegal letter of char constant");
            // char length mor than 1
            else
                errorManager->insertError(row, column, config::ErrorType::CharLengthError, "Char constant length more than 1");
            skipUntil({'\''}, config::stopwordsChar);
            _readNext();
        }
    }
    // STRCON
    else if (ch == '\"')
    {
        _readNext();
        while (_isStringLetter(ch))
        {
            buffer[p++] = ch;
            _readNext();
        }
        buffer[p] = '\0';
        if (ch == '\"')
        {
            _readNext();
            if (strlen(buffer) == 0)
            {
                // ErrorManager
                // empty string
                errorManager->insertError(row, column, config::ErrorType::EmptyCharOrString, "Empty string");
                // no skip
            }
            else
                _logtoken(config::STRCON, buffer, row, column);
        }
        else
        {
            // ErrorManager
            // illegal string letter
            errorManager->insertError(row, column, config::ErrorType::IllegalLetterString, "Illegal string letter");
            skipUntil({'\"'}, config::stopwordsChar);
        }
    }
    // 1-letter defined char
    else if (ch == '+' || ch == '-' || ch == '*' || ch == '/' ||
                ch == '(' || ch == ')' || ch == '[' || ch == ']' || ch == '{' || ch == '}' ||
                ch == ':' || ch == ';' || ch == ',')
    {
        buffer[p++] = ch;
        _readNext();
        buffer[p] = '\0';
        _logtoken(text2token(buffer), buffer, row, column);
    }
    // 2-letter defined char
    else if (ch == '<' || ch == '>' || ch == '=' || ch == '!')
    {
        config::TokenCode tkcode;
        // < or <=
        if (ch == '<')
        {
            buffer[p++] = ch;
            _readNext();
            tkcode = config::LSS;
            if (ch == '=')
            {
                buffer[p++] = ch;
                _readNext();
                tkcode = config::LEQ;
            }
        }
        // > or >=
        else if (ch == '>')
        {
            buffer[p++] = ch;
            _readNext();
            tkcode = config::GRE;
            if (ch == '=')
            {
                buffer[p++] = ch;
                _readNext();
                tkcode = config::GEQ;
            }
        }
        // = or ==
        else if (ch == '=')
        {
            buffer[p++] = ch;
            _readNext();
            tkcode = config::ASSIGN;
            if (ch == '=')
            {
                buffer[p++] = ch;
                _readNext();
                tkcode = config::EQL;
            }
        }
        // !=
        else if (ch == '!')
        {
            buffer[p++] = ch;
            _readNext();
            if (ch == '=')
            {
                buffer[p++] = ch;
                _readNext();
                tkcode = config::NEQ;
            }
            else
            {
                // TODO: log ErrorManager with dangling char '!'
            }
        }
        else
            std::cerr << "Unexpected in Lexic 2-letter defined chars" << std::endl;
        // common behaviors
        buffer[p] = '\0';
        _logtoken(tkcode, buffer, row, column);
    }
    else
    {
        // TODO: ErrorManager with un-recognized char
        _readNext();
    }

    return true;
}

void lexic::Lexic::run()
{
    while (_parseTk());
}

void
lexic::Lexic::skipUntil(const std::initializer_list<char> &successors, const std::initializer_list<char> &stopwords)
{
    std::unordered_set<char> wordset;
    for (const auto & word : successors)
        wordset.insert(word);
    for (const auto & word : stopwords)
        wordset.insert(word);
    do
    {
        _readNext();
    } while (wordset.find(ch) != wordset.end());
}
