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
template <class T> bool oneOf(const T & obj, const std::vector<T> & init);
std::string atomSize2Store(const int & atomSize);
std::string atomSize2Load(const int & atomSize);
std::string branchIR2Op(config::IRCode _code);
std::string branch2oppo(const std::string &_op);
std::vector<std::string> splitMarks(const std::string &_combined, const std::string &_sep = "#");
std::string combineMarks(const std::vector<std::string> &_marks, const std::string &_sep = "#");

#endif //PROJECT_FUNCTIONAL_CONVERT_H
