//
// Created by JacquesdeH on 2020/10/22.
//

#include "error_ErrorManager.h"

error::ErrorManager::ErrorManager()
{
    this->useCout = true;
}

error::ErrorManager::ErrorManager(const std::string &fOut)
{
    this->useCout = false;
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
}

void error::ErrorManager::printAllErrors()
{
    if (!enable_print_tuple)
        return;
    while (!errors.empty())
    {
        ErrorEntry entry = errors.top();
        errors.pop();
        std::string output = entry.to_string();
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
