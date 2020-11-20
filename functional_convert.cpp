//
// Created by JacquesdeH on 2020/9/26.
//

#include <iostream>
#include <algorithm>

#include "functional_convert.h"
#include "functional_strext.h"

string tkcode2output(const config::TokenCode& tkcode)
{
    string tkclass;
    tkclass = config::TOKEN2STR.at(tkcode);
    return tkclass;
}

config::TokenCode text2token(const string& original)
{
    string token = toLower(original);
    if (config::STR2TOKEN.end() == config::STR2TOKEN.find(token))
    {
        std::cerr << "Error branch in convert::text2token!" << std::endl;
        return config::EMPTY; // EMPTY
    }
    return config::STR2TOKEN.at(token);
}

string errorType2Code(const config::ErrorType &errorType)
{
    for (const auto& entry : config::errorType2Code)
    {
        for (const auto & eachErrorType : entry.second)
            if (eachErrorType == errorType)
                return entry.first;
    }
    // not found errorType
    std::cerr << "Unexpected ErrorType occurred!" << std::endl;
    return "GeneralError";
}

std::vector<char> vectorInt2Char(const std::vector<int> &_vec)
{
    std::vector<char> ret;
    ret.reserve(_vec.size());
    for (const auto & _elem : _vec)
    {
        ret.push_back((char) _elem);
    }
    return ret;
}

std::vector<int> vectorChar2Int(const std::vector<char> &_vec)
{
    std::vector<int> ret;
    ret.reserve(_vec.size());
    for (const auto & _elem : _vec)
    {
        ret.push_back((int) _elem);
    }
    return ret;
}

std::string atomSize2Store(const int &atomSize)
{
    switch (atomSize)
    {
        case 1:
            return "sb";
        case 2:
            return "sh";
        case 4:
            return "sw";
        default:
            return "";
    }
}

std::string atomSize2Load(const int &atomSize)
{
    switch (atomSize)
    {
        case 1:
            return "lb";
        case 2:
            return "lh";
        case 4:
            return "lw";
        default:
            return "";
    }
}

template<class T>
bool oneOf(const T &obj, const std::vector<T> &init)
{
    for (const auto & item : init)
    {
        if (obj == item)
            return true;
    }
    return false;
}

std::string branchIR2Op(config::IRCode _code)
{
    switch (_code)
    {
        case config::BEQ_IR:
            return "beq";
        case config::BNE_IR:
            return "bne";
        case config::BLT_IR:
            return "blt";
        case config::BLE_IR:
            return "ble";
        default:
            return "";
    }
    return "";
}

std::string branch2oppo(const string &_op)
{
    if (_op == "beq")
        return "bne";
    else if (_op == "bne")
        return "beq";
    else if (_op == "blt")
        return "bge";
    else if (_op == "ble")
        return "bgt";
    else if (_op == "bgt")
        return "ble";
    else if (_op == "bge")
        return "blt";
    else
        return "";
}
