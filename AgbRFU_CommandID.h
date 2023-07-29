/********************************************************************/
/*          AgbRFU_CommandID.h                                      */
/*            AGB-RFU Library Header REQ Command (STWI) ID Number   */
/*                                                                  */
/*                                         last modified 11/20/2003 */
/*                                                                  */
/*          Copyright (C) 2000-2004 NINTENDO Co., Ltd.              */
/********************************************************************/
#ifndef __RFU_COMMAND_ID_H__
#define __RFU_COMMAND_ID_H__

#ifdef __cplusplus
extern "C" {
#endif


// REQ-COMMAND (STWI) ID CODE LIST

    // REQ Command ID returned by the REQ callback
#define ID_RESET_REQ                        0x0010
#define ID_LINK_STATUS_REQ                  0x0011
#define ID_SYSTEM_STATUS_REQ                0x0013
#define ID_GAME_CONFIG_REQ                  0x0016
#define ID_SYSTEM_CONFIG_REQ                0x0017
#define ID_SC_START_REQ                     0x0019
#define ID_SC_POLL_REQ                      0x001a
#define ID_SC_END_REQ                       0x001b
#define ID_SP_START_REQ                     0x001c
#define ID_SP_POLL_REQ                      0x001d
#define ID_SP_END_REQ                       0x001e
#define ID_CP_START_REQ                     0x001f
#define ID_CP_POLL_REQ                      0x0020
#define ID_CP_END_REQ                       0x0021
#define ID_DATA_TX_REQ                      0x0024
#define ID_DATA_RX_REQ                      0x0026
#define ID_MS_CHANGE_REQ                    0x0027	  // When returned by the REQ callback, this ID indicates that execution of rfu_REQ_changeMasterSlave has finished.
#define ID_DISCONNECT_REQ                   0x0030
#define ID_CPR_START_REQ                    0x0032
#define ID_CPR_POLL_REQ                     0x0033
#define ID_CPR_END_REQ                      0x0034
#define ID_RESUME_RETRANSMIT_AND_CHANGE_REQ	0x0037
#define ID_STOP_MODE_REQ                    0x003d
#define ID_CLOCK_SLAVE_MS_CHANGE_ERROR_BY_DMA_REQ	0x00ff    // When the AGB is the clock slave, the RFU generates an informational notice, and an automatically started DMA, such as HDMA, is generated at the instant the AGB is being returned as the clock master. This ID is notified by a REQ callback when the exchange of this information (REQ command) fails.


    // REQ Command ID returned by the MSC callback
#define ID_DISCONNECTED_AND_CHANGE_REQ      0x0029
#define ID_DATA_READY_AND_CHANGE_REQ        0x0028
#define ID_DRAC_REQ_WITH_ACK_FLAG           0x0128
//#define ID_MS_CHANGE_REQ                    0x0027  // This ID is returned by both the REQ callback and the MSC callback.
                                                      // When returned by the MSC callback, this is notification that after the AGB has been made into the clock slave, the MC_Timer expired and the RFU returned the AGB to be the clock master.

#ifdef __cplusplus
};
#endif

#endif  // __RFU_COMMAND_ID_H__
