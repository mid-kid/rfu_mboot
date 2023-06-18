#include <Agb.h>

#include "Mboot.h"
extern void FUN_03003e04(void);
extern void KeyRepeatHold(void);
extern void SoundPlaySfx(u8 Num);
extern void SearchMenuDrawList(u8 Blink);
extern void RfuWaitData(void);
extern void GameListInit(void);
extern void BgScClear(u16 Pos, u8 Height, u8 Width);
extern void FrameCountReset(void);
extern u8 SearchMenuCheckGames(void);
extern void SearchMenuClearGame(void);
extern void Sio32IntrProcSet(void *Func);
extern void Sio32IntrProc_Unk0e10(void);
extern u32 RfuMbootCfg(u32 param_1, u8 Client, void *Dest, u32 Size);
extern void MainMenuInit(void);
extern struct Mboot Mboot;
extern void MainMenu(void);
extern u16 *BgScSet(u16 Pos, u16 PlttNo, char *Srcp);
extern void MenuMsgBlink(u8 Msg, u8 Rate);
extern void MenuMsgSet(u8 Msg, u16 PlttNo);
extern void SearchMenuErrorBeep(void);
extern void SearchMenuErrorCheck(u16 State);
void SearchMenuMbootStart(void);
void SearchMenuMbootDL(void);
extern u8 MenuState;
extern u8 SearchMenuErrorTimer;
extern u8 SearchMenuErrorMsg;
extern u8 SearchMenuCursor;
extern struct GameInfo GameList[4];
extern u16 MbootBeaconID;
extern u8 SearchMenuEnd;
extern u8 MenuBusy;
extern u16 (*SearchProcTable[])(void);
extern u8 SearchMenuFoundGames;
extern u16 SearchMenuTimer;
extern u8 FrameCount;
extern struct RfuBuf {
    u8 *recv;
    u8 *send;
} RfuBuf;
extern u8 MbootPeer;
extern void (*Proc)();

extern struct Keys {
    u16 trg;
    u16 cont;
} Keys;

enum SearchMenuState {
    // Correspond to entries in SearchProcTable
    SEARCH_START,
    SEARCH_RADIOCFG,
    SEARCH_SETGAMEINFO,
    SEARCH_DISCOSTART,
    SEARCH_0x1d,
    SEARCH_DISCORES,
    SEARCH_CONNECT,
    SEARCH_CONNCHECK,
    SEARCH_CONNREADY,
    SEARCH_WAITDATA,
    SEARCH_BOOT,
    SEARCH_END,
    SEARCH_STATUS,
    SEARCH_0x13,

    // Standalone states
    SEARCH_SELECT_DISCO = 0x84,
    SEARCH_MBOOT_START = 0x90,
    SEARCH_MBOOT_START_CHECK = 0x91,
    SEARCH_SELECT = 0x92,
    SEARCH_MBOOT_DL_START = 0xa0,
    SEARCH_MBOOT_DL = 0xa1,
    SEARCH_ERROR_RESTART = 0xc1,
    SEARCH_ERROR_END = 0xc2,
    SEARCH_ERROR_REBOOT = 0xc5,
    SEARCH_MBOOT_DL_COMPLETE = 0xc6,
    SEARCH_MBOOT_EXEC = 0xc7
};

