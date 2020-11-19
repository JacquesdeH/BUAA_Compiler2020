//
// Created by JacquesdeH on 2020/11/15.
//

#ifndef PROJECT_CORE_MIPS_MIPS_H
#define PROJECT_CORE_MIPS_MIPS_H

#include <vector>
#include <string>
#include <map>
#include <initializer_list>

#include "core_mips_ObjCodes.h"
#include "utils_inter_typedef.h"
#include "utils_inter_MIR.h"

namespace mips
{
    typedef std::map<std::string, mips::SymbolInfo> MipsTable;

    class Mips
    {
    private:
        inter::MIR mir;
        MipsTable mipsTable; // no reset but update at each proc
        BlockRegPool blockRegPool; // reset every block
        int stackOffset; // reset every proc
        int globalOffset; // no reset for $gp use

    public:
        Mips(const inter::MIR & _mir);

    private:
        void _resetStackOffset();
        void _resetGlobalOffset();
        void _resetBlockRegPool();
        mips::ObjCodes _alignStack(const std::string & _type);
        mips::ObjCodes _alignData(const std::string & _type);
        mips::ObjCodes _toReg(std::string & _reg, const std::string &_mark, const bool &_link, const bool &_init,
                              const std::set<std::string> &_excludedRegs = {});

    private:
        mips::ObjCodes _compileMain(const inter::Proc & _main);
        mips::ObjCodes _compileFuncs(const std::vector<inter::Proc> & funcs);
        mips::ObjCodes _compileProc(const inter::Proc & _proc);
        mips::ObjCodes _compileBlock(const inter::Block & _block);
        mips::ObjCodes _compileQuad(const inter::Quad & _quad);
        mips::ObjCodes _compileGlobalStrings(const inter::MapDeclareString & mapGlobalString);
        mips::ObjCodes _compileGlobalChars(const inter::MapDeclareChar & mapGlobalChar);
        mips::ObjCodes _compileGlobalInts(const inter::MapDeclareInt & mapGlobalInt);
        mips::ObjCodes _compileGlobals(const inter::MapDeclareChar & mapGlobalChar,
                                       const inter::MapDeclareInt & mapGlobalInt);

    private:
        mips::ObjCodes compileDataSegment();
        mips::ObjCodes compileTextSegment();

    private:
        mips::ObjCodes _compileMathOp(const inter::Quad &_quad);
        mips::ObjCodes _compileBranchJumpOp(const inter::Quad &_quad);
        mips::ObjCodes _compileLoadStoreOp(const inter::Quad &_quad);
        mips::ObjCodes _compileLoadOp(const inter::Quad &_quad);
        mips::ObjCodes _compileStoreOp(const inter::Quad &_quad);
        mips::ObjCodes _compileMoveOp(const inter::Quad &_quad);

    public:
        mips::ObjCodes compile();
    };
}


#endif //PROJECT_CORE_MIPS_MIPS_H
