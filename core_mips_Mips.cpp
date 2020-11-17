//
// Created by JacquesdeH on 2020/11/15.
//

#include "config.h"
#include "core_mips_Mips.h"
#include "functional_strext.h"

mips::Mips::Mips(const inter::MIR & _mir)
{
    this->mir = _mir;
}

mips::ObjCodes mips::Mips::_compileQuad(const inter::Quad &_quad)
{
    mips::ObjCodes ret;
    const config::IRCode & op = _quad.op;
    const std::string & out = _quad.out;
    const std::string & inl = _quad.inl;
    const std::string & inr = _quad.inr;
    switch (op)
    {
        case config::ADD_IR:
            if (config::isNumeric(inl))
            {
                ret.genCodeInsert("ori", "$t0", config::zeroReg, inl);
            }
            else if (config::isGlobal(inl))
            {
                ret.genCodeInsert((mipsTable.at(inl).getAtomSize() == 1) ? "lb" : "lw",
                                  "$t0", config::zeroReg, inl);
            }
            else
            {
                ret.genCodeInsert((mipsTable.at(inl).getAtomSize() == 1) ? "lb" : "lw",
                                  "$t0", "$sp", toString(mipsTable.at(inl).getMemOffset()));
            }
            if (config::isNumeric(inr))
            {
                ret.genCodeInsert("ori", "$t1", config::zeroReg, inr);
            }
            else if (config::isGlobal(inr))
            {
                ret.genCodeInsert((mipsTable.at(inr).getAtomSize() == 1) ? "lb" : "lw",
                                  "$t1", config::zeroReg, inr);
            }
            else
            {
                ret.genCodeInsert((mipsTable.at(inr).getAtomSize() == 1) ? "lb" : "lw",
                                  "$t1", "$sp", toString(mipsTable.at(inr).getMemOffset()));
            }
            ret.genCodeInsert("addu", "$t2", "$t0", "$t1");
            if (config::isGlobal(out))
            {
                ret.genCodeInsert((mipsTable.at(out).getAtomSize() == 1) ? "sb" : "sw", "$t2", config::zeroReg, out);
            }
            else
            {
                ret.genCodeInsert((mipsTable.at(out).getAtomSize() == 1) ? "sb" : "sw",
                                  "$t2", "$sp", toString(mipsTable.at(out).getMemOffset()));
            }
            break;
        case config::MINUS_IR:
            if (config::isNumeric(inl))
            {
                ret.genCodeInsert("ori", "$t0", config::zeroReg, inl);
            }
            else if (config::isGlobal(inl))
            {
                ret.genCodeInsert((mipsTable.at(inl).getAtomSize() == 1) ? "lb" : "lw",
                                  "$t0", config::zeroReg, inl);
            }
            else
            {
                ret.genCodeInsert((mipsTable.at(inl).getAtomSize() == 1) ? "lb" : "lw",
                                  "$t0", "$sp", toString(mipsTable.at(inl).getMemOffset()));
            }
            if (config::isNumeric(inr))
            {
                ret.genCodeInsert("ori", "$t1", config::zeroReg, inr);
            }
            else if (config::isGlobal(inr))
            {
                ret.genCodeInsert((mipsTable.at(inr).getAtomSize() == 1) ? "lb" : "lw",
                                  "$t1", config::zeroReg, inr);
            }
            else
            {
                ret.genCodeInsert((mipsTable.at(inr).getAtomSize() == 1) ? "lb" : "lw",
                                  "$t1", "$sp", toString(mipsTable.at(inr).getMemOffset()));
            }
            ret.genCodeInsert("subu", "$t2", "$t0", "$t1");
            if (config::isGlobal(out))
            {
                ret.genCodeInsert((mipsTable.at(out).getAtomSize() == 1) ? "sb" : "sw", "$t2", config::zeroReg, out);
            }
            else
            {
                ret.genCodeInsert((mipsTable.at(out).getAtomSize() == 1) ? "sb" : "sw",
                                  "$t2", "$sp", toString(mipsTable.at(out).getMemOffset()));
            }
            break;
        case config::MULT_IR:
            if (config::isNumeric(inl))
            {
                ret.genCodeInsert("ori", "$t0", config::zeroReg, inl);
            }
            else if (config::isGlobal(inl))
            {
                ret.genCodeInsert((mipsTable.at(inl).getAtomSize() == 1) ? "lb" : "lw",
                                  "$t0", config::zeroReg, inl);
            }
            else
            {
                ret.genCodeInsert((mipsTable.at(inl).getAtomSize() == 1) ? "lb" : "lw",
                                  "$t0", "$sp", toString(mipsTable.at(inl).getMemOffset()));
            }
            if (config::isNumeric(inr))
            {
                ret.genCodeInsert("ori", "$t1", config::zeroReg, inr);
            }
            else if (config::isGlobal(inr))
            {
                ret.genCodeInsert((mipsTable.at(inr).getAtomSize() == 1) ? "lb" : "lw",
                                  "$t1", config::zeroReg, inr);
            }
            else
            {
                ret.genCodeInsert((mipsTable.at(inr).getAtomSize() == 1) ? "lb" : "lw",
                                  "$t1", "$sp", toString(mipsTable.at(inr).getMemOffset()));
            }
            ret.genCodeInsert("mul", "$t2", "$t0", "$t1");
            if (config::isGlobal(out))
            {
                ret.genCodeInsert((mipsTable.at(out).getAtomSize() == 1) ? "sb" : "sw", "$t2", config::zeroReg, out);
            }
            else
            {
                ret.genCodeInsert((mipsTable.at(out).getAtomSize() == 1) ? "sb" : "sw",
                                  "$t2", "$sp", toString(mipsTable.at(out).getMemOffset()));
            }
            break;
        case config::DIV_IR:
            if (config::isNumeric(inl))
            {
                ret.genCodeInsert("ori", "$t0", config::zeroReg, inl);
            }
            else if (config::isGlobal(inl))
            {
                ret.genCodeInsert((mipsTable.at(inl).getAtomSize() == 1) ? "lb" : "lw",
                                  "$t0", config::zeroReg, inl);
            }
            else
            {
                ret.genCodeInsert((mipsTable.at(inl).getAtomSize() == 1) ? "lb" : "lw",
                                  "$t0", "$sp", toString(mipsTable.at(inl).getMemOffset()));
            }
            if (config::isNumeric(inr))
            {
                ret.genCodeInsert("ori", "$t1", config::zeroReg, inr);
            }
            else if (config::isGlobal(inr))
            {
                ret.genCodeInsert((mipsTable.at(inr).getAtomSize() == 1) ? "lb" : "lw",
                                  "$t1", config::zeroReg, inr);
            }
            else
            {
                ret.genCodeInsert((mipsTable.at(inr).getAtomSize() == 1) ? "lb" : "lw",
                                  "$t1", "$sp", toString(mipsTable.at(inr).getMemOffset()));
            }
            ret.genCodeInsert("div", "", "$t0", "$t1");
            ret.genCodeInsert("mflo", "$t2");
            if (config::isGlobal(out))
            {
                ret.genCodeInsert((mipsTable.at(out).getAtomSize() == 1) ? "sb" : "sw", "$t2", config::zeroReg, out);
            }
            else
            {
                ret.genCodeInsert((mipsTable.at(out).getAtomSize() == 1) ? "sb" : "sw",
                                  "$t2", "$sp", toString(mipsTable.at(out).getMemOffset()));
            }
            break;
        case config::LOAD_IR:
            // TODO: compile
            break;
        case config::STORE_IR:
            if (config::isNumeric(inr))
            {
                ret.genCodeInsert("li", "$t0", inr);
            }
            else
            {
                ret.genCodeInsert("ori", "$t0", config::zeroReg, toString(mipsTable.at(inr).getMemOffset()));
            }
            if (mipsTable.at(inl).getAtomSize() == 4)
                ret.genCodeInsert("sll", "$t0", "$t0", toString(2));
            if (config::isNumeric(out))
            {
                ret.genCodeInsert("ori", "$t1", config::zeroReg, out);
            }
            else
            {
                ret.genCodeInsert((mipsTable.at(out).getAtomSize() == 1) ? "lb" : "lw", "$t1", "$sp", toString(mipsTable.at(out).getMemOffset()));
            }
            ret.genCodeInsert("addiu", "$t0", "$t0", toString(mipsTable.at(inl).getMemOffset()));
            ret.genCodeInsert("addu", "$t0", "$t0", "$sp");
            ret.genCodeInsert((mipsTable.at(inl).getAtomSize() == 1) ? "sb" : "sw", "$t1", "$t0", toString(0));
            break;
        case config::MOVE_IR:
            if (config::isNumeric(inl))
            {
                ret.genCodeInsert("ori", "$t0", config::zeroReg, inl);
            }
            else if (config::isGlobal(inl))
            {
                ret.genCodeInsert((mipsTable.at(inl).getAtomSize() == 1) ? "lb" : "lw",
                                  "$t0", config::zeroReg, inl);
            }
            else
            {
                ret.genCodeInsert((mipsTable.at(inl).getAtomSize() == 1) ? "lb" : "lw",
                                  "$t0", "$sp", toString(mipsTable.at(inl).getMemOffset()));
            }
            if (config::isGlobal(out))
            {
                ret.genCodeInsert((mipsTable.at(out).getAtomSize() == 1) ? "sb" : "sw", "$t0", config::zeroReg, out);
            }
            else
            {
                ret.genCodeInsert((mipsTable.at(out).getAtomSize() == 1) ? "sb" : "sw",
                                  "$t0", "$sp", toString(mipsTable.at(out).getMemOffset()));
            }
            break;
        case config::BEQ_IR:
            // TODO: compile
            break;
        case config::BNE_IR:
            // TODO: compile
            break;
        case config::BLE_IR:
            // TODO: compile
            break;
        case config::BLT_IR:
            // TODO: compile
            break;
        case config::FUNC_IR:
            // TODO: compile
            break;
        case config::FUNCEND_IR:
            // TODO: compile
            break;
        case config::PARA_IR:
            // TODO: compile
            break;
        case config::PUSH_IR:
            // TODO: compile
            break;
        case config::CALL_IR:
            // TODO: compile
            break;
        case config::RET_IR:
            // TODO: compile
            break;
        case config::MOVERET_IR:
            // TODO: compile
            break;
        case config::READ_IR:
            ret.genCodeInsert("li", "$v0", toString((inl == "int") ? 5 : 12));
            ret.genCodeInsert("syscall");
            if (config::isGlobal(out))
            {
                ret.genCodeInsert((mipsTable.at(out).getAtomSize() == 1) ? "sb" : "sw", "$v0", config::zeroReg, out);
            }
            else
            {
                ret.genCodeInsert((mipsTable.at(out).getAtomSize() == 1) ? "sb" : "sw",
                                  "$v0", "$sp", toString(mipsTable.at(out).getMemOffset()));
            }
            if (inl == "char")
            {
                // '\n'
                ret.genCodeInsert("li", "$v0", toString(12));
                ret.genCodeInsert("syscall");
            }
            break;
        case config::WRITE_IR:
            if (config::isNumeric(out))
            {
                ret.genCodeInsert("ori", "$a0", config::zeroReg, out);
            }
            else if (config::isGlobal(out))
            {
                ret.genCodeInsert((mipsTable.at(out).getAtomSize() == 1) ? "lb" : "lw",
                                  "$a0", config::zeroReg, out);
            }
            else
            {
                ret.genCodeInsert((mipsTable.at(out).getAtomSize() == 1) ? "lb" : "lw",
                                  "$a0", "$sp", toString(mipsTable.at(out).getMemOffset()));
            }
            ret.genCodeInsert("li", "$v0", toString((inl == "int") ? 1 : 11));
            ret.genCodeInsert("syscall");
            break;
        case config::STRING_IR:
            ret.genCodeInsert("la", "$a0", out);
            ret.genCodeInsert("li", "$v0", toString(4));
            ret.genCodeInsert("syscall");
            break;
        case config::SETLABEL_IR:
            break;
        case config::EXIT_IR:
            break;
        case config::CONST_IR:
            break;
        case config::VAR_IR:
            break;
        default: ;
    }
    return ret;
}

