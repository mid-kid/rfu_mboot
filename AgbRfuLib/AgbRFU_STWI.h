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

extern void (*STWI_callback_ID)(void);
extern u8 STWI_buffer_recv[280];
extern u8 STWI_buffer_send[280];

extern u32 STWI_init(void);
extern u32 STWI_init_all(void);
extern u16 STWI_poll_CommandEnd(void);
extern u16 STWI_check_Command(u32 Cmd,u8 VarSize);
extern u32 STWI_intr_vblank(void);
extern u16 STWI_send_ResetREQ(void);
extern u16 STWI_send_LinkStatusREQ(void);
extern u16 STWI_send_SystemStatusREQ(void);
extern u16 STWI_send_GameConfigREQ(const u8 *GameData,const u8 *UserName);
extern u16 STWI_send_SystemConfigREQ(u16 param_1,u8 param_2,u8 param_3);
extern u16 STWI_send_SP_StartREQ(void);
extern u16 STWI_send_SP_PollingREQ(void);
extern u16 STWI_send_SP_EndREQ(void);
extern u16 STWI_send_CP_StartREQ(u16 BeaconID);
extern u16 STWI_send_CP_PollingREQ(void);
extern u16 STWI_send_CP_EndREQ(void);
extern u16 STWI_send_DataTxREQ(u8 *Srcp,u8 Size);
extern u16 STWI_send_DataRxREQ(void);
extern u16 STWI_send_MS_ChangeREQ(void);
extern u16 STWI_send_DisconnectREQ(u8 param_1);
extern u16 STWI_send_StopModeREQ(void);

#endif

