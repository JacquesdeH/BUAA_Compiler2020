//
// Created by JacquesdeH on 2020/12/14.
//

#include <cstdio>

int main()
{
    const unsigned shift = 17;
    for (int i=0; i<=100000; i++)
        for (int j = 1; j < 100000; ++j)
        {
        	if (j == 0)
			continue; 
            int ans = i / j;
            bool neg = (i < 0);
            if (neg) i = -i;
            unsigned int magic = ((1<<shift) + j-1) / j;
            int out = ((unsigned )i * magic) >> shift;
            if (neg) out = -out;
            int equal = (int) (ans == out);
            if (neg) i = -i;
            if (equal == 0)
            	printf("i=%d j=%d ans=%d out=%d equal=%d\n", i, j, ans, out, equal);
        }
    return 0;
}

