#if 1
__asm__("
.section .text
.global DemoMain
.type DemoMain, function
.thumb_func
DemoMain:
.2byte 0xb530,0xb081,0x4d2f,0x2400,0x802c,0x492f,0x2014,0x8008,0x201d,0xf003,0xfe11,0x2000,0x9000,0x4a2c,0x4668,0x6010,0x492b,0x6051,0x482b,0x1a40,0x0880,0x2185,0x0609,0x4308,0x6090,0x6890,0x4928,0x4829,0x6008,0x4829,0x4929,0xf003,0xfdf9,0x4829,0x21a0,0x04c9,0xf003,0xfdf4,0x4827,0x4928,0xf003,0xfdf0,0xf001,0xf9bc,0xf001,0xf970,0x4925,0x2004,0x8008,0x3102,0x4a24,0x1c10,0x8008,0x3102,0x4a23,0x1c10,0x8008,0x3102,0x4a22,0x1c10,0x8008,0x390e,0x2290,0x0112,0x1c10,0x8008,0xf002,0xfab6,0x491e,0x2081,0x8008,0x491e,0x2008,0x8008,0x2001,0x8028,0x2008,0xf002,0xfcfb,0x481b,0x7004,0x481b,0x7004,0xf000,0xf839,0x491a,0x481a,0x6008,0x1c0c,0xf003,0xfdcf,0xf001,0xf84b,0x6820,0xf003,0xfdce,0xe7f7,0x0000,0x0208,0x0400,0x0204,0x0400,0x00d4,0x0400,0x5684,0x0300,0x6310,0x0300,0x7ffc,0x0300,0x01fc,0x0300,0x460c,0x0300,0x4000,0x0600,0x43c8,0x0300,0x4420,0x0300,0x1800,0x0600,0x0008,0x0400,0x0105,0x0000,0x0206,0x0000,0x0307,0x0000,0x0200,0x0400,0x0004,0x0400,0x5764,0x0300,0x5758,0x0300,0x5c90,0x0300,0x04c5,0x0300
.size DemoMain, .-DemoMain
");
#else
#endif