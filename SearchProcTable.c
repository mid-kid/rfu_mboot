#include <Agb.h>

extern u16 rfu_REQ_reset(void);
extern u16 SearchProc_RadioCfg(void);
extern u16 SearchProc_GameInfoSet(void);
extern u16 RfuDiscoveryStart(void);
extern u16 RfuGameInfoGet(void);
extern u16 RfuDiscoveryResults(void);
extern u16 SearchProc_Connect(void);
extern u16 SearchProc_ConnectCheck(void);
extern u16 RfuConnectReady(void);
extern u16 SearchProc_WaitData(void);
extern u16 SearchProc_Boot(void);
extern u16 STWI_send_StopModeREQ(void);
extern u16 STWI_send_LinkStatusREQ(void);
extern u16 STWI_send_SystemStatusREQ(void);

u16 (*const SearchProcTable[])(void) = {
    rfu_REQ_reset,
    SearchProc_RadioCfg,
    SearchProc_GameInfoSet,
    RfuDiscoveryStart,
    RfuGameInfoGet,
    RfuDiscoveryResults,
    SearchProc_Connect,
    SearchProc_ConnectCheck,
    RfuConnectReady,
    SearchProc_WaitData,
    SearchProc_Boot,
    STWI_send_StopModeREQ,
    STWI_send_LinkStatusREQ,
    STWI_send_SystemStatusREQ
};
