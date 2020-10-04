//
// Created by JacquesdeH on 2020/10/1.
//

#include "utils_Token.h"

Token::Token()
{
    this->tkcode = config::EMPTY;
    this->tkvalue = "";
    this->row = config::ROW_INIT;
    this->column = config::COLUMN_INIT;
}

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

bool Token::isValuedType() const
{
    return tkcode == config::INTTK || tkcode == config::CHARTK;
}

bool Token::isTokens(std::initializer_list<config::TokenCode> _tkcodes) const
{
    for (const config::TokenCode _tkcode : _tkcodes)
    {
        if (this->isToken(_tkcode))
            return true;
    }
    return false;
}

bool Token::isPlusMinusOp() const
{
    return isTokens({config::PLUS, config::MINU});
}

bool Token::isMultDivOp() const
{
    return isTokens({config::MULT, config::DIV});
}

bool Token::isCmpOp() const
{
    return isTokens({config::LSS, config::LEQ, config::GRE, config::GEQ, config::NEQ, config::EQL});
}

bool Token::isLoopKeyword() const
{
    return isTokens({config::FORTK, config::WHILETK});
}
