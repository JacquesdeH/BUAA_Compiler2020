//
// Created by JacquesdeH on 2020/11/11.
//

#ifndef PROJECT_UTILS_INTER_PROC_H
#define PROJECT_UTILS_INTER_PROC_H

#include <vector>

#include "utils_inter_Quad.h"
#include "utils_inter_Block.h"
#include "core_mips_ObjCodes.h"

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
        void assertBlockForm() const;
        void addQuad(const Quad & _quad);
        void buildBlocks();
        std::vector<inter::Block> queryBlocks();
        mips::ObjCodes compile() const;
    };
}


#endif //PROJECT_UTILS_INTER_PROC_H
