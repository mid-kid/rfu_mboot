extern void (*Sio32IntrProc)(void);

void Sio32IntrProcSet(void (*Func)(void))
{
    Sio32IntrProc = Func;
}
