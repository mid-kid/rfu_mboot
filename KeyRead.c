#include <Agb.h>

extern struct Keys {
    u16 trg;
    u16 cont;
} Keys;

void KeyRead(void)
{
    u16 ReadData = *(vu16 *)REG_KEYINPUT ^ 0xffff;
    Keys.trg  = ReadData & (ReadData ^ Keys.cont);  // Trigger input
    Keys.cont = ReadData;                           // Hold input
}
