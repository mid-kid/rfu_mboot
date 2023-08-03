/********************************************************************/
/*          AgbRFU_LL.h                                             */
/*            AGB RFU-API Library Header     ver. 1.0.28            */
/*                                                                  */
/*                                         last modified 03.18.2005 */
/*                                                                  */
/*          Copyright (C) 2000-2004 NINTENDO Co.,Ltd.               */
/********************************************************************/
#ifndef __RFU_LINK_LAYER_H__
#define __RFU_LINK_LAYER_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <AgbTypes.h>
#include "AgbRFU_CommandID.h"


// --------------------------------------------------------------------------
//
// Restrictions When Using RFU
//
// --------------------------------------------------------------------------
/*
  The following restrictions apply when using RFU.

  <Restrictions for Direct Sound>
    (a) The timer for Sound DMA uses only 0
    (b) The prescaler for the timer count for the Sound DMA uses 59.5 ns.
    (c) The sound sampling rate is Max36.314KHz.

  <Restrictions for H-DMA or DMA started with V-Blank>
    These types of DMA can be operated on a maximum CPU cycle of 42 cycles.
    Calculate the number of the transferable DMA count based on this 42 cycles and the access cycles of the destination and source.
    For example, if both the CPU internal RAM --> VRAM have a one cycle access, then a 21 count DMA can occur.

  
  If RFU is used outside of these restrictions, problems, such as the loss of data caused by the failure of the AGB, as a clock slave, 
  to notify that data has been received from the RFU, will occur.
  When this problem occurs, the REQ callback will send a REQ_commandID=ID_CLOCK_SLAVE_MS_CHANGE_ERROR_BY_DMA_REQ notification.
   (When using Link Manager, the LMAN call back will send a LMAN_msg=LMAN_MSG_CLOCK_SLAVE_MS_CHANGE_ERROR_BY_DMA notification.)
  
*/


// --------------------------------------------------------------------------
//
// Data Definitions
//
// --------------------------------------------------------------------------

#define RFU_ID                          0x00008001         // ID number of AGB-RFU device: ID returned by execution of rfu_REQBN_softReset_and_checkID.

#define RFU_MBOOT_DOWNLOADER_SERIAL_NO  0x0000             // The game serial number of the multi-boot downloader (programs that boot without a Game Pak)

#define RFU_API_BUFF_SIZE_RAM			0x0e64             // Necessary size for buffer specified by rfu_initializeAPI (fast communication version that operates the library SIO interrupt routines in RAM)
#define RFU_API_BUFF_SIZE_ROM			0x0504             // Necessary size for buffer specified by rfu_initializeAPI (fast communication version that operates the library SIO interrupt routines in ROM)

#define RFU_CHILD_MAX                   4                  // Maximum number of slaves that can be connected to one parent device

#define RFU_GAME_NAME_LENGTH            13                 // Possible length of game name set by rfu_REQB_configGameData
#define RFU_USER_NAME_LENGTH            8                  // Possible length of user name set by rfu_REQB_configGameData

#define RFU_H_DMA_MAX_CPU_CYCLE         42                 // Maximum number of CPU cycles for H-DMA or DMA that starts with V-Blank that can operate when using RFU

// Value to determine the level of the icon that displays the link strength of the wireless status (value of rfuLinkStatus->strength[0-3])
#define RFU_LINK_ICON_LEVEL4_MAX        255                // 90% -100%  (If the value is betwen 229 and 255, the icon level is 4)
#define RFU_LINK_ICON_LEVEL4_MIN        229
#define RFU_LINK_ICON_LEVEL3_MAX        228                // 50% - 89%  (If the value is betwen 127 and 228, the icon level is 3)
#define RFU_LINK_ICON_LEVEL3_MIN        127
#define RFU_LINK_ICON_LEVEL2_MAX        126                // 10% - 49%  (If the value is betwen 25 and 126, the icon level is 2)
#define RFU_LINK_ICON_LEVEL2_MIN         25
#define RFU_LINK_ICON_LEVEL1_MAX         24                //  0% -  9%  (If the value is betwen 0 and 24, the icon level is 1)
#define RFU_LINK_ICON_LEVEL1_MIN          0


