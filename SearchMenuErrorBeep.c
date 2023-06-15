#include <Agb.h>

extern void SoundPlaySfx(u8 Num);

extern struct Sound {
    u16 *basePtr;
    u16 *playPtr;
    u8 time;
    u8 sfxNum;
} Sound;

void SearchMenuErrorBeep(void)
{
    u8 x;
    u8 count;

    count = 0;
    SoundPlaySfx(6);
    for (x = 0; x < 3 * 60; x++) {
        VBlankIntrWait();

        // Play the SFX twice
        if (Sound.time == (u8)-1 && count < 1) {
            count++;
            SoundPlaySfx(6);
        }
    }
}
