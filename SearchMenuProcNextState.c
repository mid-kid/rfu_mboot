#include <Agb.h>
extern u8 MenuState;
extern u8 SearchMenuErrorMsg;

void SearchMenuProcNextState(u16 State)
{
    if (!State) return;

    if (MenuState == 10) {
        MenuState = 0xc2;
    } else if (MenuState == 0) {
        MenuState = 0xc5;
    } else {
        MenuState = 0xc1;
    }

    SearchMenuErrorMsg = 4;
}