// *******************************************************
// Definition Data for Arguments Used in Library Functions
// *******************************************************
// Specified by u16 mboot_flag in rfu_REQ_configGameData
#define RFU_MBOOT_FLAG                  0x01               // Enabling this flag, allows connection to a multi-boot program.

// AvailSlot (number of slots available for connection) used in u16 availSlot_flag in rfu_REQ_configSystem
#define AVAIL_SLOT4                     0x0000
#define AVAIL_SLOT3                     0x0001
#define AVAIL_SLOT2                     0x0002
#define AVAIL_SLOT1                     0x0003

// u8 connType_flag specified by rfu_setRecvBuffer
#define TYPE_UNI                        0x10               // UNI-type
#define TYPE_NI                         0x20               // NI-type

// u8 connType_flag specified by rfu_clearSlot
#define TYPE_UNI_SEND                   0x01               // UNI-type send
#define TYPE_UNI_RECV                   0x02               // UNI-type receive
#define	TYPE_NI_SEND                    0x04               // NI-type send
#define	TYPE_NI_RECV                    0x08               // NI-type receive

// *******************************************************
// Definition Data Returned by Return Values for Library Functions
// *******************************************************

// Value of u8 *status for rfu_REQ_pollConnectParent (Connection Trial Status)
#define CP_STATUS_DONE                  0x00               // Connection successful
#define CP_STATUS_IN_PROCESS            0x01               // Connecting
#define CP_STATUS_SLOT_CLOSED           0x02               // Parent device is not in connection mode with child device
#define CP_STATUS_DISCONNECTED          0x03               // Disconnected by parent device while connecting
#define CP_STATUS_UNKNOWN               0xff               // Cannot read status due to REQ-API execution error

// Value of u8 *status argument for rfu_REQ_pollRecoveryConnect (Link Restore Status)
#define RC_STATUS_DONE                  0x00               // Connection restore successful
#define RC_STATUS_FAILED                0x01               // Connection restore failure (meaningless to try anymore)
#define RC_STATUS_SEARCHING_PARENT      0x02               // Searching for parent device
#define RC_STATUS_UNKNOWN               0xff               // Cannot read status due to REQ-API execution error

// Value of u8 *linkLossReason argument for rfu_REQBN_watchLink (Reason for Link Loss)
#define REASON_DISCONNECTED             0x00               // State of complete disconnection with rfuLL_disconnect and no possibility of restoring the link (can detect only child device)
#define REASON_LINK_LOSS                0x01               // State of link loss that allows for the possibility for the link to be restored


// *******************************************************
// Definition Data Returned with Return Values for Library Functions
// *******************************************************

// Value returned by rfu_getMasterSlave
#define AGB_CLK_SLAVE                   0                  // AGB clock slave
#define AGB_CLK_MASTER                  1                  // AGB clock master

// *******************************************************
// Error Code Group for Library Functions
// *******************************************************

// Error codes returned by u16 REQ_result with a REQ callback when a REQ-API function is executed
#define ERR_REQ_CMD                     0x0000
#define ERR_REQ_CMD_CLOCK_DRIFT        (ERR_REQ_CMD | 0x0001) // Clock drift occurs when a REQ command is sent
#define ERR_REQ_CMD_SENDING            (ERR_REQ_CMD | 0x0002) // The next command cannot be issued because a REQ command is being sent
#define ERR_REQ_CMD_ACK_REJECTION      (ERR_REQ_CMD | 0x0003) // The REQ command was refused when issued
#define ERR_REQ_CMD_CLOCK_SLAVE        (ERR_REQ_CMD | 0x0004) // Issuing the REQ command was attempted but failed because the AGB is the clock slave
#define ERR_REQ_CMD_IME_DISABLE        (ERR_REQ_CMD | 0x0006) // Issuing the REQ command was attempted but failed because the IME register is 0

#define ERR_PID_NOT_FOUND               0x0100                // The specified PID does not exist in the rfuLinkStatus->partner[0-3].id list

// Error code returned by the return value of rfu_initializeAPI
#define ERR_RFU_API_BUFF_SIZE           0x0001
#define ERR_RFU_API_BUFF_ADR			0x0002

// Error codes returned by return values for rfu_REQBN_softReset_and_checkID
#define ERR_ID_CHECK_IME_DISABLE        0xffffffff            // ID check could not be executed because REG_IME=0 when executing rfu_REQBN_softReset_and_checkID

