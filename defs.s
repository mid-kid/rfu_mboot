.macro tdef name, addr
.global \name
.thumb_set \name, \addr
.endm

tdef FUN_03003078, 0x03003078
tdef FUN_03003e04, 0x03003e04
tdef Sio32IntrProc_Unk0e10, 0x03000e10
tdef Init_Unk2894, 0x03002894
tdef Init_Unk2d34, 0x03002d34
tdef SearchProc_RadioCfg, 0x03000efc
tdef SearchProc_SetGameInfo, 0x03000ed0
tdef RfuDiscoveryStart, 0x03002a60
tdef RfuGameInfoGet, 0x03002a70
tdef RfuDiscoveryResults, 0x03002a8c
tdef SearchProc_Connect, 0x03000f10
tdef SearchProc_ConnectCheck, 0x03000f28
tdef RfuConnectReady, 0x03002d08
tdef SearchProc_WaitData, 0x03000f5c
tdef SearchProc_Boot, 0x03000f84
tdef Init_Unk292c, 0x0300292c
