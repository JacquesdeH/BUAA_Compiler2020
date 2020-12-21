//
// Created by JacquesdeH on 2020/12/21.
//

#include "functional_setext.h"


std::set<std::string> setIntersection(const std::set<std::string> &_setX, const std::set<std::string> &_setY)
{
    std::set<std::string> ret;
    for (const auto &x : _setX)
    {
        if (_setY.find(x) != _setY.end())
            ret.insert(x);
    }
    return ret;
}

