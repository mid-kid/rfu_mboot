.macro ddef name, addr
.global \name
.set \name, \addr
.endm
.macro tdef name, addr
.global \name
.thumb_set \name, \addr
.endm

tdef DemoMain, 0x030002a0
tdef SearchMenuInit, 0x03000534
