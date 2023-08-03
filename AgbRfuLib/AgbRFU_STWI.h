#ifndef _AGBRFU_STWI_H
#define _AGBRFU_STWI_H

#define STWI_BUF_SIZE 0x29e
#define STWI_BUF_SIZE_ROM 0x46
#define STWI_RECV_BUF_SIZE 0x74
#define STWI_ERROR_NO_ERROR 0
#define STWI_ERROR_CLOCK_DRIFT 1
#define STWI_ERROR_COMMAND_SENDING 2
#define STWI_CMDERR_ACK_REJECTION 3
#define STWI_CMDERR_CLOCK_SLAVE 4
#define STWI_CMDERR_IME_DISABLE 6

u32 STWI_init(void);
u32 STWI_init_all(void);
u16 STWI_poll_CommandEnd(void);
u16 STWI_check_Command(u32 Cmd,u8 VarSize);
u32 STWI_intr_vblank(void);
u16 STWI_send_ResetREQ(void);
u16 STWI_send_LinkStatusREQ(void);
u16 STWI_send_SystemStatusREQ(void);
u16 STWI_send_GameConfigREQ(const u8 *GameData,const u8 *UserName);
u16 STWI_send_SystemConfigREQ(u16 param_1,u8 param_2,u8 param_3);
u16 STWI_send_SP_StartREQ(void);
u16 STWI_send_SP_PollingREQ(void);
u16 STWI_send_SP_EndREQ(void);
u16 STWI_send_CP_StartREQ(u16 BeaconID);
u16 STWI_send_CP_PollingREQ(void);
u16 STWI_send_CP_EndREQ(void);
u16 STWI_send_DataTxREQ(u8 *Srcp,u8 Size);
u16 STWI_send_DataRxREQ(void);
u16 STWI_send_MS_ChangeREQ(void);
u16 STWI_send_DisconnectREQ(u8 param_1);
u16 STWI_send_StopModeREQ(void);

#endif