void SearchMenu(void)
{
    u16 procRes = 0;

    if (Mboot.mode == 0) {
        FUN_03003e04();

        if (MenuState != SEARCH_MBOOT_DL_COMPLETE) {
            *(vu16 *)REG_IME = 0;
            SearchMenuErrorTimer++;
            if (SearchMenuErrorTimer > 4 * 60) {
                SearchMenuErrorTimer = 0;
                MenuState = SEARCH_BOOT;
                SearchMenuErrorMsg = 4;  // ERROR OCCURRED!
            }
            *(vu16 *)REG_IME = 1;
        }
    }

    if (MenuState == SEARCH_SELECT_DISCO || MenuState == SEARCH_SELECT) {
        KeyRepeatHold();

        // Move the cursor
        if (Keys.trg & (U_KEY | D_KEY)) {
            if (Keys.trg & U_KEY) {
                SearchMenuCursor--;
                if (SearchMenuCursor & 0x80) SearchMenuCursor = 3;
            } else {
                SearchMenuCursor++;
                if (SearchMenuCursor > 3) SearchMenuCursor = 0;
            }
            *(vu16 *)REG_BG2VOFS = ~(SearchMenuCursor * 16 + 51);
            SoundPlaySfx(0);
        }

        // Select game
        if (Keys.trg & A_BUTTON && GameList[SearchMenuCursor].beaconID) {
            SearchMenuDrawList(FALSE);
            MbootBeaconID = GameList[SearchMenuCursor].beaconID;
            if (MenuState == SEARCH_SELECT_DISCO) {
                MenuState = SEARCH_DISCORES;
            } else {
                MenuState = SEARCH_CONNECT;
            }
            SoundPlaySfx(2);
        }
    }

    if (Keys.trg & B_BUTTON) {
        if (MenuState == SEARCH_MBOOT_DL_START) {
            RfuWaitData();
            MenuState = SEARCH_START;
            GameListInit();
            SoundPlaySfx(3);
        } else if (!SearchMenuEnd &&
                !MenuBusy &&
                SearchMenuErrorMsg == (u8)-1 &&
                MenuState != SEARCH_BOOT &&
                MenuState != SEARCH_MBOOT_START_CHECK) {
            RfuWaitData();
            MenuState = SEARCH_START;
            SearchMenuEnd = TRUE;
            SoundPlaySfx(3);
        }
    }

    if (!(MenuState & 0x80)) procRes = SearchProcTable[MenuState]();

    switch (MenuState) {
    case SEARCH_BOOT:
        // Resets and boots the adapter
        if (procRes == 0) MenuState = SEARCH_START;
        break;

    case SEARCH_START:
        if (procRes == 0) {
            SearchMenuErrorMsg = -1;
            if (SearchMenuEnd != FALSE) {
                MenuState = SEARCH_END;
            } else {
                MenuState = SEARCH_RADIOCFG;
                BgScClear(0x6b, 1, 8);
            }
        }
        break;

    case SEARCH_RADIOCFG:
        if (procRes == 0) {
            MenuState = SEARCH_DISCOSTART;
            FrameCountReset();
            SearchMenuFoundGames = 0;
        }
        break;

    case SEARCH_DISCOSTART:
        // Starts game discovery
        if (procRes == 0) {
            MenuState = SEARCH_SELECT_DISCO;
            MbootBeaconID = 0;
            SearchMenuTimer = 1 * 60;
        }
        break;

    case SEARCH_SELECT_DISCO:
        // Wait one second for discovery to finish
        if (SearchMenuFoundGames) {
            MenuMsgSet(9, 0);  // SELECT A GAME
        } else {
            MenuMsgBlink(8, 0x40);  // NOW SEARCHING...
        }

        SearchMenuTimer--;
        if (SearchMenuTimer == 0) MenuState = SEARCH_DISCORES;
        break;

    case SEARCH_DISCORES:
        // Get discovery results
        if (procRes == 0) {
            if (SearchMenuTimer < 60 - 8) {
                SearchMenuFoundGames = SearchMenuCheckGames();
            }

            if (MbootBeaconID) {
                MenuState = SEARCH_CONNECT;
            } else {
                MenuState = SEARCH_SELECT;
                SearchMenuTimer = 5 * 60;
                FrameCount = 0;
            }
        }
        break;

    case SEARCH_SELECT:
        // Allow the player to select a game
        // Restart discovery after 5 seconds
        if (SearchMenuFoundGames != 0) {
            MenuMsgSet(9, 0);  // SELECT A GAME
            if (SearchMenuTimer > 3 * 60) {
                SearchMenuDrawList(TRUE);
            } else {
                SearchMenuDrawList(FALSE);
            }
        } else {
            MenuMsgBlink(8, 0x40);  // NOW SEARCHING...
        }

        SearchMenuTimer--;
        if (SearchMenuTimer == 0) MenuState = SEARCH_DISCOSTART;
        break;

    case SEARCH_CONNECT:
        // Start connecting to a game
        if (procRes == 0) {
            BgScClear(0x200, 2, 0x20);
            SearchMenuTimer = 2 * 60;
            MenuState = SEARCH_CONNCHECK;
        } else if (procRes == 0x900) {
            procRes = 0;
            SearchMenuErrorMsg = 0;  // CONNECTION ATTEMPT FAILED!
            MenuState = SEARCH_ERROR_RESTART;
        }
        break;

    case SEARCH_CONNCHECK:
        // Wait a maximum of two seconds for the connection to complete
        if (procRes == 0) {
            MenuState = SEARCH_CONNREADY;
            break;
        }

        SearchMenuTimer--;
        if (procRes & 0x8000) {
            if ((procRes & 0xff) == 2) SearchMenuTimer = 0;
            procRes = 0;
        }
        if (SearchMenuTimer == 0) MenuState = SEARCH_CONNREADY;
        break;

    case SEARCH_CONNREADY:
        // Initialize multiboot download
        if (procRes == 0) {
            if (SearchMenuTimer > 0 && RfuBuf.recv[7] == 0) {
                SearchMenuClearGame();
                Sio32IntrProcSet(Sio32IntrProc_Unk0e10);
                RfuMbootCfg(0x20, MbootPeer, (void *)EX_WRAM, EX_WRAM_SIZE);
                MenuState = SEARCH_SETGAMEINFO;
            } else {
                SearchMenuErrorMsg = 0;  // CONNECTION ATTEMPT FAILED!
                MenuState = SEARCH_ERROR_RESTART;
            }
        }
        break;

    case SEARCH_SETGAMEINFO:
        // Set the timer for the next step
        if (procRes == 0) {
            SearchMenuTimer = 0.5 * 60;
            MenuState = SEARCH_STATUS;
        }
        break;

    case SEARCH_STATUS:
        // Check if peer is still connected
        if (procRes == 0) {
            if (RfuBuf.recv[4 + MbootPeer]) {
                MenuState = SEARCH_WAITDATA;
                BgScSet(0x6b, 2, Mboot.curGame.userName);
                break;
            }
        }

        SearchMenuTimer--;
        if (SearchMenuTimer == 0) {
            SearchMenuErrorMsg = 0;  // CONNECTION ATTEMPT FAILED!
            MenuState = SEARCH_ERROR_RESTART;
        }
        break;

    case SEARCH_WAITDATA:
        if (procRes == 0) {
            FrameCountReset();
            MenuState = SEARCH_MBOOT_START;
        }
        break;

    case SEARCH_END:
        // Return to main menu
        if (procRes == 0) {
            MainMenuInit();
            Proc = MainMenu + 1;
        }
        break;

    case SEARCH_ERROR_RESTART:
    case SEARCH_ERROR_END:
    case SEARCH_ERROR_REBOOT:
        MenuMsgSet(SearchMenuErrorMsg, 1);
        SearchMenuErrorBeep();

        BgScClear(0x243, 1, 0x18);
        GameListInit();

        if (MenuState == SEARCH_ERROR_RESTART) {
            MenuState = SEARCH_START;
        } else if (MenuState == SEARCH_ERROR_REBOOT) {
            MenuState = SEARCH_BOOT;
        } else {
            // MenuState == SEARCH_ERROR_END
            SoundPlaySfx(3);
            MainMenuInit();
            Proc = MainMenu + 1;
        }
        break;
    }

    SearchMenuMbootStart();
    SearchMenuMbootDL();
    SearchMenuErrorCheck(procRes);
}

