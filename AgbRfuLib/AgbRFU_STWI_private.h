#ifndef _AGBRFU_STWI_PRIVATE_H
#define _AGBRFU_STWI_PRIVATE_H

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
