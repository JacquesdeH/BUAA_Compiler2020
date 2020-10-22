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
        for (config::ErrorType eachErrorType : entry.second)
            if (eachErrorType == errorType)
                return entry.first;
    }
    // not found errorType
    std::cerr << "Unexpected ErrorType occurred!" << std::endl;
    return "GeneralError";
}
