//
// Created by JacquesdeH on 2020/9/26.
//

#include <cctype>
#include <algorithm>

#include "utils_strext.h"

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
