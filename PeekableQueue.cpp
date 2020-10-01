//
// Created by JacquesdeH on 2020/10/1.
//

#include <iostream>

#include "PeekableQueue.h"

PeekableQueue::PeekableQueue()
{
    this->queue.clear();
}

void PeekableQueue::push(const TokenPair &tkpair)
{
    this->queue.push_back(tkpair);
}

TokenPair PeekableQueue::peek(const int &k) const
{
    if (k <= 0)
            std::cerr << "Assert PeekableQueue::peek(" << k << ")" << std::endl;
    if (k > queue.size())
        return TokenPair(config::EMPTY, "");
    return queue[k-1];
}

void PeekableQueue::pop(const int &k)
{
    if (k <= 0)
        std::cerr << "Assert PeekableQueue::pop(" << k << ") for k <= 0" << std::endl;
    if (k > queue.size())
        std::cerr << "Assert PeekableQueue::pop(" << k << ") for k > queue.size()" << std::endl;
    queue.erase(queue.begin(), queue.begin() + k);
}