mips::ObjCodes mips::Mips::_compileBlock(const inter::Block &_block)
{
    mips::ObjCodes ret;
    // reset localPool
    for (const auto & line : _block.queryLines())
    {
        mips::ObjCodes tmp = _compileQuad(line);
        ret.mergeCodes(tmp);
    }
    // writeback used locals
    return ret;
}

mips::ObjCodes mips::Mips::_compileProc(const inter::Proc &_proc)
{
    mips::ObjCodes ret;
    // mipsTable filled with globals
    // alloc mem
    int offset = 0;
    for (const auto & block : _proc.queryBlocks())
    {
        for (const auto & line : block.queryLines())
        {
            if (line.op == config::CONST_IR || line.op == config::VAR_IR)
            {
                int addr = offset;
                offset += str2int(line.inr) * (line.inl == "int" ? 4 : 1);
                mipsTable.insert(std::make_pair(line.out, mips::SymbolInfo(addr, (line.inl == "int" ? 4 : 1))));
            }
        }
        for (const auto & line : block.queryLines())
        {
            if (config::isTemp(line.out))
            {
                const std::string& temp = line.out;
                int addr = offset;
                offset += 4;
                mipsTable.insert(std::make_pair(temp, mips::SymbolInfo(addr, 4)));
            }
        }
    }
    ret.genCodeInsert("subu", "$sp", "$sp", toString(offset));
    for (const auto & block : _proc.queryBlocks())
    {
        mips::ObjCodes tmp = _compileBlock(block);
        ret.mergeCodes(tmp);
    }
    ret.genCodeInsert("addu", "$sp", "$sp", toString(offset));
    ret.nextLine();
    return ret;
}

