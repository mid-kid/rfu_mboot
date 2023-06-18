.macro tdef name, addr
.global \name
.thumb_set \name, \addr
.endm

tdef DemoMain, 0x030002a0
tdef FUN_03003078, 0x03003078
tdef MbootDLStart, 0x030031f4
tdef FUN_03003e04, 0x03003e04
tdef RfuMbootCfg, 0x03003184
tdef RfuStrcmp, 0x03000eb0
tdef SearchMenuCheckGames, 0x03000fa0
tdef Sio32IntrProcSet, 0x03002f58
tdef Sio32IntrProc_Unk0e10, 0x03000e10
tdef MbootInit, 0x03002fe0
