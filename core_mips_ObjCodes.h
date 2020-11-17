//
// Created by JacquesdeH on 2020/11/15.
//

#ifndef PROJECT_CORE_MIPS_OBJCODES_H
#define PROJECT_CORE_MIPS_OBJCODES_H

#include <string>
#include <vector>
#include <set>

namespace mips
{
    class ObjCodes
    {
    private:
        std::vector<std::string> codes;

    public:
        ObjCodes();

    private:
        static bool _isIn(const std::string & target, const std::set<std::string> & options);

    public:
        void insertCode(const std::string & _code);
        void mergeCodes(const ObjCodes & _other);
        void genCodeInsert(const std::string & op, const std::string & out = "", const std::string & in1 = "", const std::string & in2 = "");
        void nextLine();
        void print(const std::string & fOut);
    };
}


#endif //PROJECT_CORE_MIPS_OBJCODES_H
