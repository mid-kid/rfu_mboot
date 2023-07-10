#include <Agb.h>

extern struct key {
    u16 trg;
    u16 cont;
} key;

void KeyRead(void)
{
    u16 ReadData = *(vu16 *)REG_KEYINPUT ^ 0xffff;
    key.trg  = ReadData & (ReadData ^ key.cont);  // Trigger input
    key.cont = ReadData;                           // Hold input
}
