//
// Created by JacquesdeH on 2020/11/15.
//

#ifndef PROJECT_OBJCODES_H
#define PROJECT_OBJCODES_H

#include <string>
#include <vector>

namespace mips
{
    class ObjCodes
    {
    private:
        std::vector<std::string> codes;

    public:
        ObjCodes();

    public:
        void insertCode(const std::string & _code);
        void mergeCodes(const ObjCodes & _other);
        void genCode(const std::string & op, const std::string & out = "", const std::string & in1 = "", const std::string & in2 = "");
    };
}


#endif //PROJECT_OBJCODES_H
