#ifndef __SOUND_H__
#define __SOUND_H__

typedef struct sndStaticTag {
    u16 *basePtr;
    u16 *playPtr;
    u8 time;
    u8 sfxNum;
}SND_STATIC;

extern SND_STATIC sndStatic;

extern void snd_init(void);
extern void snd_play(u8 num);

#endif
