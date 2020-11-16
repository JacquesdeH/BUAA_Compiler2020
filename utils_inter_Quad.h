//
// Created by JacquesdeH on 2020/11/11.
//

#ifndef PROJECT_UTILS_INTER_QUAD_H
#define PROJECT_UTILS_INTER_QUAD_H

#include <string>
#include <map>
#include <string>

#include "config.h"
#include "core_mips_ObjCodes.h"
#include "core_mips_LocalRegPool.h"

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

    private:
        static bool _isNumeric(const std::string & _str);
        static int _toNumeric(const std::string & _str);

    public:
        config::IRCode getOp() const;
        const std::string &getOut() const;
        const std::string &getInl() const;
        const std::string &getInr() const;
        mips::ObjCodes compile(mips::LocalRegPool & localRegPool, std::map<std::string, mips::SymbolInfo> & mipsTable) const;
    };
}

#endif //PROJECT_UTILS_INTER_QUAD_H
