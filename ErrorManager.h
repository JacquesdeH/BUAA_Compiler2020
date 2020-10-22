//
// Created by JacquesdeH on 2020/10/22.
//

#ifndef PROJECT_ERRORMANAGER_H
#define PROJECT_ERRORMANAGER_H

#include <queue>
#include <iostream>
#include <fstream>

#include "ErrorEntry.h"

namespace error
{
    class ErrorManager
    {
    private:
        const bool enable_print_tuple = config::PRINT_ERROR_TUPLE;
        std::priority_queue<ErrorEntry> errors;
        bool useCout;
        std::ofstream fsOut;

    public:
        ErrorManager();
        ErrorManager(const std::string & fOut);
        ~ErrorManager();

    public:
        void insertError(const int & row, const int & column, const config::ErrorType & type, const std::string & description);
        void printAllErrors();
    };
}


#endif //PROJECT_ERRORMANAGER_H
