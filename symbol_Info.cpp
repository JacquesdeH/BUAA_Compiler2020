//
// Created by JacquesdeH on 2020/10/2.
//

#include <iostream>

#include "symbol_Info.h"

symbol::Info::Info()
{
    this->ctrlDeclared = false;
    this->ctrlParamListFilled = false;
    this->ctrlAddressed = false;
}

symbol::Info::Info(const config::SymbolType &_symbolType, const config::DataType &_dataType, const uint &_declareRow,
                   const bool & _useGlobal,
                   const uint &_arrayDim, const uint &_dim0, const uint &_dim1,
                   const std::initializer_list<config::DataType> & _funcParamDataTypeList)
{
    this->ctrlDeclared = false;
    this->ctrlParamListFilled = false;
    this->ctrlAddressed = false;

    this->symbolType = _symbolType;
    this->dataType = _dataType;
    this->arrayDim = _arrayDim;
    this->dimLimit[0] = _dim0;
    this->dimLimit[1] = _dim1;
    this->declareRow = _declareRow;
    this->funcParamDataTypeList = _funcParamDataTypeList;
    this->useGlobal = _useGlobal;

    this->ctrlDeclared = true;

    this->referRows.clear();
    this->address = 0;
}

void symbol::Info::logReference(const uint &_row)
{
    this->referRows.push_back(_row);
}

void symbol::Info::logAddress(const uint &_address)
{
    this->address = _address;
    this->ctrlAddressed = true;
}

bool symbol::Info::checkDeclared() const
{
    return ctrlDeclared;
}

bool symbol::Info::checkAddressed() const
{
    return ctrlAddressed;
}

void symbol::Info::assertDeclared() const
{
    if (checkDeclared())
        return;
    std::cerr << "Assertion error on assertDeclared()" << std::endl;
}

void symbol::Info::assertAddressed() const
{
    if (checkAddressed())
        return;
    std::cerr << "Assertion error on assertAddressed()" << std::endl;
}

bool symbol::Info::isFunction() const
{
    assertDeclared();
    return (symbolType == config::SymbolType::FUNCTION);
}

bool symbol::Info::isValuedFunction() const
{
    assertDeclared();
    return isFunction() && (dataType == config::DataType::INT || dataType == config::DataType::CHAR);
}

bool symbol::Info::isVoidFunction() const
{
    assertDeclared();
    return isFunction() && (dataType == config::DataType::VOID);
}

bool symbol::Info::isSymbolTypeOf(const config::SymbolType &_symbolType) const
{
    assertDeclared();
    return symbolType == _symbolType;
}

bool symbol::Info::isDataTypeOf(const config::DataType &_dataType) const
{
    assertDeclared();
    return dataType == _dataType;
}

bool symbol::Info::isDimOf(const int &_dims, const int & _dimLim0, const int & _dimLim1) const
{
    assertDeclared();
    if (arrayDim != _dims)
        return false;
    switch (arrayDim)
    {
        case 0: return true;
        case 1: return dimLimit[0] == _dimLim0;
        case 2: return (dimLimit[0] == _dimLim0) && (dimLimit[1] == _dimLim1);
        default:
            std::cerr << "Unexpected dim counts in isDimOf" << std::endl;
            return false;
    }
}

void symbol::Info::logFuncParam(const vector<config::DataType> &_paramList)
{
    for (const auto & _param : _paramList)
    {
        funcParamDataTypeList.push_back(_param);
    }
    ctrlParamListFilled = true;
}

int symbol::Info::queryFuncParamCount() const
{
    return funcParamDataTypeList.size();
}

bool symbol::Info::checkFuncParamMatchAt(const int &_index, const config::DataType &_dataType)
{
    if (_index < 0)
        std::cerr << "Encountered negative index in checkFuncParamMatchAt" << std::endl;
    if (_index >= funcParamDataTypeList.size())
        return false;
    return _dataType == funcParamDataTypeList[_index];
}

void symbol::Info::assertParamFilled() const
{
    if (ctrlParamListFilled)
        return;
    std::cerr << "Assertion error on assertParamFilled()" << std::endl;
}

vector<config::DataType> symbol::Info::queryParamDataTypeListOfFunction() const
{
    if (!isFunction())
        std::cerr << "Not a function when call queryParamDataTypeList" << std::endl;
    return funcParamDataTypeList;
}

config::DataType symbol::Info::queryDataType() const
{
    return dataType;
}

bool symbol::Info::isGlobal() const
{
    return this->useGlobal;
}

int symbol::Info::queryDimLimAt(const int &_dim)
{
    if (!(1 <= _dim && _dim <= 2))
        std::cerr << "Querying dim not 1 nor 2 at queryDimLimAt !" << std::endl;
    return dimLimit[_dim];
}

