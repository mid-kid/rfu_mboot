#ifndef __DATA_H__
#define __DATA_H__

// define data----------------------------------
typedef struct keyDataTag {
    u16 Trg;
    u16 Cont;
}KEY_DATA;

typedef struct msgdataTag {
    const u8 *const *msg;
    const u16 *pos;
}MSGDATA;

// extern data----------------------------------
extern u16 Bg0Bak[32*20];
extern void (*nowProcess)();
extern KEY_DATA key;

extern const u8 str_my_gname_mboot[10];
extern const u8 str_header_mboot[10];
extern const u8 *str_uname[];
extern const MSGDATA MenuMsgInitial[];
extern const u8 str_lang_en[];

#endif
