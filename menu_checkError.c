#include <Agb.h>
extern u8 my_state;
extern u8 SearchMenuErrorMsg;

void menu_checkError(u16 State)
{
    if (!State) return;

    if (my_state == 10) {
        my_state = 0xc2;
    } else if (my_state == 0) {
        my_state = 0xc5;
    } else {
        my_state = 0xc1;
    }

    SearchMenuErrorMsg = 4;
}
