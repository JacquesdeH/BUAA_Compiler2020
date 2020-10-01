//
// Created by JacquesdeH on 2020/10/1.
//

#include "TokenPair.h"

TokenPair::TokenPair(const config::TokenCode &_tkcode, const string &_tkvalue)
{
    this->tkcode = _tkcode;
    this->tkvalue = _tkvalue;
}

config::TokenCode TokenPair::getTkcode() const
{
    return tkcode;
}

const string &TokenPair::getTkvalue() const
{
    return tkvalue;
}

bool TokenPair::isToken(const config::TokenCode &tkcode) const
{
    return tkcode == this->tkcode;
}