// Error codes returned by rfu_getConnectParentStatus andrfu_CHILD_getConnectRecoveryStatus
#define ERR_REQ_CMD_ID                 (ERR_REQ_CMD | 0x0010) // A required REQ command was not executed within this function execution

// Error codes returned by rfu_NI_setSendData, rfu_UNI_setSendData, and rfu_NI_CHILD_setSendGameName
#define ERR_MODE                        0x0300
#define ERR_MODE_NOT_CONNECTED         (ERR_MODE | 0x0001)    // RFU is not in connection mode (parent or child)

#define ERR_SLOT                        0x0400
#define ERR_SLOT_NO                    (ERR_SLOT | 0x0000)    // The specified slot number is not right
#define ERR_SLOT_NOT_CONNECTED         (ERR_SLOT | 0x0001)    // The specified slot is not connected
#define ERR_SLOT_BUSY                  (ERR_SLOT | 0x0002)    // The specified slot is already communicating
#define ERR_SLOT_NOT_SENDING           (ERR_SLOT | 0x0003)    // The specified slot is not currently sending (Note: This is returned by rfu_UNI_re_setSendData)
#define ERR_SLOT_TARGET                (ERR_SLOT | 0x0004)    // The specified target slot is not right

#define ERR_SUBFRAME_SIZE               0x0500                // The specified subframe size is either larger than the remaining communication frame size or smaller than the link layer subframe header size

// Error code returned by rfu_clearSlot, rfu_setRecvBuffer, and rfu_changeSendTarget
#define ERR_COMM_TYPE                   0x0600                // The specified communication format is not right

// Error code returned by rfu_REQ_recvData (when receiving NI-type or UNI-type data)
#define ERR_DATA_RECV                   0x0700                // Error notification is issued in the following format.
                                                              // "ERR_DATA_RECV | (slot number issuing error with 0x0010<<UNI) | (slot number issuing error with 0x0001<<NI)"
                                                              // The actual error code is stored in rfuSlotStatus_NI or _UNI[x]->recv.errorCode.

// Error code during NI-type communication (code entered into rfuSlotStatus_NI[x]->recv.errorCode)
#define ERR_RECV_BUFF_OVER              (ERR_DATA_RECV | 0x0001)  // The receive data size is larger than the receive buffer (Note: This error is used for both NI and UNI)
#define ERR_RECV_REPLY_SUBFRAME_SIZE    (ERR_DATA_RECV | 0x0002)  // All frames are already in use by other communications, and a subframe for reception response could not be secured

// Error code during UNI-type communication (code entered into rfuSlotStatus_UNI[x]->recv.errorCode)
#define ERR_RECV_DATA_OVERWRITED        (ERR_DATA_RECV | 0x0008)  // Received new data when rfuSlotStatus_UNI[x]->recv.newData_flag=1


// *******************************************************
// Definition Data Used by Global Variables in the Library
// *******************************************************

// rfuLinkStatus-------------------------------------------
// Connection status (refer to rfuLinkStatus->parent_child)
#define MODE_NEUTRAL                    0xff               // Initialization
#define MODE_CHILD                      0x00               // Child
#define MODE_PARENT                     0x01               // Parent


// rfuSlotStatus_NI,_UNI-----------------------------------
// Definition data for NI-(Numbered Information) and UNI-(Un-Numbered Information) type communication

// Definition data common to NI- and UNI-type communication
#define LLF_P_SIZE                      87                 // Maximum link layer communication frame size for the Parent (Maximum number of bytes per transmission)
#define LLF_C_SIZE                      16                 //      "                                "     for the Child
#define LLSF_P_HEADER_SIZE              3                  // Header size used by one link layer subframe (NI or UNI frame) for the Parent
#define LLSF_C_HEADER_SIZE              2                  //      "                                "                       for the Child

// Link Layer Communication Code
#define LCOM_NULL                       0x0000
#define LCOM_NI_START                   0x0001
#define LCOM_NI                         0x0002
#define LCOM_NI_END                     0x0003
#define LCOM_UNI                        0x0004

// Flag for Slot Status Data
#define SLOT_BUSY_FLAG                  0x8000             // Slot in use
#define SLOT_SEND_FLAG                  0x0020             // Send
#define SLOT_RECV_FLAG                  0x0040             // Receive

