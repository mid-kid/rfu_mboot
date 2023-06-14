#include <Agb.h>

struct MenuMsg {
    const char *const *msg;
    const u16 *pos;
};

static const char *const MsgEnglish[] = {
    "CONNECTION ATTEMPT FAILED!",
    "DOWNLOAD FAILED!",
    "INVALID DATA RECEIVED!",
    "DISCONNECTED!",
    "ERROR OCCURRED!",
    "",
    "PRESS THE A BUTTON",
    "",
    "NOW SEARCHING...",
    " SELECT A GAME THEN         PRESS THE A BUTTON TO CONNECT",
    "WAITING FOR DATA...",
    "DOWNLOADING...",
    "DOWNLOAD COMPLETED!"
};

static const char MsgJapanese_ConnectionAttemptFailed[] =
    {0x93, 0x97, 0x10, 0x8d, 0x20, 0x91, 0x84, 0x1b, 0x87, 0x21, 0x00};
static const char MsgJapanese_DownloadFailed[] =
    {0xfc, 0xb3, 0xdd, 0xdb, 0xb0, 0x01, 0x20, 0x91, 0x84, 0x1b, 0x87, 0x21, 0x00};
static const char MsgJapanese_InvalidDataReceived[] =
    {0xff, 0xb0, 0xc0, 0x07, 0xe0, 0x93, 0x87, 0x14, 0x92, 0xa1, 0x00};
static const char MsgJapanese_Disconnected[] =
    {0x93, 0x97, 0x10, 0x8d, 0x07, 0x8c, 0xee, 0xe3, 0x91, 0x95, 0x21, 0x00};
static const char MsgJapanese_ErrorOccurred[] =
    {0xb4, 0xd7, 0xb0, 0x21, 0x00};
static const char MsgJapanese_PressTheAButton[] =
    "PRESS THE A BUTTON";
static const char MsgJapanese_NowSearching[] =
    {0x97, 0x88, 0x91, 0xf1, 0x86, 0x87, 0x98, 0x7b, 0x90, 0x07, 0x91, 0x98, 0x87, 0xe3, 0x92, 0x2e, 0x2e, 0x2e, 0x00};
static const char MsgJapanese_SelectAGame[] =
    {0xa2, 0x41, 0x06, 0xc0, 0xdd, 0xa3, 0x14, 0x93, 0x97, 0x10, 0x8d, 0x91, 0xe3, 0x92, 0xa1, 0x00};
static const char MsgJapanese_WaitingForData[] =
    {0xff, 0xb0, 0xc0, 0x7b, 0xe3, 0x84, 0x98, 0x87, 0xe3, 0x92, 0x2e, 0x2e, 0x2e, 0x00};
static const char MsgJapanese_Downloading[] =
    {0xfc, 0xb3, 0xdd, 0xdb, 0xb0, 0x01, 0x96, 0x82, 0x88, 0x2e, 0x2e, 0x2e, 0x00};
static const char MsgJapanese_DownloadCompleted[] =
    {0xfc, 0xb3, 0xdd, 0xdb, 0xb0, 0x01, 0x93, 0x87, 0x8f, 0x88, 0x21, 0x00};

static const char *const MsgJapanese[] = {
    MsgJapanese_ConnectionAttemptFailed,
    MsgJapanese_DownloadFailed,
    MsgJapanese_InvalidDataReceived,
    MsgJapanese_Disconnected,
    MsgJapanese_ErrorOccurred,
    MsgJapanese_ErrorOccurred,
    MsgJapanese_PressTheAButton,
    MsgJapanese_PressTheAButton,
    MsgJapanese_NowSearching,
    MsgJapanese_SelectAGame,
    MsgJapanese_WaitingForData,
    MsgJapanese_Downloading,
    MsgJapanese_DownloadCompleted
};

static const u16 PosEnglish[] = {
    0x202,
    0x207,
    0x204,
    0x209,
    0x208,
    0xaa,
    0x1c6,
    0x65,
    0x208,
    0x205,
    0x207,
    0x209,
    0x206
};

static const u16 PosJapanese[] = {
    0x20a,
    0x209,
    0x20a,
    0x20a,
    0x20d,
    0xaa,
    0x1c6,
    0x66,
    0x207,
    0x208,
    0x20a,
    0x20a,
    0x20a
};

const struct MenuMsg MenuMsgInitial[] = {
    {MsgEnglish, PosEnglish},
    {MsgJapanese, PosJapanese}
};
