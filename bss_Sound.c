#include <Agb.h>

__attribute__((nocommon)) struct Sound {
    u16 *basePtr;
    u16 *playPtr;
    u8 time;
    u8 sfxNum;
} Sound;