struct RfuPeer {
    u16 sub[2][0x1a];
    void *mbootDest;
    u32 mbootSize;
};

extern u32 FUN_03003078(u8 param_1, u8 Peer);
extern u16 MbootDLStart(u8 Peer, u16 param_2);
extern struct RfuPeer RfuPeers[4];

void SearchMenuMbootStart(void)
{
    switch (MenuState) {
    case SEARCH_MBOOT_START:
        MbootDLStart(MbootPeer, 8);
        MenuState = SEARCH_MBOOT_START_CHECK;
        break;

    case SEARCH_MBOOT_START_CHECK:
        if (RfuPeers[MbootPeer].sub[0][0] == 0x27) {
            MenuState = SEARCH_MBOOT_DL_START;
            FUN_03003078(4, MbootPeer);
        }
        if (RfuPeers[MbootPeer].sub[0][1] > 0xfa) {
            SearchMenuErrorMsg = 0;  // CONNECTION ATTEMPT FAILED!
            MenuState = SEARCH_ERROR_REBOOT;
            FUN_03003078(4, MbootPeer);
        }

        if (MenuState != SEARCH_MBOOT_START_CHECK) FrameCountReset();
    }
}

extern u8 RfuStrcmp(const char *Str1, const char *Str2);
extern const u8 GameLogoInitial[10];

