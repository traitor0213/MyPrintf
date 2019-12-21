#undef UNICODE
#include "winprintf.h"

int main()
{
    CHAR array[] = "hello";

    MyPrintf("%s, %x\n", array, array);


    return 0;
}