//
// Created by JacquesdeH on 2020/10/22.
//

#include "error_ErrorEntry.h"
#include "functional_convert.h"
#include "functional_strext.h"

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

bool error::ErrorEntry::operator>(const error::ErrorEntry &other) const
{
    if (this->row != other.row)
        return this->row > other.row;
    if (this->column != other.column)
        return this->column > other.column;
    if (this->type != other.type)
        return this->type > other.type;
    return this->description > other.description;
}

std::string error::ErrorEntry::to_string(const bool & detailed)
{
    std::string code = errorType2Code(type);
    std::string output = int2str(row) + " " + code;
    if (detailed)
        output = output + " " + description;
    return output;
}
