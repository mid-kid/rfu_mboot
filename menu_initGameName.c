#include <Agb.h>

extern u8 GameName[14];

static const u8 str_my_gname_mboot[] = "RFU-MB-DL";
const u8 str_header_mboot[] = "RFU-MBOOT";

void menu_initGameName(void)
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
        if (i < sizeof(str_my_gname_mboot)) {
            GameName[i] = str_my_gname_mboot[i];
        } else {
            GameName[i] = 0;
        }
    }

    // If a valid game is inserted, append the game code
    if (*cst == sum) {
        cst = (vu8 *)(ROM_BANK0 + 0xac);
        for (i = sizeof(str_my_gname_mboot);
                i < sizeof(GameName); i++) {
            GameName[i] = *cst++;
        }
    }
}
