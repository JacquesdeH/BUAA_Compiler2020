//
// Created by JacquesdeH on 2020/10/2.
//

#ifndef PROJECT_SYMBOL_INFO_H
#define PROJECT_SYMBOL_INFO_H

#include <vector>
#include <initializer_list>

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
        vector<config::DataType> funcParamDataTypeList;
        uint address;

    private:
        bool ctrlDeclared;
        bool ctrlAddressed;

    public:
        Info();
        Info(const config::SymbolType &_symbolType, const config::DataType &_dataType, const uint &_declareRow,
             const uint &_arrayDim = 0, const uint &_dim0 = 0, const uint &_dim1 = 0,
             const std::initializer_list<config::DataType> & _funcParamDataTypeList = {});

    public:
        void logReference(const uint& _row);
        void logAddress(const uint& _address);
        bool checkDeclared() const;
        bool checkAddressed() const;
        void assertDeclared() const;
        void assertAddressed() const;
        bool isFunction() const;
        bool isValuedFunction() const;
        bool isVoidFunction() const;
        bool isSymbolTypeOf(const config::SymbolType & _symbolType) const;
        bool isDataTypeOf(const config::DataType & _dataType) const;
        bool isDimOf(const int & _dims, const int & _dimLim0 = 0, const int & _dimLim1 = 0) const;
    };
}


#endif //PROJECT_SYMBOL_INFO_H
