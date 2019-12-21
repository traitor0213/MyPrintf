#include <stdio.h>
#include <windows.h>


int MyPrintf(const void *args, ...)
{
    HANDLE WriteHandle = GetStdHandle(STD_OUTPUT_HANDLE);

    typedef char STR;
    int FormatStringLength = lstrlenA((STR *)args);

    int WriteLength = 0;
    int NextArgument = 1;

    for (int x = 0; x != FormatStringLength; x++)
    {       
        if (((STR *)*(&args))[x] == (STR)'%')
        {
            unsigned int ch = ((STR*)*(&args))[x + 1]; 

            if (ch == (STR)'s')
            {
                WriteLength = lstrlenA((STR *)*(&args + NextArgument));
                WriteFile(WriteHandle, (STR *)*(&args + NextArgument), WriteLength, NULL, NULL);
            }

            if (ch == (STR)'c')
            {
                WriteLength = sizeof(CHAR);
                WriteFile(WriteHandle, (STR *)*(&args + NextArgument), WriteLength, NULL, NULL);
            }

            if (ch == (STR)'x')
            {
                STR HexString[8 + 1];
                HexString[8] = 0;

                int HexIndex = 0;

                int hex = (int)*(&args + NextArgument);

                for (int x = sizeof(hex) - 1; x != -1; --x)
                {
                    int FirstByte = ((const unsigned char *)&hex)[x];
                    FirstByte >>= 4;

                    int SecondByte = ((const unsigned char *)&hex)[x];
                    SecondByte -= (FirstByte << 4);

                    if (FirstByte >= 0xA)
                    {
                        FirstByte += 7;
                    }

                    if (SecondByte >= 0xA)
                    {
                        SecondByte += 7;
                    }

                    FirstByte += 0x30;
                    SecondByte += 0x30;

                    HexString[HexIndex++] = (STR)FirstByte;
                    HexString[HexIndex++] = (STR)SecondByte;
                }

                for (int x = 0; x != sizeof(HexString) - 1; x++)
                {
                    if (HexString[x] != (STR)'0')
                    {
                        break;
                    }
                    else
                    {
                        HexString[x] = (STR)'-';
                    }
                }

                WriteLength = lstrlenA((STR *)HexString);
                for (int x = 0; x != sizeof(HexString) - 1; x++)
                {
                    if (HexString[x] != (STR)'-')
                    {
                        WriteFile(WriteHandle, &HexString[x], sizeof(HexString[x]), NULL, NULL);
                    }
                }
            }

            if (ch == (STR)'p')
            {
                char HexString[2 + 1];
                HexString[2] = 0;

                int hex = (int)*(&args + NextArgument);

                for (int x = sizeof(hex) - 1; x != -1; --x)
                {
                    int FirstByte = ((STR *)&hex)[x];
                    FirstByte >>= 4;

                    int SecondByte = ((STR *)&hex)[x];
                    SecondByte -= (FirstByte << 4);

                    if (FirstByte >= 0xA)
                    {
                        FirstByte += 7;
                    }

                    if (SecondByte >= 0xA)
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

            //add address

            NextArgument++;
            x++;
        }
        else 
        {
            WriteFile(WriteHandle, &((STR *)*(&args))[x], sizeof(STR), NULL, NULL);
        }
    }

    return 0;
}
