#include <Agb.h>

#include "AgbRFU_STWI_private.h"

STWI_command_status STWI_status;
u8 STWI_buffer_send[280];  // STWI_send_buf
u8 STWI_buffer_recv[280];  // STWI_recv_buf
