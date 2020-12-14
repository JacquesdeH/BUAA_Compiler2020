//
// Created by JacquesdeH on 2020/11/15.
//

#include "config.h"
#include "core_mips_Mips.h"
#include "functional_strext.h"

#include <initializer_list>
#include <cmath>

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
        case config::MINUS_IR:
        case config::MULT_IR:
        case config::DIV_IR:
            ret.mergeCodes(_compileMathOp(_quad));
            break;
        case config::LOAD_IR:
        case config::STORE_IR:
            ret.mergeCodes(_compileLoadStoreOp(_quad));
            break;
        case config::MOVE_IR:
            ret.mergeCodes(_compileMoveOp(_quad));
            break;
        case config::BEQ_IR:
        case config::BNE_IR:
        case config::BLE_IR:
        case config::BLT_IR:
        case config::JUMP_IR:
            ret.mergeCodes(_compileBranchJumpOp(_quad));
            break;
        case config::PARA_IR:
            ret.mergeCodes(_compileParaOp(_quad));
            break;
        case config::PUSH_IR:
            ret.mergeCodes(_compilePushOp(_quad));
            break;
        case config::DEPUSH_IR:
            ret.mergeCodes(_compileDepushOp(_quad));
            break;
        case config::CALL_IR:
            ret.mergeCodes(_compileCallOp(_quad));
            break;
        case config::RET_IR:
            ret.mergeCodes(_compileRetOp(_quad));
            break;
        case config::MOVERET_IR:
            ret.mergeCodes(_compileMoveRetOp(_quad));
            break;
        case config::READ_IR:
            ret.mergeCodes(_compileReadOp(_quad));
            break;
        case config::WRITE_IR:
            ret.mergeCodes(_compileWriteOp(_quad));
            break;
        case config::STRING_IR:
            ret.mergeCodes(_compileStringOp(_quad));
            break;
        case config::SETLABEL_IR:
            ret.insertLabel(_quad.out);
            break;
        case config::EXIT_IR:
            ret.mergeCodes(_compileExitOp(_quad));
        default: ;
    }
    return ret;
}

mips::ObjCodes mips::Mips::_compileBlock(const inter::Block &_block)
{
    mips::ObjCodes ret;
    _resetBlockRegPool();
    int blockOutIRcnt = 0;
    for (const auto & line : _block.queryLines())
    {
        if (config::isBlockOutIRCode(line.op))
        {
            blockOutIRcnt++;
        }
        else
        {
            if (blockOutIRcnt > 0)
                std::cerr << "Already encountered blockOutIR when a non-blockOutIR is occurring !" << std::endl;
            mips::ObjCodes tmp = _compileQuad(line);
            ret.mergeCodes(tmp);
        }
    }
    if (blockOutIRcnt > 1)
        std::cerr << "Count of blockOutIR in one block is more than 1 !" << std::endl;
    ret.mergeCodes(this->blockRegPool.saveWriteBackRegs(mipsTable));
    if (config::isBlockOutIRCode(_block.queryLines().back().op))
        ret.mergeCodes(_compileQuad(_block.queryLines().back()));
    return ret;
}

