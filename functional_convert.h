//
// Created by JacquesdeH on 2020/9/26.
//

#ifndef PROJECT_FUNCTIONAL_CONVERT_H
#define PROJECT_FUNCTIONAL_CONVERT_H

#include <string>

#include "config.h"

using std::string;

string tkcode2output(const config::TokenCode& tkcode);
config::TokenCode text2token(const string& token);
string errorType2Code(const config::ErrorType& errorType);

#endif //PROJECT_FUNCTIONAL_CONVERT_H