mips::ObjCodes mips::Mips::_compileStrings(const inter::MapGlobalString &mapGlobalString)
{
    mips::ObjCodes ret;
    ret.mergeCodes(_align("word"));
    for (const auto & entry : mapGlobalString)
    {
        std::string mark = entry.first;
        std::string content = deEscape(entry.second);
        std::string _code = mark + ": .asciiz \"" + content + "\"";
        ret.insertCode(_code);
    }
    return ret;
}

mips::ObjCodes mips::Mips::_compileChars(const inter::MapGlobalChar &mapGlobalChar)
{
    mips::ObjCodes ret;
    const int sizeBase = 1;
    ret.insertCode(".align 1"); // align with byte
    for (const auto & entry : mapGlobalChar)
    {
        const auto & _name = entry.first;
        const auto & _pair = entry.second;
        const auto & _count = _pair.first;
        const auto & _initValues = _pair.second;
        std::string line;
        if (_initValues.empty())
        {
            line = config::globalHead + _name + ": .space " + toString(_count*sizeBase);
        }
        else
        {
            line = config::globalHead + _name + ": .byte";
            for (const auto & _char : _initValues)
            {
                line += " \'" + toString(_char) + "\'";
            }
        }
        ret.insertCode(line);
    }
    return ret;
}

