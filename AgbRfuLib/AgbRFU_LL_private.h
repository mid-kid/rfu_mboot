#ifndef __RFU_LINK_LAYER_PRV_H__
#define __RFU_LINK_LAYER_PRV_H__

#define ID_VERSION_STATUS_REQ 0x0012
#define ID_CONFIG_STATUS_REQ 0x0015
#define ID_SLOT_STATUS_REQ 0x0014
#define ID_DATA_EX_REQ 0x0022
#define ID_DATA_EX_AND_CHANGE_REQ 0x0023
#define ID_DATA_TX_AND_CHANGE_REQ 0x0025
#define ID_KEY_SET_CONFIG_REQ 0x0018
#define ID_KEY_SET_MAKE_AND_CHANGE_REQ 0x0035
#define ID_KEY_SET_READY_AND_CHANGE_REQ 0x0036
#define ID_KSRAC_REQ_WITH_ACK_FLAG 0x0136
#define ID_P_KEY_SET_RX_REQ 0x0038
#define ID_C_KEY_SET_RX_REQ 0x0039
#define ID_RESET_ACK 0x0090
#define ID_LINK_STATUS_ACK 0x0091
#define ID_VERSION_STATUS_ACK 0x0092
#define ID_SYSTEM_STATUS_ACK 0x0093
#define ID_SLOT_STATUS_ACK 0x0094
#define ID_CONFIG_STATUS_ACK 0x0095
#define ID_GAME_CONFIG_ACK 0x0096
#define ID_SYSTEM_CONFIG_ACK 0x0097
#define ID_KEY_SET_CONFIG_ACK 0x0098
#define ID_SC_START_ACK 0x0099
#define ID_SC_POLL_ACK 0x009a
#define ID_SC_END_ACK 0x009b
#define ID_SP_START_ACK 0x009c
#define ID_SP_POLL_ACK 0x009d
#define ID_SP_END_ACK 0x009e
#define ID_CP_START_ACK 0x009f
#define ID_CP_POLL_ACK 0x00a0
#define ID_CP_END_ACK 0x00a1
#define ID_DATA_EX_ACK 0x00a2
#define ID_DATA_EX_AND_CHANGE_ACK 0x00a3
#define ID_DATA_TX_ACK 0x00a4
#define ID_DATA_TX_AND_CHANGE_ACK 0x00a5
#define ID_DATA_RX_ACK 0x00a6
#define ID_MS_CHANGE_ACK 0x00a7
#define ID_DATA_READY_AND_CHANGE_ACK 0x00a8
#define ID_DISCONNECTED_AND_CHANGE_ACK 0x00a9
#define ID_DISCONNECT_ACK 0x00b0
#define ID_TEST_MODE_ACK 0x00b1
#define ID_CPR_START_ACK 0x00b2
#define ID_CPR_POLL_ACK 0x00b3
#define ID_CPR_END_ACK 0x00b4
#define ID_KEY_SET_MAKE_AND_CHANGE_ACK 0x00b5
#define ID_KEY_SET_READY_AND_CHANGE_ACK 0x00b6
#define ID_RESUME_RETRANSMIT_AND_CHANGE_ACK 0x00b7
#define ID_P_KEY_SET_RX_ACK 0x00b8
#define ID_C_KEY_SET_RX_ACK 0x00b9
#define ID_STOP_MODE_ACK 0x00bd
#define ID_ACK_REJECTION 0x00ee
#define FLAG_LANGUAGE 0x01
#define FLAG_AGB_CLOCK_SLAVE 0x02
#define FLAG_WATCH_DOG_TIMER_ON 0x04
#define FLAG_REQ_CALLBACK_ON 0x08
#define FLAG_PARENT_CHILD 0x80
#define FAST_COPY_BUFF_SIZE 0x60

typedef struct RFU_staticTag {
	u8 unk_01;  // unused
	u8 MS_mode;
	u8 unk_03;  // unused
	u8 now_watchInterval;
	u8 ni_end_recv_flag;
	u8 recv_renewal_flag;
	u8 send_renewal_flag;
	u8 unk_08;  // unused
	u8 unk_09;  // unused
	u8 commExist_flag;
	u8 unk_11;  // unused
	vu8 llf_ready_flag;
	u16 cid_bak;
}RFU_STATIC;

typedef struct RFU_fixedTag {
	u8 *dst;
	void (*fastCopy_p)();
	u8 fastCopy_buff[0x60];
	u8 STWI_buf[280];
}RFU_FIXED;

extern RFU_STATIC rfuStatic;
extern RFU_FIXED rfuFixed;

#endif

