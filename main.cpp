
#include "core_lexic_Lexic.h"

int main()
{
    lexic::Lexic instance_Lexic("testfile.txt", "output.txt");
    instance_Lexic.run();
    return 0;
}
