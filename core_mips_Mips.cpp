//
// Created by JacquesdeH on 2020/11/15.
//

#include "config.h"
#include "core_mips_Mips.h"

mips::Mips::Mips()
{

}

void mips::Mips::insertCodePiece(const mips::ObjCodes &piece)
{
    this->mipscode.mergeCodes(piece);
}
