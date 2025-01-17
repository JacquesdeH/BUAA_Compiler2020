//
// Created by JacquesdeH on 2020/11/16.
//

#ifndef PROJECT_CORE_MIPS_SYMBOLINFO_H
#define PROJECT_CORE_MIPS_SYMBOLINFO_H


namespace mips
{
    class SymbolInfo
    {
    private:
        int memOffset; // global->offset_from_$gp and local->offset_from_$sp
        int atomSize;

    public:
        SymbolInfo(const int &_memOffset, const int &_atomSize);

    public:
        int getMemOffset() const;
        int getAtomSize() const;
    };
}


#endif //PROJECT_CORE_MIPS_SYMBOLINFO_H
