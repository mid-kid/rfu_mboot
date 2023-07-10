extern void (*STWI_intr)(void);

void Sio32IntrProcSet(void (*Func)(void))
{
    STWI_intr = Func;
}
