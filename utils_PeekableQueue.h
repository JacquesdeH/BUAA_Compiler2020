//
// Created by JacquesdeH on 2020/10/1.
//

#ifndef PROJECT_UTILS_PEEKABLEQUEUE_H
#define PROJECT_UTILS_PEEKABLEQUEUE_H

#include <vector>

#include "utils_TokenPair.h"

using std::vector;

class PeekableQueue
{
private:
    vector<TokenPair> queue;

public:
    PeekableQueue();

public:
    void push(const TokenPair& tkpair);
    TokenPair peek(const int& k) const;
    void pop(const int& k);
};


#endif //PROJECT_UTILS_PEEKABLEQUEUE_H
