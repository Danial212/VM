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

//  Check if 2 string are equal, with the given length, and ignore the rest, case in-setsitive,
int StrEqul_with_length(char *str1, char *str2, int length)
{
    return strnicmp(str1, str2, length) == 0;
}

//  The length if a digit
int int_length(int num)
{
    num = abs(num);
    if (num == 0)
        return 1;
    return (floor(log10(num)) + 1);
}

// Slicing string from start point, to end point
char *slice_string(char *str, int start_point, int endPoint)
{
    if(endPoint < 0)
        endPoint += strlen(str);

    size_t size = (endPoint - start_point);
    char *sliced_string = malloc(sizeof(char) * size);

    strncpy(sliced_string, str + start_point, size);

    sliced_string[size] = '\0';

    return sliced_string;
}
