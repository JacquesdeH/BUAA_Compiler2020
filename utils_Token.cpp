//
// Created by JacquesdeH on 2020/10/1.
//

#include "utils_Token.h"

Token::Token(const config::TokenCode &_tkcode, const string &_tkvalue, const int& _row, const int& _column)
{
    this->tkcode = _tkcode;
    this->tkvalue = _tkvalue;
    this->row = _row;
    this->column = _column;
}

config::TokenCode Token::getTkcode() const
{
    return tkcode;
}

const string &Token::getTkvalue() const
{
    return tkvalue;
}

bool Token::isToken(const config::TokenCode &_tkcode) const
{
    return _tkcode == this->tkcode;
}

int Token::getRow() const
{
    return row;
}

int Token::getColumn() const
{
    return column;
}
