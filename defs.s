.macro tdef name, addr
.global \name
.thumb_set \name, \addr
.endm
.macro adef name, addr
.global \name
.set \name, \addr
.endm

tdef FUN_03003078, 0x03003078
tdef Init_Unk2d34, 0x03002d34

tdef FUN_03002d4c, 0x03002d4c
tdef FUN_03003044, 0x03003044
tdef FUN_03003b60, 0x03003b60
tdef FUN_0300397c, 0x0300397c
