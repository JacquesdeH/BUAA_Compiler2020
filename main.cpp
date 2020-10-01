
#include "core_lexic_Lexic.h"

int main()
{
    PeekableQueue* queue;
    lexic::Lexic instance_Lexic("testfile.txt", "output.txt");

    queue = instance_Lexic.run();

    delete queue;
    return 0;
}
