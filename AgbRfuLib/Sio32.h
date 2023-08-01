#ifndef Sio32_h
#define Sio32_h

struct Sio32 {
    u8 slave;
    u8 state;
    u16 handshakeStep;
    u16 dataLo;
    u16 dataHi;
    u16 unused;
    u16 deviceID;
};

#endif
