#include <Agb.h>

extern struct Sound {
    u16 *basePtr;
    u16 *playPtr;
    u8 time;
    u8 sfxNum;
} Sound;

void SoundMain(void)
{
    u8 i;
    vu16 *soundCnt;

    if (Sound.time == (u8)-1) return;

    if (Sound.time == 0) {
        for (;;) {
            Sound.time = *Sound.playPtr >> 8;
            soundCnt = (vu16 *)REG_SOUND1CNT;
            if (Sound.time == (u8)-2) {
                Sound.playPtr = Sound.basePtr;
                continue;
            }
            if (Sound.time == (u8)-1) {
                for (i = 0; i < 3; i++) *soundCnt++ = 0;
                return;
            }

            *(vu16 *)REG_SOUND1CNT_X = 0;
            for (i = 0; i < 3; i++) {
                *soundCnt = *Sound.playPtr;
                if (i < 2) {
                    soundCnt++;
                    Sound.playPtr++;
                }
            }
            *soundCnt = *Sound.playPtr++;
            break;
        }

    }
    Sound.time = Sound.time - 1;
}
