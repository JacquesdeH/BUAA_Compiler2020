//
// Created by JacquesdeH on 2020/11/16.
//

#ifndef PROJECT_SYMBOLINFO_H
#define PROJECT_SYMBOLINFO_H


namespace mips
{
    class SymbolInfo
    {
    private:
        int memOffset;
        int atomSize;

    public:
        SymbolInfo(const int & _memOffset, const int & _atomSize);

    public:
        int getMemOffset() const;
        int getAtomSize() const;
    };
}


#endif //PROJECT_SYMBOLINFO_H
