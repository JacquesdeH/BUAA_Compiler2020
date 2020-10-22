//
// Created by JacquesdeH on 2020/10/22.
//

#include "ErrorManager.h"

error::ErrorManager::ErrorManager()
{
    this->useCout = false;
}

error::ErrorManager::ErrorManager(const std::string &fOut)
{
    this->useCout = false;
    this->fsOut.open(fOut);
}

error::ErrorManager::~ErrorManager()
{
    if (!useCout)
        this->fsOut.close();
}

void error::ErrorManager::insertError(const int &row, const int &column, const config::ErrorType &type,
                                      const std::string &description)
{
    errors.push(ErrorEntry(row, column, type, description));
}

void error::ErrorManager::printAllErrors()
{
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
