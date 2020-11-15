//
// Created by JacquesdeH on 2020/11/11.
//

#ifndef PROJECT_UTILS_PROC_H
#define PROJECT_UTILS_PROC_H

#include <vector>

#include "utils_Quad.h"
#include "utils_Block.h"

namespace inter
{
    class Proc
    {
    private:
        bool isBlockForm;
        std::vector<inter::Quad> lines;
        std::vector<inter::Block> blocks;

    public:
        Proc(const bool & _isBlockForm = false);

    public:
        void addQuad(const Quad & _quad);
    };
}


#endif //PROJECT_UTILS_PROC_H
