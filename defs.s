.macro tdef name, addr
.global \name
.thumb_set \name, \addr
.endm

tdef DemoMain, 0x030002a0
tdef FUN_03003078, 0x03003078
tdef FUN_03003e04, 0x03003e04
tdef Sio32IntrProc_Unk0e10, 0x03000e10
tdef RfuCmdInit, 0x03001ee0
tdef RfuCmdSend, 0x03001f58
tdef RfuCmdRecv, 0x03002020
