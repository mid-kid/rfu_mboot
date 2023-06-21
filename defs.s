.macro tdef name, addr
.global \name
.thumb_set \name, \addr
.endm

tdef FUN_03003078, 0x03003078
tdef FUN_03003e04, 0x03003e04
tdef Sio32IntrProc_Unk0e10, 0x03000e10
tdef Init_Unk2894, 0x03002894
tdef Init_Unk2d34, 0x03002d34