// NI- and UNI-type shared slot stand-by data (same state as rfuSlotStatus_NI[x]->send or ->recv.state or rfuSlotStatus_UNI)
#define SLOT_STATE_READY                0x0000             // Slot initialization

// Status data for NI-type communication (rfuSlotStatus_NI[x]->send or ->recv.state)
#define SLOT_STATE_SEND_START           (SLOT_BUSY_FLAG | SLOT_SEND_FLAG | LCOM_NI_START)   // Start transmission
#define SLOT_STATE_SENDING              (SLOT_BUSY_FLAG | SLOT_SEND_FLAG | LCOM_NI)         // Transmitting
#define SLOT_STATE_SEND_LAST            (SLOT_BUSY_FLAG | SLOT_SEND_FLAG | LCOM_NI_END)     // Verify transmission end
#define SLOT_STATE_SEND_NULL            (SLOT_BUSY_FLAG | SLOT_SEND_FLAG | LCOM_NULL)       // Send NULL subframe to notify end of transmission
#define SLOT_STATE_SEND_SUCCESS         (                 SLOT_SEND_FLAG | 0x006)           // Transmission successful
#define SLOT_STATE_SEND_FAILED          (                 SLOT_SEND_FLAG | 0x007)           // Transmission failed
                                                                                            // Distinction of slot where transmission succeeded/failed compares rfuSlotStatus_NI[x]->send.bmSlot and same bmSlot_org
#define SLOT_STATE_RECV_START           (SLOT_BUSY_FLAG | SLOT_RECV_FLAG | LCOM_NI_START)   // Start reception
#define SLOT_STATE_RECEIVING            (SLOT_BUSY_FLAG | SLOT_RECV_FLAG | LCOM_NI)         // Receiving
#define SLOT_STATE_RECV_LAST            (SLOT_BUSY_FLAG | SLOT_RECV_FLAG | LCOM_NI_END)     // Verify reception end
#define SLOT_STATE_RECV_SUCCESS         (                 SLOT_RECV_FLAG | 0x006)           // Reception successful
#define SLOT_STATE_RECV_FAILED          (                 SLOT_RECV_FLAG | 0x007)           // Reception failed
#define SLOT_STATE_RECV_SUCCESS_AND_SENDSIDE_UNKNOWN     (SLOT_RECV_FLAG | 0x008)           // The data was received successful, but because the end of the communication was incomplete, whether the transmission side status is successful or failed is unknown
#define SLOT_STATE_RECV_IGNORE          (                 SLOT_RECV_FLAG | 0x009)           // Ignore reception (refer to rfuSlotStatus_NI[x]->recv.errorCode for reason for ignoring)

// Status data for UNI-type communication (refer to rfuSlotStatus_UNI[x]->send.state)
#define SLOT_STATE_SEND_UNI             (SLOT_BUSY_FLAG | SLOT_SEND_FLAG | LCOM_UNI)        // Transmitting


// --------------------------------------------------------------------------
//
// Data Type Definition
//
// --------------------------------------------------------------------------

    // Game Identification Information Data Structure
typedef struct rfuTgtDataTag {                             // During SearchParent                                    All other times (Parent or Child)
    u16 id;                                                // ID of parent candidate                                 ID of connection partner
    u8  slot;                                              // Slot number where parent candidate can enter           Connection slot number
    u8  mboot_flag;                                        // Flag to indicate whether or not the parent candidate   Flag to indicate whether or not the connection partner
                                                           //   is multiboot program for download                      is multiboot program for download
    u16 serialNo;                                          // Game serial number of parent candidate                 Game serial number of connection partner
    u8  gname[RFU_GAME_NAME_LENGTH+2];                     // Game name of parent candidate                          Game name of connection partner
    u8  uname[RFU_USER_NAME_LENGTH+1];                     // User name for parent candidate                         User name for connection partner
	u8  padding[2];
}rfuTgtData;

