#include <windows.h>

int MyPrintf(const void *args, ...)
{
    HANDLE WriteHandle = GetStdHandle(STD_OUTPUT_HANDLE);

#ifdef UNICODE
    int FormatStringLength = lstrlenW((const WCHAR *)args) * 2 + 1;

#else
    int FormatStringLength = lstrlenA((const CHAR *)args);

#endif

    int WriteLength = 0;
    int NextArgument = 1;

    for (int x = 0; x != FormatStringLength; x++)
    {
        if(((const char *)*(&args))[x] == 0)
        {
            //end of string
            break;
        }

        if (((const char *)*(&args))[x] == '%')
        {
            if (((const char *)*(&args))[x + 1] == 's')
            {

#ifdef UNICODE
                WriteLength = lstrlenW((const WCHAR*)*(&args + NextArgument)) * 2 + 1;
#else
                WriteLength = lstrlenA((const char *)*(&args + NextArgument));
#endif

                WriteFile(WriteHandle, (const char *)*(&args + NextArgument), WriteLength, NULL, NULL);
            }

            if (((const char *)*(&args))[x + 1] == 'c')
            {

#ifdef UNICODE
                WriteLength = sizeof(WCHAR);
#else
                WriteLength = sizeof(CHAR);
#endif

                WriteFile(WriteHandle, (const char *)*(&args + NextArgument), WriteLength, NULL, NULL);
            }

            if (((const char *)*(&args))[x + 1] == 'x')
            {
                char HexString[8 + 1];
                HexString[8] = 0;

                int HexIndex = 0;

                int hex = (int)*(&args + NextArgument);

                for (int x = sizeof(hex) - 1; x != -1; --x)
                {
                    int FirstByte = ((const unsigned char *)&hex)[x];
                    FirstByte >>= sizeof(int);

                    int SecondByte = ((const unsigned char *)&hex)[x];
                    SecondByte -= (FirstByte << sizeof(int));

                    if(FirstByte >= 0xA)
                    {
                        FirstByte += 7;
                    }

                    if(SecondByte >= 0xA)
                    {
                        SecondByte += 7;
                    }

                    FirstByte += 0x30;
                    SecondByte += 0x30;

                    HexString[HexIndex++] = FirstByte;
                    HexString[HexIndex++] = SecondByte;
                }

                for(int x = 0; x != sizeof(HexString) - 1; x++)
                {
                    if(HexString[x] != '0')
                    {
                        break;
                    }
                    else 
                    {
                        HexString[x] = '-';
                    }
                }
                
#ifdef UNICODE
                WriteLength = lstrlenW( (const WCHAR*)HexString) * 2 + 1;
#else
                WriteLength = lstrlenA(HexString);
#endif

                for(int x = 0; x != sizeof(HexString) - 1; x++)
                {
                    if(HexString[x] != '-')
                    {
                        WriteFile(WriteHandle, &HexString[x], sizeof(HexString[x]), NULL, NULL);
                    }
                }
            }

            if (((const char *)*(&args))[x + 1] == 'p')
            {
                char HexString[2 + 1];
                HexString[2] = 0;

                int hex = (int)*(&args + NextArgument);

                for (int x = sizeof(hex) - 1; x != -1; --x)
                {
                    int FirstByte = ((const unsigned char *)&hex)[x];
                    FirstByte >>= sizeof(int);

                    int SecondByte = ((const unsigned char *)&hex)[x];
                    SecondByte -= (FirstByte << sizeof(int));

                    //printf("\ndata=>%x, %x\n", FirstByte, SecondByte);

                    if(FirstByte >= 0xA)
                    {
                        FirstByte += 7;
                    }

                    if(SecondByte >= 0xA)
                    {
                        SecondByte += 7;
                    }

                    FirstByte += 0x30;
                    SecondByte += 0x30;

                    HexString[0] = FirstByte;
                    HexString[1] = SecondByte;

                    WriteFile(WriteHandle, HexString, sizeof(HexString) - 1, NULL, NULL);
                }
            }

            NextArgument++;
            x++;
        }
        else 
        {
            WriteFile(WriteHandle, &((const char *)*(&args))[x], sizeof(char), NULL, NULL);
        }
    }

    return 0;
}
