//
// Created by JacquesdeH on 2020/11/11.
//

#ifndef PROJECT_QUAD_H
#define PROJECT_QUAD_H

#include "config.h"

#include <string>

namespace inter
{
    class Quad
    {
    private:
        config::IRCode op;
        std::string out;
        std::string inl;
        std::string inr;

    public:
        Quad(const config::IRCode & _op, const std::string & _out, const std::string & _inl, const std::string & _inr);
    };
}


#endif //PROJECT_QUAD_H
