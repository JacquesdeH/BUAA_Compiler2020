//
// Created by JacquesdeH on 2020/10/22.
//

#include "error_ErrorManager.h"

error::ErrorManager::ErrorManager(semantic::Semantic* _semantic)
{
    this->useCout = true;
    this->semanticGenerator = _semantic;
}

error::ErrorManager::ErrorManager(semantic::Semantic* _semantic, const std::string &fOut)
{
    this->useCout = false;
    this->semanticGenerator = _semantic;
    if (enable_print_tuple)
        this->fsOut.open(fOut);
}

error::ErrorManager::~ErrorManager()
{
    if (!useCout && enable_print_tuple)
        this->fsOut.close();
}

void error::ErrorManager::insertError(const int &row, const int &column, const config::ErrorType &type,
                                      const std::string &description)
{
    watch = true;
    errors.push(ErrorEntry(row, column, type, description));
    this->semanticGenerator->error();
}

void error::ErrorManager::printAllErrors()
{
    if (!enable_print_tuple)
        return;
    while (!errors.empty())
    {
        ErrorEntry entry = errors.top();
        errors.pop();
        std::string output = entry.to_string(enable_detailed_info);
        if (useCout)
            std::cout << output << std::endl;
        else
            fsOut << output << std::endl;
    }
}

void error::ErrorManager::watchErrors()
{
    watch = false;
}

bool error::ErrorManager::queryWatch() const
{
    return watch;
}
