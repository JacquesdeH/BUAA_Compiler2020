//
// Created by JacquesdeH on 2020/11/15.
//

#ifndef PROJECT_CORE_MIPS_MIPS_H
#define PROJECT_CORE_MIPS_MIPS_H

#include <vector>
#include <string>
#include <map>

#include "core_mips_ObjCodes.h"
#include "utils_inter_MIR.h"

namespace mips
{
    typedef std::map<std::string, mips::SymbolInfo> MipsTable;

    class Mips
    {
    private:
        inter::MIR mir;
        MipsTable mipsTable;
        LocalRegPool localRegPool;
        int stackOffset;
        int globalOffset;

    public:
        Mips(const inter::MIR & _mir);

    private:
        mips::ObjCodes _align(const std::string & _type);
        mips::ObjCodes _compileMain(const inter::Proc & _main);
        mips::ObjCodes _compileFuncs(const std::vector<inter::Proc> & funcs);
        mips::ObjCodes _compileProc(const inter::Proc & _proc);
        mips::ObjCodes _compileBlock(const inter::Block & _block);
        mips::ObjCodes _compileQuad(const inter::Quad & _quad);
        mips::ObjCodes _compileStrings(const inter::MapGlobalString & mapGlobalString);
        mips::ObjCodes _compileChars(const inter::MapGlobalChar & mapGlobalChar);
        mips::ObjCodes _compileInts(const inter::MapGlobalInt & mapGlobalInt);
        mips::ObjCodes _compileGlobals(const inter::MapGlobalChar & mapGlobalChar,
                                       const inter::MapGlobalInt & mapGlobalInt);

    private:
        mips::ObjCodes compileDataSegment();
        mips::ObjCodes compileTextSegment();

    public:
        mips::ObjCodes compile();
    };
}


#endif //PROJECT_CORE_MIPS_MIPS_H
