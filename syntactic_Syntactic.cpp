//
// Created by JacquesdeH on 2020/10/2.
//

#include "syntactic_Syntactic.h"

syntactic::Syntactic::Syntactic(const string &fOut, PeekableQueue* _queue)
{
    this->printer = new Printer(fOut);
    this->queue = _queue;
}

syntactic::Syntactic::~Syntactic()
{
    delete this->printer;
}
