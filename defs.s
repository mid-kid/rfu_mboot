.macro tdef name, addr
.global \name
.thumb_set \name, \addr
.endm
.macro adef name, addr
.global \name
.set \name, \addr
.endm

tdef FUN_03003078, 0x03003078
tdef FUN_03003e04, 0x03003e04
tdef RfuIntrDataTransfer, 0x03000e10
tdef Init_Unk2894, 0x03002894
tdef Init_Unk2d34, 0x03002d34
tdef Init_Unk292c, 0x0300292c
adef Sio32Intr, 0x030017a0
tdef FUN_03002d4c, 0x03002d4c
