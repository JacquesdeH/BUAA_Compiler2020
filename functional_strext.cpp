//
// Created by JacquesdeH on 2020/9/26.
//

#define _CRT_SECURE_NO_WARNINGS

#include <cctype>
#include <algorithm>
#include <iostream>

#include "functional_strext.h"
#include "config.h"

char toLower(const char& ch)
{
    return (char) tolower((int) ch);
}

string toLower(const string& s)
{
    string ret(s);
    std::transform(ret.begin(), ret.end(), ret.begin(), ::tolower);
    return ret;
}

int str2int(const string& s)
{
    return strtol(s.c_str(), nullptr,10);
}

int char2int(const char& ch)
{
    return (int)(unsigned int)ch;
}

string int2str(const int &_int)
{
    char buffer[config::BUFFER_SIZE];
    sprintf(buffer, "%d", _int);
    return string(buffer);
}

string toString(const config::DataType &type)
{
    string ret;
    switch (type)
    {
        case config::DataType::CHAR:
            ret = "char";
            break;
        case config::DataType::INT:
            ret = "int";
            break;
        case config::DataType::VOID:
            ret = "void";
            break;
        default:
            std::cerr << "Unexpected dataType in toString!" << std::endl;
    }
    return ret;
}

string toString(const int &number)
{
    return std::to_string(number);
}

string toString(const char &character)
{
    return std::to_string(character);
}
