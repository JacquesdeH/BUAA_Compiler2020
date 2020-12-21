//
// Created by JacquesdeH on 2020/10/1.
//

#include <iostream>

#include "utils_PeekableQueue.h"

PeekableQueue::PeekableQueue()
{
    this->queue.clear();
}

void PeekableQueue::push(const Token &tkpair)
{
    this->queue.push_back(tkpair);
}

Token PeekableQueue::peek(const int &k) const
{
    if (k <= 0)
        if (config::USE_STDERR)
            std::cerr << "Assert PeekableQueue::peek(" << k << ")" << std::endl;
    if (k > queue.size())
        return Token(config::EMPTY);
    return queue[k-1];
}

void PeekableQueue::pop(const int &k)
{
    if (k <= 0)
        if (config::USE_STDERR)
            std::cerr << "Assert PeekableQueue::pop(" << k << ") for k <= 0" << std::endl;
    if (k > queue.size())
        if (config::USE_STDERR)
            std::cerr << "Assert PeekableQueue::pop(" << k << ") for k > queue.size()" << std::endl;
    queue.erase(queue.begin(), queue.begin() + k);
}

