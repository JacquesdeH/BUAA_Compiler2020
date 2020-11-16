//
// Created by JacquesdeH on 2020/10/23.
//

#include "config.h"

namespace config
{
    bool isValuedDataType(const DataType & dataType)
    {
        return dataType == INT || dataType == CHAR;
    }

    bool isVoidDataType(const DataType & dataType)
    {
        return dataType == VOID;
    }

    bool isTemp(const std::string &_str)
    {
        return _str.find(config::tempHead) == 0;
    }

    bool isLabel(const std::string &_str)
    {
        return _str.find(config::labelHead) == 0;
    }

    bool isString(const std::string &_str)
    {
        return _str.find(config::stringHead) == 0;
    }

    bool isFunc(const std::string &_str)
    {
        return _str.find(config::funcHead) == 0;
    }

    bool isGlobal(const std::string &_str)
    {
        return _str.find(config::globalHead) == 0;
    }

}
