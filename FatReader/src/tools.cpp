#include "tools.h"

#include <iostream>
#include <cstdlib>
#include <inttypes.h>

using namespace std;

int Tools::printDump(unsigned char * start, int size)
{
    uint8_t * i;
    printf("Dump:");
    uint32_t s = (intptr_t) start;

    for (i = (uint8_t *)((intptr_t)start & 0xFFFFFFF0);
    i < (uint8_t *)((intptr_t)start + size);
    i++)
    {
        if ((intptr_t)i % 16 == 0)
        {
            printf("\n");
            printf("%4X",(unsigned long)i - s);
            printf(": ");
        }
        if ((i >= start) && (i <= start + size))
            printf("%3X",*i);
        else
            printf("....");
            printf(" ");
    }
    printf("\n\n");
}


string Tools::split(string & strIn,char symb)
{
    string outStr;
    return outStr;
}
