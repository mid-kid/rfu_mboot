#include <Agb.h>
#include <AgbRFU_LL.h>
#include "data.h"
#include "sound.h"

extern void IntrSIO32(void);

static void VBlankIntr(void);

void(*const IntrTable[])(void)={
	IntrSIO32,
	VBlankIntr
};

static void VBlankIntr(void)
{
	DmaArrayCopy(3,Bg0Bak,VRAM,32);
	
	snd_syncVBlank();
	rfu_syncVBlank();
	
	*(vu16 *)INTR_CHECK_BUF=1;
}

