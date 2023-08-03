#ifndef _AGBRFU_SIO32_ID_H
#define _AGBRFU_SIO32_ID_H

#define SIO32ID_MODULE_ID 0x8001

typedef struct {
    u8 Type;
    u8 State;
    u16 SendCheckCount;
    u16 SendBak;
    u16 RecvBak;
    u16 ConnectionCount;
    u16 Connected;
}Sio32IDArea;

#endif
