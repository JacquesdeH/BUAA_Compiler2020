//
// Created by JacquesdeH on 2020/11/15.
//

#ifndef PROJECT_CORE_MIPS_LOCALREGPOOL_H
#define PROJECT_CORE_MIPS_LOCALREGPOOL_H

#include <queue>
#include <string>
#include <unordered_map>

namespace mips
{
    class LocalRegPool
    {
    private:
        std::queue <std::string> freePool;
        std::queue <std::string> allocPool;
        std::unordered_map<std::string, std::string> regs2var;
        std::unordered_map<std::string, std::string> var2regs;

    public:
        LocalRegPool();

    public:
        bool hasFree() const;
        void insertFree(const std::string & _reg);
        std::string queryReg2Var(const std::string & _var);
        std::string queryVar2Reg(const std::string & _reg);
        std::string allocReg(bool & writeback);
        void updateInfo(const std::string & _reg, const std::string & _var);
    };
}


#endif //PROJECT_CORE_MIPS_LOCALREGPOOL_H
