//
// Created by JacquesdeH on 2020/11/17.
//

#ifndef PROJECT_UTILS_INTER_TYPEDEF_H
#define PROJECT_UTILS_INTER_TYPEDEF_H

#include <iostream>
#include <vector>
#include <map>

namespace inter
{
    typedef std::pair<int, std::vector<char> > InitChar;
    typedef std::pair<int, std::vector<int> > InitInt;
    typedef std::map<std::string, InitChar> MapDeclareChar;
    typedef std::map<std::string, InitInt> MapDeclareInt;
    typedef std::map<std::string, std::string> MapDeclareString;
    typedef std::vector<std::pair<std::string, std::string> > ParasList;
}

#endif //PROJECT_UTILS_INTER_TYPEDEF_H
