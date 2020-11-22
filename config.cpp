//
// Created by JacquesdeH on 2020/10/23.
//

#include <iostream>
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

    bool isProc(const std::string &_str)
    {
        return _str.find(config::procHead) == 0;
    }

    bool isGlobal(const std::string &_str)
    {
        return _str.find(config::globalHead) == 0;
    }

    bool isNumeric(const std::string &_str)
    {
        char *p = nullptr;
        strtol(_str.c_str(), &p, 10);
        return (*p) == '\0';
    }

    int toNumeric(const std::string &_str)
    {
        if (!isNumeric(_str))
        {
            std::cerr << "Not a numeric when converting to numeric!" << std::endl;
        }
        char *p = nullptr;
        return strtol(_str.c_str(), &p, 10);
    }

    bool isLocal(const std::string &_str)
    {
        return _str.find(config::localHead) == 0;
    }

}
