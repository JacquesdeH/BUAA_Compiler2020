//
// Created by JacquesdeH on 2020/10/22.
//

#ifndef PROJECT_ERROR_ERRORMANAGER_H
#define PROJECT_ERROR_ERRORMANAGER_H

#include <queue>
#include <iostream>
#include <fstream>

#include "error_ErrorEntry.h"
#include "core_semantic_Semantic.h"

namespace error
{
    class ErrorManager
    {
    private:
        const bool enable_print_tuple = config::PRINT_ERROR_TUPLE;
        const bool enable_detailed_info = config::PRINT_DETAILED_ERROR;
        std::priority_queue<ErrorEntry, std::vector<ErrorEntry>, std::greater<ErrorEntry> > errors;
        bool useCout;
        std::ofstream fsOut;
        bool watch;
        semantic::Semantic* semanticGenerator;

    public:
        ErrorManager(semantic::Semantic* _semantic);
        ErrorManager(semantic::Semantic* _semantic, const std::string & fOut);
        ~ErrorManager();

    public:
        void insertError(const int & row, const int & column, const config::ErrorType & type, const std::string & description);
        void printAllErrors();
        void watchErrors();
        bool queryWatch() const;
    };
}


#endif //PROJECT_ERROR_ERRORMANAGER_H