// Link Status Data Structure
typedef struct RFU_linkStatus_Tag {
    u8  parent_child;                                      // Connection status (0x01:MODE_PARENT  0x00:MODE_CHILD  0xff:MODE_NEUTRAL)
    u8  connectCount;                                      // Number of connections
    u8  connectSlot_flag;                                  // Flag to indicate in bits the connection slot (0x01<<Connection slot number)
    u8  linkLossSlot_flag;                                 // Flag to indicate in bits the slot where the link loss was generated (")
    u8  sendSlot_NI_flag;                                  // Flag to indicate in bits the slot where NI-type send is occurring (")
    u8  recvSlot_NI_flag;                                  // Flag to indicate in bits the slot where NI-type receive is occurring (")
    u8  sendSlot_UNI_flag;                                 // Flag to indicate in bits the slot where UNI-type send is occurring (")
    u8  getName_flag;                                      // Parent: Flag to indicate whether or not game identification information was obtained from the child connected to this slot (")
                                                           // Child: Flag to indicate whether or not game identification information was sent to the parent connected to this slot (")
    u8  findParentCount;                                   // Number of parent candidates discovered with rfu_REQ_xxxSearchParent, valid only with a child device
    u8  watchInterval;                                     // Link monitor execution interval (16.7 ms unit AGB picture frame)
    u8  strength[RFU_CHILD_MAX];                           // Link strength value (0x00 - 0xff; 0x00 is link loss. Updates with rfu_REQBN_watchLink)
    vu8 llf_ready_flag;                                    // Indicates whether the link layer frame to send next has be constructed (Flag is on when calling rfu_constructLLFrame, flag is cleared when sending by rfu_REQ_sendData.)
    u8  remainLLFrameSize_P;                               // Remaining size of the link layer communication frame for the parent device
    u8  remainLLFrameSize_C[RFU_CHILD_MAX];                // Remaining size of the link layer communication frame for each slot for child device
    
    rfuTgtData partner[RFU_CHILD_MAX];                     // Parent, Child: When there is a connection, the matrix element corresponding to the number of the connected slot
                                                           //         stores the game identification information for the connection partner.
                                                           // Child: Stores the game identification information for the parent candidate discovered when executing rfu_REQ_xxxxSearchParent.
    rfuTgtData my;                                         // The device's own game identification information (unrelated to the element slot value)
}RFU_LINK_STATUS;


#define WINDOW_COUNT    4

    // Data Structure for NI-Type Data Transmission/Reception
typedef struct NI_COMM_Tag {
    // Parameters indicating current communication state
    u16 state;                                             // Communication state of slot
    
    u16 failCounter;                                       // Count of failed transmissions/receptions (Count is increased when transmission/reception of data does not succeed within 1PF=16.7 ms)
    u8  *nowp[WINDOW_COUNT];                               // Address of current send/receive (The data is divided into WINDOW_COUNT blocks and sent in payloadSize units.)
    u32 remainSize;                                        // Size of remaining communication data
    u16 errorCode;                                         // Error code
    u8  bmSlot;                                            // Expresses the current communication slot in bits
                                                           // (When sending from the Maser, because multiple slaves can be specified with bmSlot, communications are terminated based on the failCounter for each child device)
    
    // Parameters used inside the Library
    u8  recv_ack_flag[WINDOW_COUNT];
    u8  ack;
    u8  phase;
    u8  n[WINDOW_COUNT];
    
    // Parameters indicating the current communication content
    const u8 *src;                                         // Start address transmission data (valid only for transmitting device)
    u8  bmSlot_org;                                        // Expresses the communication source slot at the beginning of the transmission in bits (valid only for transmitting device)
    u8  dataType;                                          // Transmission/reception data type (0: User data; 1: Game identification information)
    u16 payloadSize;                                       // Payload size for one transmission
    u32 dataSize;                                          // Size of transmission/reception data
                                                           // Calculated by subtracting link layer header size from subFrameSize specified by the rfu_NI_setSendData function    
}NI_COMM;

    // Data Structure for Transmitting UNI-Type Data
typedef struct UNI_SEND_Tag {
    u16 state;                                             // Slot communication state (SLOT_STATE_READY, SLOT_STATE_SEND_UNI)
    u8  dataReady_flag;                                    // Flag indicating transmission data is prepared (0: data sent; 1: transmission data ready)
                                                           //   Set with rfu_UNI_re_setSendData or rfu_UNI_readySendData, cleared when data is sent with rfu_REQ_sendData.
    u8  bmSlot;                                            // Expresses transmission destination slot as bits
    u16 payloadSize;                                       // Payload size of 1 transmission. "size" specified by rfu_UNI_setSendData or rfu_UNI_re_setSendData is used.
    const u8  *src;                                        // Beginning address of transmission data
}UNI_SEND;

    // Data Structure for Receiving UNI Data
