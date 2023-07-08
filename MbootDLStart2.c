#include <Agb.h>

#include "Mboot.h"
extern u16 RfuMbootDLStart(u8 param_1, u8 param_2, u16 param_3, u16 *GameID, u32 param_5);
extern struct Mboot Mboot;

u16 MbootDLStart2(u8 Peer, u16 param_2)
{
    return RfuMbootDLStart(0x40, 1 << Peer, param_2, &Mboot.curGame.gameID, 0x1a);
}
