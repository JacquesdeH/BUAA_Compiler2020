//
// Created by JacquesdeH on 2020/10/2.
//

#ifndef PROJECT_SYMBOL_INFO_H
#define PROJECT_SYMBOL_INFO_H

#include <vector>

#include "config.h"

using std::vector;

namespace symbol
{
    class Info
    {
    private:
        config::SymbolType symbolType;
        config::DataType dataType;
        uint arrayDim;
        uint dimLimit[2];
        uint declareRow;
        vector<uint> referRows;
        uint address;

    private:
        bool ctrlDeclared;
        bool ctrlAddressed;

    public:
        Info(const config::SymbolType& _symbolType, const config::DataType& _dataType,
             const uint& _arrayDim, const uint& _declareRow, const uint& _dim0=0, const uint& _dim1=0);

    public:
        void logReference(const uint& _row);
        void logAddress(const uint& _address);
    };
}


#endif //PROJECT_SYMBOL_INFO_H
