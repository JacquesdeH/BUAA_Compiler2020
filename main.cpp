
#include "core_lexic_Lexic.h"
#include "core_syntactic_Syntactic.h"
#include "utils_PeekableQueue.h"
#include "error_ErrorManager.h"
#include "core_mips_Mips.h"

int main()
{
    PeekableQueue queue;
    symbol::SymbolManager symbolManager;
    semantic::Semantic semanticGenerator;
    error::ErrorManager errorManager(&semanticGenerator, "error.txt");

    lexic::Lexic instance_Lexic("testfile.txt", "output.txt", &queue, &errorManager);
    syntactic::Syntactic instance_Syntactic("output.txt", &queue, &symbolManager, &errorManager, &semanticGenerator);

    instance_Lexic.run();
    instance_Syntactic.parseProgram();

    errorManager.printAllErrors();

    inter::MIR mir = semanticGenerator.getMir();
    mir.buildBlocks();
    mips::Mips objDumper;

    mips::ObjCodes objCodes = mips::Mips::compile(mir);
    objCodes.print("mips.txt");

    return 0;
}
