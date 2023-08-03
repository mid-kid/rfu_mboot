#ifndef __RFU_DEFINE_H__
#define __RFU_DEFINE_H__

#include "AgbRFU_LL.h"

#include "data.h"

extern u8 SearchMenuEnd;
extern u8 SearchMenuCursor;
extern u8 MbootPeer;
extern u8 SearchMenuErrorMsg;
extern u16 MbootBeaconID;
extern u16 SearchMenuTimer;
extern u8 blink_counter;
extern u32 DAT_0300569c;
extern rfuTgtData GameList[4];
extern u8 GameListBits;
extern u8 GameListBitsNew;

extern u8 SearchMenuErrorTimer;
extern u8 MenuBusy;
extern u8 Lang;
extern const MSGDATA *MenuMsg;
extern u8 my_state;
extern u8 MainMenuFadeOut;
extern char GameName[14];

extern void SEQ_title_init(void);
extern void SEQ_title(void);
extern void SEQ_search_init(void);
extern void SEQ_search(void);
extern void menu_initBlinkCounter(void);
extern void menu_blinkMessage(u8 Msg,u8 Rate);

#endif
