//
// Created by JacquesdeH on 2020/11/15.
//

#ifndef PROJECT_LOCALREGPOOL_H
#define PROJECT_LOCALREGPOOL_H

#include <queue>
#include <string>
#include <unordered_map>

namespace mips
{
    class LocalRegPool
    {
    private:
        std::queue <std::string> freePool;
        std::unordered_map<std::string, std::string> regs2var;
        std::unordered_map<std::string, std::string> var2reg;

    public:
        LocalRegPool();

    public:

    };
}


#endif //PROJECT_LOCALREGPOOL_H
