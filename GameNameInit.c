#include <Agb.h>

extern u8 GameName[14];
extern u8 GameNameInitial[10] = "RFU-MB-DL";

void GameNameInit(void)
{
    u8 i;
    u8 sum;
    vu8 *cst;

    // Calculate rom checksum
    sum = 0x19;
    cst = (vu8 *)(ROM_BANK0 + 0xa0);
    while (cst != (vu8 *)(ROM_BANK0 + 0xbd)) sum += *cst++;
    sum = ~sum + 1;

    // Initialize the game name
    for (i = 0; i < sizeof(GameName); i++) {
        if (i < sizeof(GameNameInitial)) {
            GameName[i] = GameNameInitial[i];
        } else {
            GameName[i] = 0;
        }
    }

    // If a valid game is inserted, append the game code
    if (*cst == sum) {
        cst = (vu8 *)(ROM_BANK0 + 0xac);
        for (i = sizeof(GameNameInitial);
                i < sizeof(GameName); i++) {
            GameName[i] = *cst++;
        }
    }
}
