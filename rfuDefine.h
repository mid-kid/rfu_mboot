#ifndef __RFU_DEFINE_H__
#define __RFU_DEFINE_H__

#include "AgbRFU_LL.h"

#include "data.h"

// bss
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

// common
extern u8 SearchMenuErrorTimer;
extern u8 MenuBusy;
extern u8 Lang;
extern const MSGDATA *MenuMsg;
extern u8 my_state;
extern u8 MainMenuFadeOut;
extern char GameName[14];

// Main.c
extern void SEQ_title_init(void);
extern void SEQ_title(void);

// searchMenu.c
extern void SEQ_search_init(void);
extern void SEQ_search(void);

// menuFuncs.c
extern u8   menu_drawGameList(void);
extern void menu_clearGameList(void);
extern void menu_initGameList(void);
extern void menu_blinkGame(u8 blink);
extern void menu_checkError(u16 state);
extern void menu_playErrorSFX(void);
extern void menu_drawMessage(u8 msg,u16 pltt);
extern void menu_initBlinkCounter(void);
extern void menu_blinkMessage(u8 msg,u8 rate);
extern void menu_initGameName(void);

#endif