mips::ObjCodes mips::Mips::_compileProc(const inter::Proc &_proc)
{
    // mipsTable already filled with globals
    // subTable is reversed in new proc
    MipsTable subTable;
    // pre alloc mem of local variables
    this->_resetStackOffset();
    for (const auto & _entry : _proc.queryParasList())
    {
        int atomSize = (_entry.second == "int") ? config::atomSizeInt : config::atomSizeChar;
        std::string _mark = _entry.first; // already defined as extended form
        int addr = stackOffset;
        stackOffset += 1 * config::atomSizePush;
        subTable.insert(std::make_pair(_mark, mips::SymbolInfo(addr, atomSize))); // use as exact length
    }
    _alignStack("word");
    for (const auto & _entry : _proc.queryLocalChars())
    {
        const std::string & _mark = _entry.first;
        const inter::InitChar & _init = _entry.second;
        const int & _count = _init.first;
        const std::vector<char> & _initList = _init.second;
        int addr = stackOffset;
        stackOffset += _count * config::atomSizeChar;
        subTable.insert(std::make_pair(_mark, mips::SymbolInfo(addr, config::atomSizeChar)));
    }
    _alignStack("word");
    for (const auto & _entry : _proc.queryLocalInts())
    {
        const std::string & _mark = _entry.first;
        const inter::InitInt & _init = _entry.second;
        const int & _count = _init.first;
        const std::vector<int> & _initList = _init.second;
        int addr = stackOffset;
        stackOffset += _count * config::atomSizeInt;
        subTable.insert(std::make_pair(_mark, mips::SymbolInfo(addr, config::atomSizeInt)));
    }
    // pre alloc temp variables
    for (const auto & block : _proc.queryBlocks())
    {
        for (const auto & line : block.queryLines())
        {
            if (config::isTemp(line.out))
            {
                const std::string& temp = line.out;
                int addr = stackOffset;
                stackOffset += config::atomSizeTemp;
                subTable.insert(std::make_pair(temp, mips::SymbolInfo(addr, config::atomSizeTemp)));
            }
        }
    }
    // **************************** objCodeRet modification begin **************************************
    mips::ObjCodes ret;
    ret.insertLabel(_proc.queryProcName());
    // actually alloc memory
    mipsTable.insert(subTable.begin(), subTable.end());
    ret.genCodeInsert("addiu", "$sp", "$sp", toString(-stackOffset));
    for (const auto & block : _proc.queryBlocks())
    {
        mips::ObjCodes tmp = _compileBlock(block);
        ret.mergeCodes(tmp);
    }
    // recover is set in return call statement
    // ret.genCodeInsert("addiu", "$sp", "$sp", toString(stackOffset));
    for (const auto & entry : subTable)
    {
        const std::string & _name = entry.first;
        mipsTable.erase(_name);
    }

    ret.nextLine();
    return ret;
}

mips::ObjCodes mips::Mips::_compileGlobalStrings(const inter::MapDeclareString &mapGlobalString)
{
    mips::ObjCodes ret;
    ret.mergeCodes(_alignData("word"));
    for (const auto & entry : mapGlobalString)
    {
        std::string mark = entry.first;
        std::string content = deEscape(entry.second);
        int memoryUse = calcMemoryUse(entry.second, true);
        std::string _code = mark + ": .asciiz \"" + content + "\"";
        ret.insertCode(_code);
        int addr = globalOffset;
        globalOffset += memoryUse;
        mipsTable.insert(std::make_pair(mark, mips::SymbolInfo(addr, memoryUse)));
    }
    return ret;
}

mips::ObjCodes mips::Mips::_compileGlobalChars(const inter::MapDeclareChar &mapGlobalChar)
{
    mips::ObjCodes ret;
    const int sizeBase = config::atomSizeChar;
    ret.mergeCodes(_alignData("byte"));
    for (const auto & entry : mapGlobalChar)
    {
        const auto & _mark = entry.first;
        const auto & _pair = entry.second;
        const auto & _count = _pair.first;
        const auto & _initValues = _pair.second;
        std::string line;
        if (_initValues.empty())
        {
            line = _mark + ": .space " + toString(_count*sizeBase);
        }
        else
        {
            line = _mark + ": .byte";
            for (const auto & _char : _initValues)
            {
                line += " \'" + toString(_char) + "\'";
            }
        }
        ret.insertCode(line);
        int memoryUse = _count*sizeBase;
        int addr = globalOffset;
        globalOffset += memoryUse;
        mipsTable.insert(std::make_pair(_mark, mips::SymbolInfo(addr, config::atomSizeChar)));
    }
    return ret;
}

