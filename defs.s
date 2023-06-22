.macro tdef name, addr
.global \name
.thumb_set \name, \addr
.endm

tdef FUN_03003078, 0x03003078
tdef FUN_03003e04, 0x03003e04
tdef Sio32IntrProc_Unk0e10, 0x03000e10
tdef Init_Unk2894, 0x03002894
tdef Init_Unk2d34, 0x03002d34
tdef Init_Unk292c, 0x0300292c
tdef RfuConnectCheck, 0x03002bc4
tdef RfuSetGameInfo, 0x030029a0
tdef RfuCmd_GetGameInfo_Parse, 0x03002aa8
tdef RfuHandshake, 0x030025fc
tdef RfuResetAdapter, 0x03002598
