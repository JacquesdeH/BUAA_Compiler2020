//
// Created by JacquesdeH on 2020/9/26.
//

#ifndef PROJECT_UTILS_CONVERT_H
#define PROJECT_UTILS_CONVERT_H

#include <string>

#include "config.h"

using std::string;

string tkcode2output(const config::TokenCode& tkcode);
config::TokenCode text2token(const string& token);

#endif //PROJECT_UTILS_CONVERT_H
