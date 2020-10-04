//
// Created by JacquesdeH on 2020/10/1.
//

#ifndef PROJECT_UTILS_TOKEN_H
#define PROJECT_UTILS_TOKEN_H

#include <string>
#include <initializer_list>

#include "config.h"

using std::string;

class Token
{
private:
    config::TokenCode tkcode;
    string tkvalue;
    int row;
    int column;

public:
    Token();
    Token(const config::TokenCode& _tkcode, const string &_tkvalue = "", const int& _row = 0, const int& _column = 0);

public:
    config::TokenCode getTkcode() const;
    const string &getTkvalue() const;
    bool isToken(const config::TokenCode& _tkcode) const;
    bool isTokens(std::initializer_list<config::TokenCode> _tkcodes) const;
    bool isValuedType() const;
    bool isPlusMinusOp() const;
    bool isMultDivOp() const;
    bool isCmpOp() const;
    int getRow() const;
    int getColumn() const;
};

#endif //PROJECT_UTILS_TOKEN_H
