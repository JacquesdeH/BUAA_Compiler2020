//
// Created by JacquesdeH on 2020/10/1.
//

#ifndef PROJECT_TOKENPAIR_H
#define PROJECT_TOKENPAIR_H

#include <string>

#include "config.h"

using std::string;

class TokenPair
{
private:
    config::TokenCode tkcode;
    string tkvalue;

public:
    TokenPair(const config::TokenCode& _tkcode, const string& _tkvalue);

public:
    config::TokenCode getTkcode() const;
    const string &getTkvalue() const;
};

#endif //PROJECT_TOKENPAIR_H
