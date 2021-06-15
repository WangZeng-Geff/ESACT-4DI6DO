#ifndef _PROTO_SMART_H_
#define _PROTO_SMART_H_

#include <types.h>
#include <utils.h>


//---------------------------------------------------------------------------------------
/*
 * frame flags
 */
#define STC					0x7E
#define STC_LEN             0x01
#define CMD_LEN             0x01
#define CTRL_LEN            0x01
#define DID_LEN         	0x02
#define SID_LEN         	0x02
#define AID_LEN         	0x04
#define PANID_LEN			0x02
#define BUF_LEN				0xFF
#define EID_LEN				0x08
#define PW_LEN          	0x02
#define PSK_LEN 			0x08
#define SN_LEN          	0x0C
#define DKEY_LEN        	0x08
#define MAGIC_NUM_LEN		0x04
#define WDATA_NUM			0x0A
#define WDATA_LEN			0x40

//---------------------------------------------------------------------------------------
/*
 * frame command
 */
#define CMD_SET_AID     	0x01
#define CMD_GET_AID     	0x03
#define CMD_ACK_AID     	0x13
#define CMD_DEL_AID     	0x04
#define CMD_REQ_AID     	0x05
#define CMD_GET_SID     	0x06
#define CMD_ACK_SID     	0x16
#define CMD_GET_EID     	0x07
#define CMD_ACK_EID     	0x17
#define CMD_SET_BPS     	0x08
#define CMD_SET_REG     	0x09
#define CMD_UNLINK      	0x0A
#define CMD_REGINFOR    	0x0B
#define CMD_SET_PANID   	0x0C
#define CMD_GET_GWAID   	0x0D
#define CMD_GET_VER     	0x0E
#define CMD_ACK_VER     	0x1E
#define CMD_GET_PANID   	0x0F
#define CMD_ACK_PANID   	0x1F
#define CMD_TST_PLC     	0x20
#define CMD_CHG_TONE    	0x21

#define CMD_REQ_UPDATE		0x23

#define CMD_ACK         	0x00
#define CMD_NAK         	0xFF

#define CMD_SET         	0x07
#define CMD_GET        		0x02
#define CMD_UPDATE      	0x05
#define CMD_RELI_REPORT     0x01
#define CMD_NRELI_REPORT    0x00


#define PRESSKEY_REG     	0x01
#define PASSWORD_REG     	0x00
#define PASSWORD_ERR     	0x02
//---------------------------------------------------------------------------------------
/*
 * frame error no
 */
#define NO_ERR        		0x00
#define OTHER_ERR     		0x0F
#define LEN_ERR       		0x01
#define BUFFER_ERR    		0x02
#define DATA_ERR      		0x03
#define DID_ERR       		0x04
#define DEV_BUSY      		0x05
#define NO_RETURN     		0x10
#define DATA_TRANS    		0x12

/*
 * DID definitions
 */
enum
{
    DID_DEVTYPE     =   0x0001,
    DID_COMMVER     =   0x0002,
    DID_SOFTVER     =   0x0003,
    DID_DEVKEY      =   0x0005,
    DID_DEVSN       =   0x0007,
    DID_CHKPWD      =   0xC030,
    DID_RELAY_MODE  =   0xC060,

    DID_CTRLINFO    =	0xFB20,

    //relay
    DID_ONOFF    	=	0xC012,
    DID_REVERSE    	=	0xC018,
    DID_CALTM	    =	0xC020,
    //key
    DID_KEYCONF     =   0xFC19,
    DID_KEYLOCK     =   0xE014,
    DID_RETRY_INFOR =   0x0802,
    DID_CERTAIN_TIME=   0xD702,
    DID_TEMP        =   0xB691,
    DID_RESET_PARA  =   0xCD00,
    DID_REPORT    	=	0xD005,
    DID_OPCNT      	=	0xC132,
    DID_DEBACK    	=	0xC135,
    DID_OUT_PARA  	=	0xC136,
    DID_RELAY_MUTEX	=	0xB624,

    DID_DEVSHOW     =	0x0009,
    DID_PWD		    =	0x000A,
    DID_SILENT	    =	0x000B,
    DID_REBOOT    	=   0xFEFD,
};

enum
{
    INVALID_SIG   = 0x00,
    SELF_LOCK_KEY = 0x01,
    SELF_RESET_KEY = 0x02,
    NORMALLY_OPEND_SENSOR = 0x03,
    NORMALLY_CLOSED_SENSOR = 0x04,
};

