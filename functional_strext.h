//
// Created by JacquesdeH on 2020/9/26.
//

#ifndef PROJECT_FUNCTIONAL_STREXT_H
#define PROJECT_FUNCTIONAL_STREXT_H

#include <string>

#include "config.h"

using std::string;

char toLower(const char& ch);
string toLower(const string& s);
int str2int(const string& s);
int char2int(const char& ch);
string int2str(const int& _int);
string toString(const config::DataType & type);
string toString(const int & number);
string toString(const char & character);
string toString(const config::IRCode &_ircode);
string deEscape(const string & s);
int calcMemoryUse(const string & s, const bool & countEnd);

#endif //PROJECT_FUNCTIONAL_STREXT_H
