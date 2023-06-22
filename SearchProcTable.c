#include <Agb.h>

extern u16 RfuStart(void);
extern u16 SearchProc_RadioCfg(void);
extern u16 SearchProc_SetGameInfo(void);
extern u16 RfuDiscoveryStart(void);
extern u16 RfuGameInfoGet(void);
extern u16 RfuDiscoveryResults(void);
extern u16 SearchProc_Connect(void);
extern u16 SearchProc_ConnectCheck(void);
extern u16 RfuConnectReady(void);
extern u16 SearchProc_WaitData(void);
extern u16 SearchProc_Boot(void);
extern u16 RfuCmd_End(void);
extern u16 RfuCmd_Status(void);
extern u16 RfuCmd_0x13(void);

u16 (*const SearchProcTable[])(void) = {
    RfuStart,
    SearchProc_RadioCfg,
    SearchProc_SetGameInfo,
    RfuDiscoveryStart,
    RfuGameInfoGet,
    RfuDiscoveryResults,
    SearchProc_Connect,
    SearchProc_ConnectCheck,
    RfuConnectReady,
    SearchProc_WaitData,
    SearchProc_Boot,
    RfuCmd_End,
    RfuCmd_Status,
    RfuCmd_0x13
};