void SearchMenuMbootDL(void)
{
    u16 *data;

    data = RfuPeers[MbootPeer].sub[1];

    switch (MenuState) {
    case SEARCH_MBOOT_DL_START:
        MenuMsgBlink(10, 0x40);  // WAITING FOR DATA...
        if ((data[0] & 0x8000) != 0) {
            SoundPlaySfx(4);
            FrameCountReset();
            MenuState = SEARCH_MBOOT_DL;
            MenuBusy = TRUE;
        } else if (data[0] == 0x49) {
            SearchMenuErrorMsg = 1;  // DOWNLOAD FAILED!
            MenuState = SEARCH_ERROR_REBOOT;
        }
        break;

    case SEARCH_MBOOT_DL:
        MenuMsgBlink(0xb, 0x20);  // DOWNLOADING...
        if (data[0] == 0x47) {
            if (RfuStrcmp(GameLogoInitial, (void *)0x02000004) == 0) {
                SoundPlaySfx(5);
                SearchMenuTimer = 0x78;
                MenuMsgSet(0xc, 2);  // DOWNLOAD COMPLETED!
                MenuState = SEARCH_MBOOT_DL_COMPLETE;
            } else {
                SearchMenuErrorMsg = 2;  // INVALID DATA RECEIVED!
                MenuState = SEARCH_ERROR_RESTART;
            }
            FUN_03003078(0xc, MbootPeer);
            RfuWaitData();
        } else if (data[1] > 0xfa || data[0] == 0x49) {
            SearchMenuErrorMsg = 1;  // DOWNLOAD FAILED!
            MenuState = SEARCH_ERROR_REBOOT;
        }
        break;

    case SEARCH_MBOOT_DL_COMPLETE:
        SearchMenuTimer--;
        if (SearchMenuTimer == 0) MenuState = SEARCH_MBOOT_EXEC;
        break;
    }

    if (MenuState == SEARCH_MBOOT_EXEC) {
        u16 x;
        u16 sum;
        u16 *ptr;

        CpuCopy(&Mboot, 0x03000000, sizeof(Mboot), 16);
        CpuCopy(GameLogoInitial, 0x030000f0, sizeof(GameLogoInitial), 16);

        ptr = (void *)0x03000000;
        sum = 0;
        for (x = 0; x < 0x5a; x++) {
            sum += *ptr++;
        }

        *(u16 *)0x030000fa = sum;
        *(vu16 *)REG_IME = 0;

        *(vu8 *)SOFT_RESET_DIRECT_BUF = 1;
        SoftReset(0xdc);
    }
}
