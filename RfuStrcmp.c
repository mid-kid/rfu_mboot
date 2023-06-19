#include <Agb.h>

u8 RfuStrcmp(char *Str1, char *Str2)
{
    while (*Str1) if (*Str1++ != *Str2++) return 1;
    return 0;
}
