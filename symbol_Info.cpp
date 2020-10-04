//
// Created by JacquesdeH on 2020/10/2.
//

#include <iostream>

#include "symbol_Info.h"

symbol::Info::Info()
{
    this->ctrlDeclared = false;
    this->ctrlAddressed = false;
}

symbol::Info::Info(const config::SymbolType &_symbolType, const config::DataType &_dataType, const uint &_declareRow,
                   const uint &_arrayDim, const uint &_dim0, const uint &_dim1)
{
    this->ctrlDeclared = false;
    this->ctrlAddressed = false;

    this->symbolType = _symbolType;
    this->dataType = _dataType;
    this->arrayDim = _arrayDim;
    this->dimLimit[0] = _dim0;
    this->dimLimit[1] = _dim1;
    this->declareRow = _declareRow;

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

