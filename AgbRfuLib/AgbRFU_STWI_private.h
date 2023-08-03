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

typedef struct {
    u32 state;  // state
    u32 cmdHeader;  // REQ_header
    u8 cmdSize;  // REQ_length
    u8 field3_0x9;  // REQ_next
    u8 error;   // HS_error/error
    u8 modeMaster;  // MS_mode
    u8 unk_07;
    u8 unk_08;
    u8 unk_09;
    u8 timer;
    u8 unk_11;
    u8 unk_12;  // sending_flag
}STWI_command_status;

extern STWI_command_status STWI_status;

#endif
