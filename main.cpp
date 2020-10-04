
#include "core_lexic_Lexic.h"
#include "core_syntactic_Syntactic.h"

int main()
{
    PeekableQueue queue;
    symbol::SymbolManager symbolManager;

    lexic::Lexic instance_Lexic("testfile.txt", "output.txt", &queue);
    syntactic::Syntactic instance_Syntactic("output.txt", &queue, &symbolManager);

    instance_Lexic.run();
    instance_Syntactic.parseProgram();

    return 0;
}
