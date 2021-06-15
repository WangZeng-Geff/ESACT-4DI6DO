#ifndef _SETTINGS_H_
#define _SETTINGS_H_

#include <types.h>

#define FLASH_SECTOR_SIZE       (1024u)
#define FLASH_SECTOR_MASK       (FLASH_SECTOR_SIZE - 1)

#define SETTING_MAGIC           (0x55AAu)
#define SETTING_FROM            (70 * 1024u)
#define SETTING_SIZE            (2 * 1024u)
#define SETTING_INVALID_ADDR    (SETTING_FROM + SETTING_SIZE)

#define MAGIC_SIZE                   (2)
#define SWITCH_PARAM_MAGIC           (0xA5A5u)
#define SWITCH_PARAM_SIZE            (1024u)
#define SWITCH_PARAM_FROM            (56 * 1024u)


#define RESTORE_KEY_NUM  		4
#define MAX_CHN_NUM             4
//#define RESTORE_TRAIC_NUM  		2
#define RESTORE_RELAY_NUM       2
#define RESTORE_LED_NUM         4
#define RESTORE_DO_NUM          6

//#define TRIAC_CTRL_KEEP
#define LED_CTRL_KEEP

#define MAX_KEY_TYPE      0x03  //short long double_click //keyup keydown 
#define MAX_DEV_CNT	      0x0C
#define MAX_TYPEBODY_LEN	      0x80
#define ID_LEN            0x04
#define NAME_LEN          0x0F



#define FLAG_SEND_GATEWAY 15
typedef struct SendInfor
{
	uint8_t pos:4;//local link dev pos
	uint8_t local_linkctrl:4;//start local dev ctrl
	uint8_t retry;//ctr dev retry
	uint8_t  seq[MAX_DEV_CNT];
	uint16_t flag_send;
	uint16_t flag_ask;
	uint8_t aid_pos[MAX_DEV_CNT];

}send_infor_t;


#define TRIGGER_BROADCAST_LED 3  //when trigger the broadcast ,the led turn on 3S
#define TRIGGER_KEYREPORT_LED 10

typedef struct 
{
	uint8_t aid[4];
    uint8_t chn;
}led_need_sync_chn_t;

typedef struct 
{
	uint8_t  flag;
    uint16_t temp;
}tempover_t;

typedef struct DevStateInfo
{
    uint8_t  flag_write_SN;
    uint8_t  ex_sensor_state;
    uint8_t  ex_sensor_state_bak;
    uint8_t  flag_key_state;//down or up
    uint8_t  flag_key_report;
    uint8_t  time_keyreport[RESTORE_KEY_NUM];
  	
    uint8_t  param_save;
    uint8_t  state_led;    
	uint8_t  state_led_bak;
    uint8_t  act_chn;
    uint8_t  need_act;
    uint8_t  state_digtal_output;
    uint8_t  state_digtal_output_back;
    uint8_t  state_relay;
	uint8_t  state_relay_bak;
    uint8_t  flag_broadcast;
    uint8_t  time_broadcast[RESTORE_KEY_NUM];
  	uint8_t  need_act_led;
    uint8_t  need_act_relay;
  	uint8_t  need_restore;
    uint8_t  flag_para_report;
    uint8_t  relay_to_normal;
    uint8_t  relay_to_normal_cnt[RESTORE_RELAY_NUM];
    
    tempover_t temp_over;
	send_infor_t send_infor[RESTORE_KEY_NUM][MAX_KEY_TYPE];
    uint8_t send_interval_count;
    uint8_t key_state[RESTORE_KEY_NUM];
    led_need_sync_chn_t  led_need_sync_chn[RESTORE_KEY_NUM];
    uint16_t output_time[RESTORE_RELAY_NUM];
    uint8_t flag_relay_ctrl[RESTORE_RELAY_NUM];
} dev_state_info_t;
extern dev_state_info_t dev_state;



typedef struct
{
    uint8_t send_type;
    uint8_t len;
    uint8_t body[MAX_TYPEBODY_LEN];
}virtual_chn_typebody_t;
#define VIRTUAL_CHN_HEAD offset_of(virtual_chn_typebody_t, body)

typedef struct
{
    uint8_t aid[4];
    uint8_t len;
    uint8_t did[2];
    uint8_t data_ctrl;
    uint8_t data[128];
}key_config_t;
#define HEAD_KEY_CONFIG (4+1+2+1)

typedef struct setting_hdr
{
    uint16_t magic;
    uint16_t crc;
} setting_hdr_t;

#define LEN_SWITCH_PARAM (2)//signal_type,name_len
#define MIN_SWITCH_SETTING_DATA_LEN (7)//action,chn,chn_signal_type,virtual_chn_index,name_len,dev_type,fbd_len
typedef struct
{
    setting_hdr_t hdr; //4B
    uint8_t action;
    uint8_t chn_id;
    uint8_t virtual_chn_id;
    uint8_t signal_type;
    uint8_t name_len;
    uint8_t name[NAME_LEN];
    virtual_chn_typebody_t vir_typebody[MAX_KEY_TYPE];
}__attribute__((aligned(4)))switch_param_t;

#define SWITCH_PARAM_HEAD  ((offset_of(switch_param_t, name) - sizeof(setting_hdr_t)))
extern switch_param_t switch_param[RESTORE_KEY_NUM];



typedef struct
{
    uint8_t sn[12];
    uint8_t dkey[8];
    uint8_t id[4];
    uint8_t pwd[2];
} encode_t;

typedef struct
{
	uint8_t gid[4];
	uint8_t panid[2];
	uint8_t sid[2];
} para_t;

typedef struct
{
	uint8_t  normal_state;
    uint8_t  out_mode;
    uint16_t output_time;
} relay_para_t;

typedef struct DevInfo
{
	uint8_t  report;
	uint8_t  lock;
	uint8_t	 try_cnt;
  	uint8_t	 interval;
    uint16_t opt_cnt[RESTORE_RELAY_NUM];
    uint8_t  relay_tm[RESTORE_RELAY_NUM *2];
    uint8_t  state_digtal_output;
    uint8_t  relay_poweron_mode;
    relay_para_t relay_para[RESTORE_RELAY_NUM];
    uint8_t  flag_relay_ctrl_mutex;
  	uint8_t  reverse[16];
} dev_info_t;

typedef struct
{
    setting_hdr_t hdr;//4B

    /* device */
    encode_t encode;//26B
 
    /* para */
	para_t para;//8B

	/* dev */
	dev_info_t dev_infor; //12B
} __attribute__((aligned(4)))setting_t;

#define SETTING_HDR_SIZE       (sizeof(setting_hdr_t))
#define SETTING_DAT_SIZE(s)    (s - sizeof(setting_hdr_t))
#define setting_addr(s)        ((uint8_t*)s + SETTING_HDR_SIZE)

#define SWITCH_HDR_SIZE       (sizeof(setting_hdr_t))
#define SWITCH_DAT_SIZE(s)    (s - sizeof(setting_hdr_t))
#define SWITCH_addr(s)        ((uint8_t*)s + SWITCH_HDR_SIZE)

extern setting_t setting;

int disk_erase(uint32_t start, uint32_t size);
int disk_write(uint32_t start, const void *data, uint32_t size);
int disk_read(uint32_t start, void *data, uint32_t size);


void dev_restore_factory(void);
void setting_save(void);
void switch_param_save(uint8_t idx, void * data);
void setting_load(void);
bool switch_param_load(uint8_t idx, void * data);

#endif