mips::ObjCodes mips::Mips::_compileInts(const inter::MapGlobalInt &mapGlobalInt)
{
    mips::ObjCodes ret;
    const int sizeBase = 4;
    ret.insertCode(".align 2"); // align with word
    for (const auto & entry : mapGlobalInt)
    {
        const auto & _name = entry.first;
        const auto & _pair = entry.second;
        const auto & _count = _pair.first;
        const auto & _initValues = _pair.second;
        std::string line;
        if (_initValues.empty())
        {
            line = config::globalHead + _name + ": .space " + toString(_count*sizeBase);
        }
        else
        {
            line = config::globalHead + _name + ": .word";
            for (const auto & _int : _initValues)
            {
                line += " " + toString(_int);
            }
        }
        ret.insertCode(line);
    }
    return ret;
}

mips::ObjCodes
mips::Mips::_compileGlobals(const inter::MapGlobalChar &mapGlobalChar, const inter::MapGlobalInt &mapGlobalInt)
{
    mips::ObjCodes ret;
    ret.mergeCodes(_compileChars(mapGlobalChar));
    ret.mergeCodes(_compileInts(mapGlobalInt));
    return ret;
}

mips::ObjCodes mips::Mips::_compileFuncs(const std::vector<inter::Proc> &funcs)
{
    mips::ObjCodes ret;
    // global symbols should be gathered before
    for (const auto & _func : funcs)
    {
        mips::ObjCodes tmp = _compileProc(_func);
        ret.mergeCodes(tmp);
    }
    return ret;
}

mips::ObjCodes mips::Mips::_compileMain(const inter::Proc &_main)
{
    mips::ObjCodes ret;
    ret.mergeCodes(_compileProc(_main));
    ret.nextLine();
    return ret;
}

mips::ObjCodes mips::Mips::compileDataSegment()
{
    mips::ObjCodes ret;
    ret.insertCode(".data");
    ret.mergeCodes(_compileStrings(mir.queryGlobalStrings()));
    ret.mergeCodes(_compileGlobals(mir.queryGlobalChars(), mir.queryGlobalInts()));
    return ret;
}

mips::ObjCodes mips::Mips::compileTextSegment()
{
    mips::ObjCodes funcsObj = _compileFuncs(mir.queryFunctions());
    mips::ObjCodes mainObj = _compileMain(mir.queryMainProc());

    mips::ObjCodes ret;
    ret.insertCode(".text");
    ret.genCodeInsert("j", config::mainLabel);
    ret.nextLine();
    ret.mergeCodes(funcsObj);
    ret.mergeCodes(mainObj);
    return ret;
}

mips::ObjCodes mips::Mips::compile()
{
    mir.assertSeal();

    mips::ObjCodes dataSegment = compileDataSegment();
    mips::ObjCodes textSegment = compileTextSegment();

    mips::ObjCodes ret;
    ret.mergeCodes(dataSegment);
    ret.mergeCodes(textSegment);
    return ret;
}

mips::ObjCodes mips::Mips::_align(const string &_type)
{
    int alignWidth =
            (_type == "byte") ? 1 :
            (_type == "half") ? 2 :
            (_type == "word") ? 4 :
            0;
    mips::ObjCodes ret;
    ret.insertCode(".align " + toString(alignWidth));
    return ret;
}
