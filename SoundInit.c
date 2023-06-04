#include <Agb.h>

extern void SoundStop(void);

void SoundInit(void)
{
    *(vu16 *)REG_SOUNDCNT_X = SOUND_DMG_ON;
    *(vu16 *)REG_SOUNDCNT_L = SOUND_DMG_ALL_SO_ON << 8 |
        7 << SOUND_SO1_LEVEL_SHIFT |
        7 << SOUND_SO2_LEVEL_SHIFT;
    *(vu16 *)REG_SOUNDCNT_H = SOUND_DMG_MIX_FULL;
    *(vu16 *)REG_SOUNDBIAS = 3 << 14 | 0x200;
    SoundStop();
}