mips::ObjCodes mips::Mips::_compileGlobalInts(const inter::MapDeclareInt &mapGlobalInt)
{
    mips::ObjCodes ret;
    const int sizeBase = config::atomSizeInt;
    ret.mergeCodes(_alignData("word"));
    for (const auto & entry : mapGlobalInt)
    {
        const auto & _mark = entry.first;
        const auto & _pair = entry.second;
        const auto & _count = _pair.first;
        const auto & _initValues = _pair.second;
        std::string line;
        if (_initValues.empty())
        {
            line = _mark + ": .space " + toString(_count*sizeBase);
        }
        else
        {
            line = _mark + ": .word";
            for (const auto & _int : _initValues)
            {
                line += " " + toString(_int);
            }
        }
        ret.insertCode(line);
        int memoryUse = _count*sizeBase;
        int addr = globalOffset;
        globalOffset += memoryUse;
        mipsTable.insert(std::make_pair(_mark, mips::SymbolInfo(addr, config::atomSizeInt)));
    }
    return ret;
}

mips::ObjCodes
mips::Mips::_compileGlobals(const inter::MapDeclareChar &mapGlobalChar, const inter::MapDeclareInt &mapGlobalInt)
{
    mips::ObjCodes ret;
    ret.mergeCodes(_compileGlobalChars(mapGlobalChar));
    ret.mergeCodes(_compileGlobalInts(mapGlobalInt));
    return ret;
}

mips::ObjCodes mips::Mips::_compileFuncs(const std::vector<inter::Proc> &funcs)
{
    mips::ObjCodes ret;
    // global symbols already stored in mipsTable
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
    this->_resetGlobalOffset();
    ret.insertCode(".data");
    ret.mergeCodes(_compileGlobalStrings(mir.queryGlobalStrings()));
    ret.mergeCodes(_compileGlobals(mir.queryGlobalChars(), mir.queryGlobalInts()));
    return ret;
}

mips::ObjCodes mips::Mips::compileTextSegment()
{
    mips::ObjCodes ret;
    ret.insertCode(".text");
    ret.genCodeInsert("li", config::globalReg, config::gpInit);
    ret.genCodeInsert("j", config::mainLabel);
    ret.nextLine();
    mips::ObjCodes funcsObj = _compileFuncs(mir.queryFunctions());
    ret.mergeCodes(funcsObj);
    mips::ObjCodes mainObj = _compileMain(mir.queryMainProc());
    ret.mergeCodes(mainObj);
    return ret;
}

mips::ObjCodes mips::Mips::compile()
{
    mir.assertIsSeal();

    mips::ObjCodes ret;
    mips::ObjCodes dataSegment = compileDataSegment();
    ret.mergeCodes(dataSegment);
    ret.nextLine();
    mips::ObjCodes textSegment = compileTextSegment();
    ret.mergeCodes(textSegment);
    return ret;
}

mips::ObjCodes mips::Mips::_alignData(const string &_type)
{
    int alignWidth =
            (_type == "byte") ? 1 :
            (_type == "half") ? 2 :
            (_type == "word") ? 4 :
            0;
    int alignCode =
            (_type == "byte") ? 0 :
            (_type == "half") ? 1 :
            (_type == "word") ? 2 :
            -1;
    if (alignWidth == 0)
        std::cerr << "Unexpected align type in _alignData" << std::endl;
    mips::ObjCodes ret;
    ret.insertCode(".align " + toString(alignCode));
    while (globalOffset % alignWidth != 0)
        globalOffset ++;
    return ret;
}

mips::ObjCodes mips::Mips::_alignStack(const string &_type)
{
    int alignWidth =
            (_type == "byte") ? 1 :
            (_type == "half") ? 2 :
            (_type == "word") ? 4 :
            0;
    if (alignWidth == 0)
        std::cerr << "Unexpected align type in _alignStack" << std::endl;
    while (stackOffset % alignWidth != 0)
        stackOffset ++;
    return mips::ObjCodes();
}

void mips::Mips::_resetStackOffset()
{
    stackOffset = 0;
}

void mips::Mips::_resetGlobalOffset()
{
    globalOffset = 0;
}

