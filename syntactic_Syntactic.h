//
// Created by JacquesdeH on 2020/10/2.
//

#ifndef PROJECT_SYNTACTIC_SYNTACTIC_H
#define PROJECT_SYNTACTIC_SYNTACTIC_H

#include "utils_PeekableQueue.h"
#include "syntactic_Printer.h"

namespace syntactic
{
    class Syntactic
    {
    private:
        PeekableQueue* queue;
        Printer* printer;

    public:
        Syntactic(const string& fOut, PeekableQueue* _queue);
        ~Syntactic();
    };
}


#endif //PROJECT_SYNTACTIC_SYNTACTIC_H
