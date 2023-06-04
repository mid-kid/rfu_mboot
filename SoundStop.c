#include <Agb.h>

extern struct Sound {
    u16 *basePtr;
    u16 *playPtr;
    u8 time;
    u8 sfxNum;
} Sound;

void SoundStop(void)
{
    Sound.sfxNum = 0;
    Sound.time = -1;
}
