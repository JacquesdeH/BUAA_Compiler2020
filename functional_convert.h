//
// Created by JacquesdeH on 2020/9/26.
//

#ifndef PROJECT_FUNCTIONAL_CONVERT_H
#define PROJECT_FUNCTIONAL_CONVERT_H

#include <string>
#include <vector>

#include "config.h"

using std::string;

string tkcode2output(const config::TokenCode& tkcode);
config::TokenCode text2token(const string& token);
string errorType2Code(const config::ErrorType& errorType);
std::vector<char> vectorInt2Char(const std::vector<int> &_vec);
std::vector<int> vectorChar2Int(const std::vector<char> &_vec);

#endif //PROJECT_FUNCTIONAL_CONVERT_H
