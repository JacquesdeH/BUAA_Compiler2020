//
// Created by JacquesdeH on 2020/11/15.
//

#include "config.h"
#include "core_mips_Mips.h"

mips::Mips::Mips()
{

}

mips::ObjCodes mips::Mips::compile(const inter::MIR & mir)
{
    mir.assertSeal();

    mips::ObjCodes dataSegment;
    dataSegment.mergeCodes(mir.compileStrings());
    dataSegment.mergeCodes(mir.compileGlobals());

    mips::ObjCodes textSegment = mir.compileProcs();

    mips::ObjCodes ret;
    ret.insertCode(".data");
    ret.mergeCodes(dataSegment);
    ret.insertCode(".text");
    ret.mergeCodes(textSegment);

    return ret;
}