mips::ObjCodes mips::Mips::_compileMathOp(const inter::Quad &_quad)
{
    mips::ObjCodes ret;
    std::string _out, _inl, _inr;
    _out = _quad.out;
    _inl = _quad.inl;
    _inr = _quad.inr;
    std::string _regOut, _regInl, _regInr;
    string opcode =
            (_quad.op == config::ADD_IR) ? "addu" :
            (_quad.op == config::MINUS_IR) ? "subu" :
            (_quad.op == config::MULT_IR) ? "mul" :
            (_quad.op == config::DIV_IR) ? "div" :
            "";
    if (_quad.op == config::ADD_IR || _quad.op == config::MINUS_IR)
    {
        ret.mergeCodes(_toReg(_regInl, _inl, true, true, {}, ""));
        ret.mergeCodes(_toReg(_regInr, _inr, true, true, {_regInl}, ""));
        ret.mergeCodes(_toReg(_regOut, _out, true, false, {_regInl, _regInr}, ""));
        ret.genCodeInsert(opcode, _regOut, _regInl, _regInr);
        blockRegPool.markWriteBack(_regOut);
    }
    else if (_quad.op == config::MULT_IR)
    {
        if (config::OPTIM_BACKEND_MULT_SHIFT && config::isNumeric(_inl) && config::isAbsPower2(str2int(_inl)))
        {
            std::string tmp = _inl;
            _inl = _inr;
            _inr = tmp;
        }
        // if needed to swap, has swapped if OPTIM_MULT
        if (config::OPTIM_BACKEND_MULT_SHIFT && config::isNumeric(_inr) && config::isAbsPower2(str2int(_inr)))
        {
            int multer = str2int(_inr);
            int logMulter = config::toAbsLog2(multer);
            if (multer == 0)
            {
                ret.mergeCodes(_toReg(_regOut, _out, true, false, {}, ""));
                ret.genCodeInsert("addu", _regOut, config::zeroReg, config::zeroReg);
                blockRegPool.markWriteBack(_regOut);
            }
            else
            {
                ret.mergeCodes(_toReg(_regInl, _inl, true, true, {}, ""));
                ret.mergeCodes(_toReg(_regOut, _out, true, false, {_regInl}, ""));
                ret.genCodeInsert("sll", _regOut, _regInl, toString(logMulter));
                if (multer < 0)
                    ret.genCodeInsert("subu", _regOut, config::zeroReg, _regOut);
                blockRegPool.markWriteBack(_regOut);
            }
        }
        else
        {
            ret.mergeCodes(_toReg(_regInl, _inl, true, true, {}, ""));
            ret.mergeCodes(_toReg(_regInr, _inr, true, true, {_regInl}, ""));
            ret.mergeCodes(_toReg(_regOut, _out, true, false, {_regInl, _regInr}, ""));
            ret.genCodeInsert(opcode, _regOut, _regInl, _regInr);
            blockRegPool.markWriteBack(_regOut);
        }
    }
    else if (_quad.op == config::DIV_IR)
    {
        // if - else
        ret.mergeCodes(_toReg(_regInl, _inl, true, true, {}, ""));
        ret.mergeCodes(_toReg(_regInr, _inr, true, true, {_regInl}, ""));
        ret.mergeCodes(_toReg(_regOut, _out, true, false, {_regInl, _regInr}, ""));
        ret.genCodeInsert(opcode, "", _regInl, _regInr);
        ret.genCodeInsert("mflo", _regOut);
        blockRegPool.markWriteBack(_regOut);
    }
    return ret;
}

