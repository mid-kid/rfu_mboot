#include <Agb.h>

struct sndStaticTag {
    u16 *basePtr;
    u16 *playPtr;
    u8 time;
    u8 sfxNum;
} sndStatic;
extern u16 *SoundSfxTable[];
void snd_stop(void);

void snd_init(void)
{
    *(vu16 *)REG_SOUNDCNT_X = SOUND_DMG_ON;
    *(vu16 *)REG_SOUNDCNT_L =
        SOUND_DMG_ALL_SO_ON << 8 |
        7 << SOUND_SO1_LEVEL_SHIFT |
        7 << SOUND_SO2_LEVEL_SHIFT;
    *(vu16 *)REG_SOUNDCNT_H = SOUND_DMG_MIX_FULL;
    *(vu16 *)REG_SOUNDBIAS = 3 << 14 | 0x200;
    snd_stop();
}

void snd_syncVBlank(void)
{
    u8 i;
    vu16 *soundCnt;

    if (sndStatic.time == (u8)-1) return;

    if (sndStatic.time == 0) {
        for (;;) {
            sndStatic.time = *sndStatic.playPtr >> 8;
            soundCnt = (vu16 *)REG_SOUND1CNT;
            if (sndStatic.time == (u8)-2) {
                sndStatic.playPtr = sndStatic.basePtr;
                continue;
            }
            if (sndStatic.time == (u8)-1) {
                for (i = 0; i < 3; i++) *soundCnt++ = 0;
                return;
            }

            *(vu16 *)REG_SOUND1CNT_X = 0;
            for (i = 0; i < 3; i++) {
                *soundCnt = *sndStatic.playPtr;
                if (i < 2) {
                    soundCnt++;
                    sndStatic.playPtr++;
                }
            }
            *soundCnt = *sndStatic.playPtr++;
            break;
        }
    }

    sndStatic.time--;
}

void snd_play(u8 Num)
{
    if (sndStatic.sfxNum > Num && sndStatic.time != (u8)-1) return;
    sndStatic.playPtr = SoundSfxTable[Num];
    sndStatic.basePtr = sndStatic.playPtr;
    sndStatic.time = 0;
    sndStatic.sfxNum = Num;
}

void snd_stop(void)
{
    sndStatic.sfxNum = 0;
    sndStatic.time = -1;
}
