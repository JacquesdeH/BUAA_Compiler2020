//
// Created by JacquesdeH on 2020/10/1.
//

#ifndef PROJECT_UTILS_PEEKABLEQUEUE_H
#define PROJECT_UTILS_PEEKABLEQUEUE_H

#include <vector>

#include "utils_Token.h"

using std::vector;

class PeekableQueue
{
private:
    vector<Token> queue;

public:
    PeekableQueue();

public:
    void push(const Token& tkpair);
    Token peek(const int& k = 1) const;
    void pop(const int& k = 1);
};


#endif //PROJECT_UTILS_PEEKABLEQUEUE_H