mips::ObjCodes mips::Mips::_compileBranchJumpOp(const inter::Quad &_quad)
{
    mips::ObjCodes ret;
    if (_quad.op == config::JUMP_IR)
    {
        ret.genCodeInsert("j", _quad.out);
        return ret;
    }
    std::string _regL, _regR;
    // both numeric case
    if (config::isNumeric(_quad.inl) && config::isNumeric(_quad.inr))
    {
        int L = str2int(_quad.inl);
        int R = str2int(_quad.inr);
        bool out =
                (_quad.op == config::BEQ_IR) ? L == R :
                (_quad.op == config::BNE_IR) ? L != R :
                (_quad.op == config::BLT_IR) ? L <  R :
                (_quad.op == config::BLE_IR) ? L <= R :
                false;
        if (out == true)
            ret.genCodeInsert("j", _quad.out);
    }
    // one non-numeric case
    else if (config::isNumeric(_quad.inl) || config::isNumeric(_quad.inr))
    {
        std::string _op = branchIR2Op(_quad.op);
        std::string _inl = _quad.inl;
        std::string _inr = _quad.inr;
        if (config::isNumeric(_inl))
        {
            _op = branch2oppo(_op);
            std::string tmp = _inl;
            _inl = _inr;
            _inr = tmp;
        }
        if (config::isNumeric(_inl) || !config::isNumeric(_inr))
            std::cerr << "Still a numeric or right is not numeric when swapped from inl to inr !" << std::endl;
        ret.mergeCodes(_toReg(_regR, _inr, true, true, {}, ""));
        ret.mergeCodes(_toReg(_regL, _inl, true, true, {_regR}, ""));
        ret.genCodeInsert(_op, _quad.out, _regL, _regR);
    }
    // all regs form
    else
    {
        ret.mergeCodes(_toReg(_regL, _quad.inl, true, true, {}, ""));
        ret.mergeCodes(_toReg(_regR, _quad.inr, true, true, {_regL}, ""));
        ret.genCodeInsert(branchIR2Op(_quad.op), _quad.out, _regL, _regR);
    }
    return ret;
}

mips::ObjCodes mips::Mips::_compileLoadStoreOp(const inter::Quad &_quad)
{
    mips::ObjCodes ret;
    if (_quad.op == config::IRCode::LOAD_IR)
        ret.mergeCodes(_compileLoadOp(_quad));
    else if (_quad.op == config::IRCode::STORE_IR)
        ret.mergeCodes(_compileStoreOp(_quad));
    else
        std::cerr << "Unexpected quad op in LoadStoreOp !" << std::endl;
    return ret;
}

mips::ObjCodes mips::Mips::_compileLoadOp(const inter::Quad &_quad)
{
    mips::ObjCodes ret;
    std::string _regOut;
    int atomSize = mipsTable.at(_quad.inl).getAtomSize();
    int arrOffset = mipsTable.at(_quad.inl).getMemOffset();
    bool useSLL = atomSize != config::atomSizeChar;
    std::string gpOrSp = (config::isGlobal(_quad.inl)) ? config::globalReg : config::stackReg;
    std::string cmdOp = atomSize2Load(atomSize);
    if (config::isNumeric(_quad.inr))
    {
        int byteOffset = str2int(_quad.inr) * (useSLL ? config::atomSizeInt : config::atomSizeChar);
        int totOffset = arrOffset + byteOffset;
        ret.mergeCodes(_toReg(_regOut, _quad.out, true, false, {}, ""));
        ret.genCodeInsert(cmdOp, _regOut, gpOrSp, toString(totOffset));
        blockRegPool.markWriteBack(_regOut);
    }
    else if (config::isGlobal(_quad.inr) || config::isLocal(_quad.inr) || config::isTemp(_quad.inr))
    {
        std::string _regOffset;
        ret.mergeCodes(_toReg(_regOffset, _quad.inr, false, true, {}, ""));
        if (useSLL)
        {
            ret.genCodeInsert("sll", _regOffset, _regOffset, toString(config::bitsSLLInt));
            blockRegPool.markWriteBack(_regOffset);
        }
        // c = arr[x] with x<<2 done, need $off+$gsp with offset of arr_relative
        ret.genCodeInsert("addu", _regOffset, _regOffset, gpOrSp);
        blockRegPool.markWriteBack(_regOffset);
        ret.mergeCodes(_toReg(_regOut, _quad.out, true, false, {_regOffset}, ""));
        ret.genCodeInsert(cmdOp, _regOut, _regOffset, toString(arrOffset));
        blockRegPool.markWriteBack(_regOut);
    }
    else
        std::cerr << "Unexpected quad.inr in compileLoadOp" << std::endl;
    return ret;
}

