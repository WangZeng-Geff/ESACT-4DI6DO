#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_

#include <stdint.h>
//---------------------------------------------------------------------------------------
#define STC             0x7E
#define DID_LEN         0x02
#define SID_LEN         0x02
#define GID_LEN         0x02
#define AID_LEN         0x04
#define PANID_LEN		0x02
#define BUF_LEN			0xFF
#define EID_LEN			0x08
#define PW_LEN          0x02
#define PSK_LEN 		0x08

//---------------------------------------------------------------------------------------
#define CMD_SET_AID     0x01
#define CMD_GET_AID     0x03
#define CMD_ACK_AID     0x13
#define CMD_DEL_AID     0x04
#define CMD_REQ_AID     0x05
#define CMD_GET_SID     0x06
#define CMD_ACK_SID     0x16
#define CMD_GET_EID     0x07
#define CMD_ACK_EID     0x17
#define CMD_SET_BPS     0x08
#define CMD_SET_REG     0x09
#define CMD_UNLINK      0x0A
#define CMD_REGINFOR    0x0B
#define CMD_SET_PANID   0x0C
#define CMD_GET_GWAID   0x0D
#define CMD_GET_VER     0x0E
#define CMD_ACK_VER     0x1E
#define CMD_GET_PANID   0x0F
#define CMD_ACK_PANID   0x1F

#define CMD_UPDATE		0xFE

#define CMD_ACK         0x00
#define CMD_NAK         0xFF

//---------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------

#pragma pack(1)
struct SmartFrame
{
    uint8_t stc;
    uint8_t said[AID_LEN];
    uint8_t taid[AID_LEN];
    uint8_t seq;
    uint8_t len;
    uint8_t data[1];
};
#define SMART_FRAME_HEAD offsetof(struct SmartFrame, data)
#pragma pack()
//---------------------------------------------------------------------------------------
#define frame_len(frame) (SMART_FRAME_HEAD + frame->len + 1)
//---------------------------------------------------------------------------------------
extern uint8_t g_frame_cache[0x100];
//---------------------------------------------------------------------------------------
struct SmartFrame *get_smart_frame(const uint8_t *in, uint8_t len);
uint16_t code_local_frame(const uint8_t *in, uint16_t len, void *out);
int8_t smart_frame_handle(struct SmartFrame *frame);
//---------------------------------------------------------------------------------------
#endif
