//
// Created by JacquesdeH on 2020/10/2.
//

#include "Info.h"

symbol::Info::Info(const config::SymbolType &_symbolType, const config::DataType &_dataType, const uint &_arrayDim,
           const uint &_declareRow, const uint &_dim0, const uint &_dim1)
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