mips::ObjCodes mips::Mips::_compileStoreOp(const inter::Quad &_quad)
{
    mips::ObjCodes ret;
    std::string _regOut;
    int atomSize = mipsTable.at(_quad.inl).getAtomSize();
    int arrOffset = mipsTable.at(_quad.inl).getMemOffset();
    bool useSLL = atomSize != config::atomSizeChar;
    std::string gpOrSp = (config::isGlobal(_quad.inl)) ? config::globalReg : config::stackReg;
    std::string cmdOp = atomSize2Store(atomSize);
    if (config::isNumeric(_quad.inr))
    {
        int byteOffset = str2int(_quad.inr) * (useSLL ? config::atomSizeInt : config::atomSizeChar);
        int totOffset = arrOffset + byteOffset;
        ret.mergeCodes(_toReg(_regOut, _quad.out, true, true, {}, ""));
        ret.genCodeInsert(cmdOp, _regOut, gpOrSp, toString(totOffset));
        // update load _regOut
        // blockRegPool.markWriteBack(_regOut);
    }
    else if (config::isGlobal(_quad.inr) || config::isLocal(_quad.inr) || config::isTemp(_quad.inr))
    {
        std::string _regOffset;
        ret.mergeCodes(_toReg(_regOffset, _quad.inr, false, true, {}, ""));
        if (useSLL)
        {
            ret.genCodeInsert("sll", _regOffset, _regOffset, toString(config::bitsSLLInt));
            blockRegPool.markWriteBack(_regOffset);
        }
        // c = arr[x] with x<<2 done, need $off+$gsp with offset of arr_relative
        ret.genCodeInsert("addu", _regOffset, _regOffset, gpOrSp);
        blockRegPool.markWriteBack(_regOffset);
        ret.mergeCodes(_toReg(_regOut, _quad.out, true, true, {_regOffset}, ""));
        ret.genCodeInsert(cmdOp, _regOut, _regOffset, toString(arrOffset));
        // update load _regOut
        // blockRegPool.markWriteBack(_regOut);
    }
    else
        std::cerr << "Unexpected quad.inr in compileStoreOp" << std::endl;
    return ret;
}

mips::ObjCodes mips::Mips::_compileMoveOp(const inter::Quad &_quad)
{
    mips::ObjCodes ret;
    std::string _regOut, _regIn;
    // prepare right expr
    ret.mergeCodes(_toReg(_regIn, _quad.inl, true, true, {}, ""));
    // prepare left reg
    ret.mergeCodes(_toReg(_regOut, _quad.out, true, false, {_regIn}, ""));
    ret.genCodeInsert("move", _regOut, _regIn);
    // update WB mark
    blockRegPool.markWriteBack(_regOut);
    return ret;
}

mips::ObjCodes mips::Mips::_compileReadOp(const inter::Quad &_quad)
{
    mips::ObjCodes ret;
    std::string _readCode = toString(
            (_quad.inl == "int") ? config::_readInt:
            (_quad.inl == "char") ? config::_readChar:
            0);
    // read idenfr
    ret.genCodeInsert("li", "$v0", _readCode);
    ret.genCodeInsert("syscall");
    std::string _regOut;
    ret.mergeCodes(_toReg(_regOut, _quad.out, true, false, {"$v0"}, ""));
    ret.genCodeInsert("move", _regOut, "$v0");
    blockRegPool.markWriteBack(_regOut);
    // read '\n' only when char is read
    /*
    if (_quad.inl == "char")
    {
        ret.genCodeInsert("li", "$v0", toString(config::_readChar));
        ret.genCodeInsert("syscall");
    }
     */
    return ret;
}

mips::ObjCodes mips::Mips::_compileWriteOp(const inter::Quad &_quad)
{
    mips::ObjCodes ret;
    std::string _writeCode = toString(
            (_quad.inl == "int") ? config::_printInt:
            (_quad.inl == "char") ? config::_printChar:
            0);
    std::string _regParam;
    ret.mergeCodes(_toReg(_regParam, _quad.out, false, true, {}, "$a0"));
    ret.genCodeInsert("li", "$v0", _writeCode);
    ret.genCodeInsert("syscall");
    return ret;
}

mips::ObjCodes mips::Mips::_compileStringOp(const inter::Quad &_quad)
{
    mips::ObjCodes ret;
    if (!config::isString(_quad.out))
        std::cerr << "Not a string in Write String Op !" << std::endl;
    // addr of string use $gp with offset addiu
    ret.genCodeInsert("addiu", "$a0", config::globalReg,
                      toString(mipsTable.at(_quad.out).getMemOffset()));
    ret.genCodeInsert("li", "$v0", toString(config::_printString));
    ret.genCodeInsert("syscall");
    return ret;
}

