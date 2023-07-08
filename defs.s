.macro tdef name, addr
.global \name
.thumb_set \name, \addr
.endm
.macro adef name, addr
.global \name
.set \name, \addr
.endm

tdef Init_Unk2d34, 0x03002d34
tdef RfuDataRecvWrite2, 0x03003d38
tdef RfuDataRecvWrite1, 0x03003ce0
