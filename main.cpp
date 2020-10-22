
#include "core_lexic_Lexic.h"
#include "core_syntactic_Syntactic.h"
#include "utils_PeekableQueue.h"
#include "ErrorManager.h"

int main()
{
    PeekableQueue queue;
    symbol::SymbolManager symbolManager;
    error::ErrorManager errorManager("error.txt");

    lexic::Lexic instance_Lexic("testfile.txt", "output.txt", &queue, &errorManager);
    syntactic::Syntactic instance_Syntactic("output.txt", &queue, &symbolManager, &errorManager);

    instance_Lexic.run();
    instance_Syntactic.parseProgram();

    errorManager.printAllErrors();

    return 0;
}
