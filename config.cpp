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
}
