#undef UNICODE
#include "winprintf.h"

int Test(const char *args, ...)
{
    for (int x = 0;; x++)
    {
        if (((char *)*(&args + x))[0] == 0)
        {
            break;
        }

        MyPrintf("%s\n", ((char *)*(&args + x)));
    }

    return 0;
}

int main()
{
    Test("Hello", "World!!\n", 0);

    return 0;
}