//
// Created by JacquesdeH on 2020/9/26.
//

#define _CRT_SECURE_NO_WARNINGS

#include <cctype>
#include <algorithm>

#include "functional_strext.h"

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