void mips::Mips::_resetBlockRegPool()
{
    this->blockRegPool.reset();
}

mips::ObjCodes mips::Mips::_toReg(string &_reg, const string &_mark, const bool &_link, const bool &_init,
                                  const std::set<std::string> &_excludedRegs, const std::string &_mustReg)
{
    mips::ObjCodes ret;
    // TODO: procRegBook early return
    // blockRegPool with _mustReg check!!
    if (blockRegPool.hasMark(_mark))
    {
        _reg = blockRegPool.queryMark2Reg(_mark);
        if (!_mustReg.empty() && _mustReg != _reg)
            ret.genCodeInsert("move", _mustReg, _reg);
        else
        {
            ret.mergeCodes(blockRegPool.syncLink(_reg, _mark, _link, mipsTable));
        }
        return ret;
    }
    // load to a new reg
    if (_mustReg.empty())
        ret.mergeCodes(blockRegPool.allocBlockReg(_reg, mipsTable, _excludedRegs));
    else
    {
        if (_link)
            std::cerr << "Should not link when selected register !" << std::endl;
        _reg = _mustReg;
    }
    // begin toReg
    if (config::isNumeric(_mark))
    {
        ret.genCodeInsert("ori", _reg, config::zeroReg, _mark);
    }
    else if (config::isGlobal(_mark))
    {
        SymbolInfo _info = mipsTable.at(_mark);
        if (_init)
            ret.genCodeInsert(atomSize2Load(_info.getAtomSize()), _reg, config::globalReg, toString(_info.getMemOffset()));
    }
    else if (config::isLocal(_mark))
    {
        SymbolInfo _info = mipsTable.at(_mark);
        if (_init)
            ret.genCodeInsert(atomSize2Load(_info.getAtomSize()), _reg, config::stackReg, toString(_info.getMemOffset()));
    }
    else if (config::isTemp(_mark))
    {
        SymbolInfo _info = mipsTable.at(_mark);
        if (_init)
            ret.genCodeInsert(atomSize2Load(_info.getAtomSize()), _reg, config::stackReg, toString(_info.getMemOffset()));
    }
    else if (config::isLabel(_mark))
    {
        std::cerr << "Encountered labels in _toReg !" << std::endl;
    }
    else
        std::cerr << "Encountered String or Proc or nothing at all in _toReg !" << std::endl;
    // update links
    if (_link)
        blockRegPool.updateInfo(_reg, _mark);
    return ret;
}

mips::ObjCodes mips::Mips::_compilePushOp(const inter::Quad &_quad)
{
    mips::ObjCodes ret;
    std::vector <std::string> _marks = splitMarks(_quad.out);
    int pushParamMemorySize = config::atomSizePush * (int) (_marks.size() - config::paramRegCntLimit);
    // prepare reg form param first
    for (int i = 0; i < std::min((int) _marks.size(), config::paramRegCntLimit); ++i)
    {
        std::string _paramReg;
        ret.mergeCodes(_toReg(_paramReg, _marks[i], false, true, {}, "$a"+toString(i)));
    }
    // spilled param regs to stack
    if (_marks.size() > config::paramRegCntLimit)
    {
        for (int i = config::paramRegCntLimit; i < _marks.size(); ++i)
        {
            std::string _paramReg;
            ret.mergeCodes(_toReg(_paramReg, _marks[i], true, true, config::ParamRegs, ""));
            int offset = (i - config::paramRegCntLimit) * config::atomSizePush;
            ret.genCodeInsert(atomSize2Store(config::atomSizePush), _paramReg, config::stackReg, toString(-pushParamMemorySize + offset));
        }
        ret.genCodeInsert("addiu", config::stackReg, config::stackReg, toString(-pushParamMemorySize));
    }
    return ret;
}

