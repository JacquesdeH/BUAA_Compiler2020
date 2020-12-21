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
            if (config::USE_STDERR)
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
    string ret;
    ret.insert(ret.begin(), character);
    return ret;
}

string deEscape(const string &s)
{
    string ret;
    for (const auto & _ch : s)
    {
        int repeat = 1;
        if (_ch == '\\')
            repeat = 2;
        ret.append(repeat, _ch);
    }
    return ret;
}

int calcMemoryUse(const string &s, const bool &countEnd)
{
    int ret = s.size();
    if (countEnd)
        ret ++;
    return ret;
}

string toString(const config::IRCode &_ircode)
{
    switch (_ircode)
    {
        case config::ADD_IR:
            return "+";
        case config::MINUS_IR:
            return "-";
        case config::MULT_IR:
            return "*";
        case config::DIV_IR:
            return "/";
        case config::LOAD_IR:
            return "LD";
        case config::STORE_IR:
            return "SD";
        case config::MOVE_IR:
            return "MV";
        case config::BEQ_IR:
            return "?==";
        case config::BNE_IR:
            return "?!=";
        case config::BLE_IR:
            return "?<=";
        case config::BLT_IR:
            return "?<";
        case config::JUMP_IR:
            return "JP";
        case config::PARA_IR:
            return "Param";
        case config::PUSH_IR:
            return "Push";
        case config::DEPUSH_IR:
            return "DePush";
        case config::CALL_IR:
            return "Call";
        case config::RET_IR:
            return "Return";
        case config::MOVERET_IR:
            return "MoveRet";
        case config::READ_IR:
            return "Read";
        case config::WRITE_IR:
            return "Write";
        case config::STRING_IR:
            return "String";
        case config::SETLABEL_IR:
            return "Label";
        case config::EXIT_IR:
            return "Exit";
        default:
            return "Error";
    }
}