enum RELAY_PARA
{
    NORMAL_STATE = 0x01,
    OUT_MODE     = 0x02,
    OUT_TIME     = 0x03,
};

enum RELAY_VIR_STATE
{
    RELAY_NORMAL_OPEN    = 0x01,
    RELAY_NORMAL_CLOSED  = 0x02,
};

enum RELAY_OUT_MODE
{
    RELAY_OUT_LEVEL    = 0x01,
    RELAY_OUT_PULSE    = 0x02,
};

enum RELAY_CTRL_SOURCE
{
    KEY    = 0x01,
    REMOTE  = 0x02,
    AUTO    = 0x03,
};

enum KEY_CONFIG_SAVE
{
    CH1  = 0x00,
    CH2  = 0x01,
    CH3  = 0x02,
    CH4  = 0x03,
};

enum
{
    BROAD_CAST  = 0x00,
    SINGLE_CAST = 0x01,
    KEY_REPORT  = 0x02,
};

enum KEY_CONFIG_ACTION
{
    INQUIRY     = 0x00,
    CLEAR_CHN   = 0x01,
    REVERSE_CHN = 0x02,
};

enum SELF_RESET_KEY
{
    INVALID       = 0x00,
    SHORT_PRESS   = 0x01,
    DOUBLE_PRESS  = 0x02,
    LONG_PRESS    = 0x03,
    INVALID_KEY_TYPE  = 0x04,
};

enum SELF_LOCK_KEY
{
    KEY_UP     = 0x01,
    KEY_DOWN   = 0x02,
};

#pragma pack(1)
typedef struct SmartFrame
{
    uint8_t stc;
    uint8_t said[AID_LEN];
    uint8_t taid[AID_LEN];
    uint8_t seq;
    uint8_t len;
    uint8_t data[1];
} smart_frame_t;
#define SMART_FRAME_HEAD offset_of(smart_frame_t, data)

struct AppFrame
{
    uint8_t cmd;
    uint8_t data[0];
};
struct GroupFrame
{
    uint8_t len : 6;
    uint8_t type: 2;
    uint8_t data[1];
};

typedef struct Body
{
    uint8_t did[DID_LEN];
    uint8_t ctrl;
    uint8_t data[0];
} body_t;
#define SMART_BODY_HEAD offset_of(body_t, data)
typedef struct DecodeBody
{
    uint8_t aid[AID_LEN];
    uint8_t len;
    body_t  body[30];
} de_body_t;
#define SMART_DEBODY_HEAD offset_of(de_body_t, body)

typedef struct 
{
    uint8_t act;
    uint8_t chn;
    uint8_t virtual_chn_index;
}switch_config_t;

#define FBD_FRAME_HEAD  offset_of(body_t, data)

struct RegData
{
    uint8_t aid[AID_LEN];
    uint8_t panid[SID_LEN];
    uint8_t pw[PW_LEN];
    uint8_t gid[AID_LEN];
    uint8_t sid[SID_LEN];
};

#pragma pack()

static inline int smart_frame_len(const smart_frame_t *frame)
{
    return SMART_FRAME_HEAD + frame->len + 1;
}
static inline bool smart_frame_is_broadcast(const smart_frame_t *frame)
{
    return is_all_xx(frame->taid, 0xff, AID_LEN);
}
static inline bool smart_frame_is_local(const smart_frame_t *pframe)
{
    return is_all_xx(pframe->said, 0x00, AID_LEN) && is_all_xx(pframe->taid, 0x00, AID_LEN);
}
static inline bool smart_frame_is_ack(const smart_frame_t *frame)
{
    return tst_bit(frame->seq, 7);
}
static inline int smart_frame_body_len(const body_t *body);

smart_frame_t *get_smart_frame(const uint8_t *in, uint32_t len);
int code_body(uint16_t did, int err, const void *data, int len, void *out, int maxlen);
int code_frame(const uint8_t *src, const uint8_t *dest, int seq, int cmd, \
               uint8_t *data, int len, void *out, int maxlen);
int code_local_frame(const uint8_t *in, int len, void *out, int maxlen);
int code_ret_frame(smart_frame_t *pframe, int len);
bool is_gid_equal(const uint8_t *data, uint16_t mysid);
int remote_frame_opt(struct SmartFrame *pframe);
uint8_t led_separate_digital_output(uint8_t in);
uint8_t relay_separate_digital_output(uint8_t in);
//void set_led_status(uint8_t said[],uint8_t ctrl_fbd[]);
void switch_param_default(uint8_t chn);

#endif
