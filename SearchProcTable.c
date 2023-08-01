#include <Agb.h>

extern u16 rfu_REQ_reset(void);
extern u16 REQ_configSystem(void);
extern u16 REQ_configGameData(void);
extern u16 rfu_REQ_startSearchParent(void);
extern u16 rfu_REQ_pollSearchParent(void);
extern u16 rfu_REQ_endSearchParent(void);
extern u16 REQ_startConnectParent(void);
extern u16 REQ_pollConnectParent(void);
extern u16 rfu_REQ_endConnectParent(void);
extern u16 REQ_changeMasterSlave(void);
extern u16 my_softReset_and_checkID(void);
extern u16 STWI_send_StopModeREQ(void);
extern u16 STWI_send_LinkStatusREQ(void);
extern u16 STWI_send_SystemStatusREQ(void);

u16 (*const SearchProcTable[])(void) = {
    rfu_REQ_reset,
    REQ_configSystem,
    REQ_configGameData,
    rfu_REQ_startSearchParent,
    rfu_REQ_pollSearchParent,
    rfu_REQ_endSearchParent,
    REQ_startConnectParent,
    REQ_pollConnectParent,
    rfu_REQ_endConnectParent,
    REQ_changeMasterSlave,
    my_softReset_and_checkID,
    STWI_send_StopModeREQ,
    STWI_send_LinkStatusREQ,
    STWI_send_SystemStatusREQ
};