typedef struct UNI_RECV_Tag {
    u16 state;                                             // Slot communication state (SLOT_STATE_READY, SLOT_STATE_RECEIVING, SLOT_STATE_RECV_IGNORE)
    u16 errorCode;                                         // Error code during reception
    u16 dataSize;                                          // Size of receive data
    u8  newData_flag;                                      // Flag indicating whether receive data has newly arrived (0: no newly arrived data; 1: newly arrived data)
                                                           //   The flag is cleared with rfu_UNI_clearRecvNewDataFlag; it is set when data is received with rfu_REQ_recvData.
    u8  dataBlock_flag;                                    // Flag for unreferenced receive data overwrite block (default is 0 Note: not used)
}UNI_RECV;

    // Slot Status Data Structure for NI-Type Communication
typedef struct RFU_SlotStatus_NI_Tag {
    NI_COMM  send;                                         // Transmission Status
    NI_COMM  recv;                                         // Reception Status
    u8       *recvBuff;                                    // Reception Buffer Address
    u32      recvBuffSize;                                 // Reception Buffer Size
}RFU_SLOT_STATUS_NI;

    // Slot Status Data Structure for UNI-Type Communication
typedef struct RFU_SlotStatus_UNI_Tag {
    UNI_SEND send;                                         // Transmission Status
    UNI_RECV recv;                                         // Reception Status
    u8       *recvBuff;                                    // Reception Buffer Address
    u32      recvBuffSize;                                 // Reception Buffer Size
}RFU_SLOT_STATUS_UNI;


// --------------------------------------------------------------------------
//
// Global Variables for the Library
//
// --------------------------------------------------------------------------

extern RFU_LINK_STATUS     rfuLinkStatus;                      // Link Status Data
extern RFU_SLOT_STATUS_NI  rfuSlotStatus_NI[RFU_CHILD_MAX];    // Slot Status Data for NI-type Communication
extern RFU_SLOT_STATUS_UNI rfuSlotStatus_UNI[RFU_CHILD_MAX];   // Slot Status Data for UNI-type Communication


// --------------------------------------------------------------------------
//
// Library Functions
//
// --------------------------------------------------------------------------

/*
  Arguments with "bm_..." specify slots of the form (0x01<<slot number) that are the object of a function operation.
*/

// API Initialization and Initial Settings
    // API Initialization
extern u16  rfu_initializeAPI(u32 *API_buffer,u16 buffByteSize, void (**sioIntrTablep)( void ),u8 sio_use_ram_flag);
    // Set Timer Interrupt
extern void rfu_setTimerInterrupt(u8 timerNo, void (**timerIntrTablep)( void ));
    // Resident Function called from within a V-Blank Interrupt
extern void rfu_syncVBlank(void);
    // Specify REQ Callback function
extern void rfu_setREQCallback(void (*callbackFuncp)(u16 REQ_command_ID,u16 REQ_result));
    // REQ-API Execution Completion Wait
extern u16  rfu_waitREQComplete(void);



// RFU Initialization and Initial Settings
    // RFU Startup and ID Check (Forced RFU reset occurs simultaneously)
extern u32  rfu_REQBN_softReset_and_checkID(void);
    // RFU Reset
extern u16  rfu_REQ_reset(void);
    // Set RFU to Stop Mode (Power Down)
extern void rfu_REQ_stopMode(void);
    // RFU Hardware Settings
extern u16  rfu_REQ_configSystem(u16 availSlot_flag,u8 MaxMframe,u8 MC_Timer);
    // Game Identification Information Configuration
extern u16  rfu_REQ_configGameData(u8 mboot_flag,u16 serialNo,const u8 *GName,const u8 *UName);



// RFU Connection Process
    // Operate as parent device; search for and connect to child device
extern void rfu_REQ_startSearchChild(void);
extern void rfu_REQ_pollSearchChild(void);
extern void rfu_REQ_endSearchChild(void);
    // Operate as child device; search for parent device
extern u16  rfu_REQ_startSearchParent(void);
extern u16  rfu_REQ_pollSearchParent(void);
extern u16  rfu_REQ_endSearchParent(void);
    // Operate as child device; connect to specified parent device
