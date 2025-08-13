#ifndef COMMON_TOOLS_H
#define COMMON_TOOLS_H

// Function declarations
void DebugLog(const char *__format, ...);
int StrEqul(char *, char *);
int StrEqul_with_length(char *str1, char *str2, int length);
int int_length(int num);
char *slice_string(char *str, int start_point, int endPoint);

#endif
