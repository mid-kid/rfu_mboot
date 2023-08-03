#ifndef _RFU_STWI_PRIVATE_H
#define _RFU_STWI_PRIVATE_H

#define STWI_SIO_BUF_SIZE 0x960
#define SIO_CNT_RESET 0x5003
#define SIO_SEND_SPEED 0x5083
#define SIO_RECEIVE_SPEED 0x5082
#define STWI_STATUS_ERROR 0
#define STWI_STATUS_MSMODE 1
#define STWI_STATUS_STATE 2
#define STWI_STATUS_CMD_ID 3

// ---- pubnames:
// timer_state
// NOT_USED
// WAIT50MS
// WAIT80MS
// WAIT100MS
// WAIT130MS
// comm_state
// M_TxREQ
// M_RxACK_HEADER
// M_RxACK_END
// M_COMPLETE
// S_RxREQ_HEADER
// S_RxREQ_CONTENTS
// S_TxACK
// S_TxACK_END
// S_COMPLETE
// ID_CHECK

typedef struct {
	u32 state;
	u32 REQ_header;
	u8 REQ_length;
	u8 REQ_next;
	u8 error;
	u8 MS_mode;
	u8 sending_flag;
	u8 unk_08;
	u8 unk_09;
	u8 unk_10;
	u8 unk_11;
	u8 HS_error;
}STWI_command_status;

extern STWI_command_status STWI_status;

extern u8 STWI_callback_ID;
extern void (*STWI_callback_MSC)(void);

#endif

