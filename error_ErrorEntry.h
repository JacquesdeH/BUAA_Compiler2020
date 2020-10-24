//
// Created by JacquesdeH on 2020/10/22.
//

#ifndef PROJECT_ERROR_ERRORENTRY_H
#define PROJECT_ERROR_ERRORENTRY_H

#include <string>

#include "config.h"

namespace error
{
    class ErrorEntry
    {
    private:
        int row;
        int column;
        config::ErrorType type;
        std::string description;

    public:
        ErrorEntry(const int & _row, const int & _column, const config::ErrorType & _type, const std::string & _description);
        bool operator < (const ErrorEntry & other) const;
        bool operator > (const ErrorEntry & other) const;
        std::string to_string(const bool & detailed = false);
    };
}


#endif //PROJECT_ERROR_ERRORENTRY_H
