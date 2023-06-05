#include <Agb.h>

extern struct Sound {
    u16 *basePtr;
    u16 *playPtr;
    u8 time;
    u8 sfxNum;
} Sound;

extern u16 *SoundSfxTable[];

void SoundPlaySfx(u8 Num)
{
    if (Sound.sfxNum > Num && Sound.time != (u8)-1) return;
    Sound.playPtr = SoundSfxTable[Num];
    Sound.basePtr = Sound.playPtr;
    Sound.time = 0;
    Sound.sfxNum = Num;
}
