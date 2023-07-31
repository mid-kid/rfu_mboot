#include <Agb.h>

struct Sound {
    u16 *basePtr;
    u16 *playPtr;
    u8 time;
    u8 sfxNum;
} Sound;
extern u16 *SoundSfxTable[];
void SoundStop(void);

void SoundInit(void)
{
    *(vu16 *)REG_SOUNDCNT_X = SOUND_DMG_ON;
    *(vu16 *)REG_SOUNDCNT_L =
        SOUND_DMG_ALL_SO_ON << 8 |
        7 << SOUND_SO1_LEVEL_SHIFT |
        7 << SOUND_SO2_LEVEL_SHIFT;
    *(vu16 *)REG_SOUNDCNT_H = SOUND_DMG_MIX_FULL;
    *(vu16 *)REG_SOUNDBIAS = 3 << 14 | 0x200;
    SoundStop();
}

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

    Sound.time--;
}

void SoundPlaySfx(u8 Num)
{
    if (Sound.sfxNum > Num && Sound.time != (u8)-1) return;
    Sound.playPtr = SoundSfxTable[Num];
    Sound.basePtr = Sound.playPtr;
    Sound.time = 0;
    Sound.sfxNum = Num;
}

void SoundStop(void)
{
    Sound.sfxNum = 0;
    Sound.time = -1;
}
