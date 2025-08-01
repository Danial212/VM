#include "CommonTools.h"
#include "GlobalVariables.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Enable In-Programm debuging, if the 'DEBUG_LOG' is True(1)
void DebugLog(const char *__format, ...)
{
    if (DEBUG_LOG)
        printf(__format);
}

//  Check if 2 string are equal, case in-setsitive
int StrEqul(char *str1, char *str2)
{
    return stricmp(str1, str2) == 0;
}

//  The length if a digit
int int_length(int num)
{
    num = abs(num);
    if (num == 0)
        return 1;
    return (floor(log10(num)) + 1);
}
