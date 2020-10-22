//
// Created by JacquesdeH on 2020/10/22.
//

#include "ErrorEntry.h"
#include "functional_convert.h"

error::ErrorEntry::ErrorEntry(const int &_row, const int &_column, const config::ErrorType &_type,
                       const std::string &_description)
{
    this->row = _row;
    this->column = _column;
    this->type = _type;
    this->description = _description;
}

bool error::ErrorEntry::operator<(const ErrorEntry &other) const
{
    if (this->row != other.row)
        return this->row < other.row;
    if (this->column != other.column)
        return this->column < other.column;
    if (this->type != other.type)
        return this->type < other.type;
    return this->description < other.description;
}

std::string error::ErrorEntry::to_string()
{
    std::string code = errorType2Code(type);
    return std::to_string(row) + " " + code;
}
