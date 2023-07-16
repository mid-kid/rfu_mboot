#include <Agb.h>
#include "myFunc.h"

#include "Mboot.h"
#include "GameInfo.h"
extern void SoundPlaySfx(u8 Num);
extern void SearchMenuDrawGame(u16 Pos, struct GameInfo *Game);
extern struct Mboot Mboot;
extern struct GameInfo GameList[4];
extern u8 GameListBitsNew;

u8 SearchMenuUpdateGames(void)
{
    u8 x;
    u8 y;
    u8 GamesOld;
    u8 GamesNew;
    u16 Pos;

    // Find out which games are already in the list, and which ones aren't
    GamesOld = 0;
    GamesNew = 0;
    for (x = 0; x < Mboot.gamesCount; x++) {
        if (Mboot.games[x].beaconID &&
                Mboot.games[x].isMultiboot &&
                Mboot.games[x].playerNum < 4) {

            for (y = 0; y < 4; y++) {
                if (GameList[y].beaconID != Mboot.games[x].beaconID) continue;
                GamesOld |= 1 << y;
                break;
            }
            if (y == 4) {
                GamesNew |= 1 << x;
            }
        }
    }

    // Remove any games that don't exist anymore from the list
    for (x = 0; x < 4; x++) {
        if (!(GamesOld & 1 << x)) GameList[x].beaconID = 0;
    }

    // Copy new games into the games list
    GameListBitsNew = 0;
    for (x = 0; x < 4; x++) {
        if (!(GamesNew & 1 << x)) continue;
        for (y = 0; y < 4; y++) {
            if (GameList[y].beaconID) continue;
            CpuCopy(&Mboot.games[x], &GameList[y], sizeof(struct GameInfo), 16);
            GameListBitsNew |= 1 << y;
            break;
        }
    }

    // If any new game was found, play a ditty
    if (GameListBitsNew != 0) SoundPlaySfx(1);

    // Draw the game list
    mf_clearRect(0xe3, 8, 0x19);
    Pos = 0xe3;
    for (x = 0; x < 4; x++) {
        if (GameList[x].beaconID != 0) SearchMenuDrawGame(Pos, &GameList[x]);
        Pos += 0x40;
    }
    return GamesOld | GamesNew;
}
