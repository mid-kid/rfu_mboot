.macro tdef name, addr
.global \name
.thumb_set \name, \addr
.endm

tdef FUN_03003078, 0x03003078
tdef FUN_03003e04, 0x03003e04
tdef Sio32IntrProc_Unk0e10, 0x03000e10
tdef Init_Unk2894, 0x03002894
tdef Init_Unk2d34, 0x03002d34
tdef RfuDiscoveryStart, 0x03002a60
tdef RfuGameInfoGet, 0x03002a70
tdef RfuDiscoveryResults, 0x03002a8c
tdef RfuConnectReady, 0x03002d08
tdef Init_Unk292c, 0x0300292c
tdef RfuRadioCfg, 0x03002a40
tdef RfuBoot, 0x030025ec
tdef RfuConnect, 0x03002b70
tdef RfuConnectCheck, 0x03002bc4
tdef RfuSetGameInfo, 0x030029a0
