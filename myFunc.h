#ifndef __MY_FUNC_H__
#define __MY_FUNC_H__

#include <AgbTypes.h>

// define data----------------------------------

// extern data----------------------------------

// function's prototype declaration-------------
extern void mf_readKey(void);
extern void mf_rapidKey(void);
extern void mf_drawBg2_title(void);
extern void mf_drawBg2_search(u8 len);
extern u16 *mf_drawString(u16 pos,u16 color,const u8 *str);
extern void mf_clearRect(u16 topPos,u8 height,u8 width);
extern void mf_wait3sec(void);
extern void mf_winInit(void);
extern void mf_winFade(u8 dir);

#endif

