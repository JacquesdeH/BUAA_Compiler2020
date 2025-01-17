//
// Created by JacquesdeH on 2020/11/11.
//

#ifndef PROJECT_UTILS_INTER_PROC_H
#define PROJECT_UTILS_INTER_PROC_H

#include <vector>
#include <fstream>

#include "utils_inter_typedef.h"
#include "utils_inter_Quad.h"
#include "utils_inter_Block.h"
#include "core_mips_ObjCodes.h"

namespace inter
{
    class Proc
    {
    private:
        bool isBlockForm;
        std::string procName;
        ParasList parasList;
        MapDeclareChar localChars;
        MapDeclareInt localInts;
        std::vector<inter::Quad> lines;
        std::vector<inter::Block> blocks;

    public:
        Proc(const std::string & _procName, const bool & _isBlockForm = false);

    public:
        const string &queryProcName() const;
        const ParasList &queryParasList() const;
        const MapDeclareChar &queryLocalChars() const;
        const MapDeclareInt &queryLocalInts() const;
        const std::vector<inter::Quad> &queryLines() const;
        const std::vector<inter::Block> &queryBlocks() const;

    public:
        void assertBlockForm() const;
        void addParam(const std::string &_name, const std::string &_type);
        void addLocalChar(const std::string & name, const InitChar & initChar);
        void addLocalInt(const std::string & name, const InitInt & initInt);
        void addQuad(const Quad & _quad);
        void buildBlocks();
        void print(std::ofstream &_fsOut) const;
    };
}


#endif //PROJECT_UTILS_INTER_PROC_H