mips::ObjCodes mips::Mips::_compileParaOp(const inter::Quad &_quad)
{
    mips::ObjCodes ret;
    std::vector <std::string> _marks = splitMarks(_quad.out);
    int pushParamMemorySize = config::atomSizePush * (int) (_marks.size() - config::paramRegCntLimit);
    // reg form params
    for (int i = 0; i < std::min((int) _marks.size(), config::paramRegCntLimit); ++i)
    {
        std::string _paramReg;
        ret.mergeCodes(_toReg(_paramReg, _marks[i], true, false, {}, ""));
        ret.genCodeInsert("move", _paramReg, "$a"+toString(i));
        blockRegPool.markWriteBack(_paramReg);
    }
    // spilled parameters
    if (_marks.size() > config::paramRegCntLimit)
    {
        for (int i = config::paramRegCntLimit; i < _marks.size(); ++i)
        {
            // $fp is already updated to the current stack frame bottom
            const int offset = (i - config::paramRegCntLimit) * config::atomSizePush;
            std::string _paramReg;
            ret.mergeCodes(_toReg(_paramReg, _marks[i], true, false, {}, ""));
            ret.genCodeInsert(atomSize2Load(config::atomSizePush), _paramReg, config::frameReg, toString(offset));
            blockRegPool.markWriteBack(_paramReg);
        }
    }
    return ret;
}

mips::ObjCodes mips::Mips::_compileDepushOp(const inter::Quad &_quad)
{
    mips::ObjCodes ret;
    int paramCount = str2int(_quad.out);
    if (paramCount > config::paramRegCntLimit)
    {
        int pushParamMemorySize = config::atomSizePush * (paramCount - config::paramRegCntLimit);
        ret.genCodeInsert("addiu", config::stackReg, config::stackReg, toString(+pushParamMemorySize));
    }
    return ret;
}

mips::ObjCodes mips::Mips::_compileCallOp(const inter::Quad &_quad)
{
    mips::ObjCodes ret;
    // prepare env
    ret.genCodeInsert(atomSize2Store(config::atomSizeReg), config::frameReg, config::stackReg, toString(- config::atomSizeReg * 1));
    ret.genCodeInsert(atomSize2Store(config::atomSizeReg), config::returnAddrReg, config::stackReg, toString(- config::atomSizeReg * 2));
    ret.genCodeInsert("move", config::frameReg, config::stackReg);
    ret.genCodeInsert("addiu", config::stackReg, config::stackReg, toString(- config::atomSizeReg * 2));
    // use jump call function
    ret.genCodeInsert("jal", _quad.out);
    // restore env
    ret.genCodeInsert("addiu", config::stackReg, config::stackReg, toString(+ config::atomSizeReg * 2));
    ret.genCodeInsert(atomSize2Load(config::atomSizeReg), config::frameReg, config::stackReg, toString(- config::atomSizeReg * 1));
    ret.genCodeInsert(atomSize2Load(config::atomSizeReg), config::returnAddrReg, config::stackReg, toString(- config::atomSizeReg * 2));
    return ret;
}

mips::ObjCodes mips::Mips::_compileMoveRetOp(const inter::Quad &_quad)
{
    mips::ObjCodes ret;
    std::string _reg;
    ret.mergeCodes(_toReg(_reg, _quad.out, true, false, {}, ""));
    ret.genCodeInsert("move", _reg, "$v0");
    blockRegPool.markWriteBack(_reg);
    return ret;
}

mips::ObjCodes mips::Mips::_compileRetOp(const inter::Quad &_quad)
{
    mips::ObjCodes ret;
    if (!_quad.out.empty())
    {
        std::string _reg;
        ret.mergeCodes(_toReg(_reg, _quad.out, false, true, {}, ""));
        ret.genCodeInsert("move", "$v0", _reg);
    }
    // jump back to caller but r4cover stackOffset first
    ret.genCodeInsert("addiu", config::stackReg, config::stackReg, toString(stackOffset));
    ret.genCodeInsert("jr", config::returnAddrReg);
    return ret;
}

mips::ObjCodes mips::Mips::_compileExitOp(const inter::Quad &_quad)
{
    mips::ObjCodes ret;
    ret.genCodeInsert("j", _quad.out);
    return ret;
}
