#ifndef Rfu_h
#define Rfu_h

struct STWI_status {
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
};

#endif