extern u16  rfu_REQ_startConnectParent(u16 pid);
extern u16  rfu_REQ_pollConnectParent(u8 *status,u8 *connect_slotNo);
extern u16  rfu_REQ_endConnectParent(void);
    // Restore link from child device
extern void rfu_REQ_CHILD_startConnectRecovery(u8 bm_recoverySlot);
extern void rfu_REQ_CHILD_pollConnectRecovery(void);
extern void rfu_REQ_CHILD_endConnectRecovery(void);
extern u16  rfu_CHILD_getConnectRecoveryStatus(u8 *status);



// RFU Link Management
    // Link Monitoring
extern u16  rfu_REQBN_watchLink(u8 *bm_linkLossSlot,u8 *linkLossReason,u8 *parent_bm_linkRecoverySlot);
    // Link Disconnect
extern void rfu_REQ_disconnect(u8 bm_disconnectSlot);



// Relation of clock between AGB and RFU
    // Switch to AGB clock slave
extern u16  rfu_REQ_changeMasterSlave(void);
    // Acquire either the master or slave clock from the current AGB-RFU
extern u8   rfu_getMasterSlave(void);



// Communication Configuration
    // MSC Callback Configuration
extern void rfu_setMSCCallback(void (*callbackFuncp)(u16 REQ_command_ID));
    // Shared by NI- and UNI-type communications
        // Clear Communication Status
extern u16  rfu_clearAllSlot(void);
extern u16  rfu_clearSlot(u8 connType_flag,u8 slotStatus_Index);
        // Set Receive Buffer
extern u16  rfu_setRecvBuffer(u8 connType,u8 slotNo,void *buffer,u32 buffSize);



// Receive/Send Data
    // UNI-type communication
        // Set transmission data
extern u16  rfu_UNI_setSendData(u8 bm_sendSlot,const void *src,u8 size);
        // Enable transmission data
extern void rfu_UNI_readySendData(u8 slotStatus_Index);
        // Change address or size of transmission data and enable transmission data
extern u16  rfu_UNI_changeAndReadySendData(u8 slotStatus_Index,const void *src,u8 size);
        // Used only by parent device. At the beginning of a MSC Callback that received notification that the data transmission completed, an ACK flag is obtained.
extern u16  rfu_UNI_PARENT_getDRAC_ACK(u8 *ACK_flag);
        // Clear the flag that indicates newly arrived reception data
extern void rfu_UNI_clearRecvNewDataFlag(u8 slotStatus_Index);
    // NI-type Communication
        // Set transmission data
extern u16  rfu_NI_setSendData(u8 bm_sendSlot,u16 subFrameSize,const void *src,u32 size);
        // Used only by child device. After establishing connection at the RFU level, configure transmission of child device game identification information in order to authenticate connection
extern u16  rfu_NI_CHILD_setSendGameName(u8 slotNo,u16 subFrameSize);
        // Stop the NI data currently being received
extern u16  rfu_NI_stopReceivingData(u8 slotStatus_Index);
    // Shared by NI- and UNI-type communications
        // Narrow transmission targets for transmission data.
extern u16  rfu_changeSendTarget(u8 connType, u8 slotStatus_Index,u8 bm_newTgtSlot);

    // Functions for sending/receiving data to RFU
        // Data transmission
extern u16  rfu_REQ_sendData(void);
        // Used only by parent device. Resend previous sent data (packet)
extern void rfu_REQ_PARENT_resumeRetransmitAndChange(void);
        // Read receive data
extern u16  rfu_REQ_recvData(void);



// For Multi-boot
    // Inherits the information about the link established by the downloader just after the program downloaded with multiboot starts up.
extern u16  rfu_MBOOT_CHILD_inheritanceLinkStatus(void);



// For Debug
    // Obtain address of the SWTI-layer receive buffer
extern u8  *rfu_getSTWIRecvBuffer(void);
    // Obtain RFU state
extern void rfu_REQ_RFUStatus(void);
extern u16  rfu_getRFUStatus(u8 *rfuState);
    // Using RFU, generate noise (jamming radio waves) for other RFUs
extern void rfu_REQ_noise(void);


#ifdef __cplusplus
};
#endif

#endif  // __RFU_LINK_LAYER_H__
