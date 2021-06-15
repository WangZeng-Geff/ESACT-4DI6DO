#include <config.h>
#include <compiler.h>
#include <types.h>
#include <utils.h>
#include <printk.h>
#include "dev.h"
#include "settings.h"
#include "board.h"
#include "update.h"
#include "proto_smart.h"
#include "plc_smart.h"
#include "dev_show.h"
#include "report\\auto_report_app.h"


/*
 * frame decode
 */
smart_frame_t *get_smart_frame(const uint8_t *in, uint32_t len)
{
    int i = 0;

    start_lbl:
    while (i < len)
    {
        if (STC == in[i])
            break;
        i++;
    }
    if (len - i < SMART_FRAME_HEAD)
        return NULL;
    smart_frame_t *pframe = (struct SmartFrame *)&in[i];
    int dlen = pframe->len;

    if (i + SMART_FRAME_HEAD + dlen + 1 > len)
    {
        i++;
        goto start_lbl;
    }

    if (pframe->data[dlen] != checksum(pframe, dlen + SMART_FRAME_HEAD))
    {
        i++;
        goto start_lbl;
    }
    pframe = (struct SmartFrame *)&in[i];
    return pframe;
}

/*
 * frame encode
 */
int code_frame(const uint8_t *src, const uint8_t *dest, int seq, int cmd, 
    uint8_t *data, int len, void *out, int maxlen)
{
    const uint8_t addr[AID_LEN] = {0x00, 0x00, 0x00, 0x00};
    static uint8_t _seq = 0;
    struct SmartFrame *pframe = (struct SmartFrame *)out;
    memmove(&pframe->data[1], data, len);
    pframe->stc = STC;
    if (!src) src = addr;
    if (!dest) dest = addr;
    memcpy(pframe->said, src, AID_LEN);
    memcpy(pframe->taid, dest, AID_LEN);
    pframe->seq = seq < 0 ? (_seq++ & 0x7F) : seq;

    pframe->data[0] = cmd;
//    memcpy(&pframe->data[1], data, len);
    pframe->len = len+1;
    pframe->data[pframe->len] = checksum(pframe, SMART_FRAME_HEAD + pframe->len);
    return SMART_FRAME_HEAD + pframe->len + 1;
}

int code_local_frame(const uint8_t *in, int len, void *out, int maxlen)
{
    return code_frame(NULL, NULL, 0, in[0], (uint8_t *)&in[1], len-1, out, maxlen);
}

int code_ret_frame(struct SmartFrame *pframe, int len)
{
    uint8_t tmp[AID_LEN];

    memcpy(tmp,          pframe->taid, sizeof(pframe->taid)); 
    memcpy(pframe->taid, pframe->said, sizeof(pframe->taid)); 
    memcpy(pframe->said, tmp,          sizeof(pframe->said)); 
    pframe->seq |= 0x80;
    pframe->len  = len;
    pframe->data[len] = checksum(pframe, pframe->len + SMART_FRAME_HEAD);
    return pframe->len + SMART_FRAME_HEAD + 1;
}

int code_body(uint16_t did, int err, const void *data, int len, void *out, int maxlen)
{
    body_t *body = (body_t *)out;

    put_le_val(did, body->did, sizeof(body->did));
    body->ctrl = get_bits(len, 0, 6);
    if (err) body->ctrl |= 0x80;
    memcpy(body->data, data, len);

    return sizeof(body_t) + len;
}

/*
 * doing protocol 
 */
static int do_reboot(const uint8_t *in, size_t len, uint8_t *out, size_t maxlen)
{
    int ms100 = 20;

    if (len == 1) ms100 = in[0];
    board_reboot(ms100);
    return 0;
}

static int do_get_dev_type(const uint8_t *in, size_t len, uint8_t *out, size_t maxlen)
{
    return dev_type_get(out, maxlen);
}

static int do_get_soft_ver(const uint8_t *in, size_t len, uint8_t *out, size_t maxlen)
{
    return dev_ver_get(out, maxlen);
}

static int get_dev_key(const uint8_t *in, size_t len, uint8_t *out, size_t maxlen)
{
    memcpy(out, setting.encode.dkey, sizeof(setting.encode.dkey));
    return sizeof(setting.encode.dkey);
}

static int get_dev_sn(const uint8_t *in, size_t len, uint8_t *out, size_t maxlen)
{
    memcpy(out, setting.encode.sn, sizeof(setting.encode.sn));
    return sizeof(setting.encode.sn);
}

static int get_app_comm_ver(const uint8_t *in, size_t len, uint8_t *out, size_t maxlen)
{
	uint8_t ver_len;
	const char ver[] = "EASTSOFT(v1.0)";

	ver_len = (char)strlen(ver);
	if (maxlen < ver_len)
		return (-BUFFER_ERR);

	memcpy(out, ver, ver_len);
	return (ver_len);
}

static int check_password(const uint8_t *in, size_t len, uint8_t *out, size_t maxlen)
{
    if (len != 2) return(-DATA_ERR);
	
	out[0] = !memcmp(setting.encode.pwd, in, 2);
	
	return(1);
}
//------------------------------------------------------------------------------------
uint8_t led_separate_digital_output(uint8_t in)
{
    uint8_t led_chn = 0;      
    led_chn |= in & 0x80; 
    led_chn |= in & 0x0F;    
    return led_chn;
}
uint8_t relay_separate_digital_output(uint8_t in)
{
    uint8_t relay_chn = 0;  
    relay_chn |= in & 0x30;
    relay_chn  = relay_chn >> 4;
    relay_chn |= in & 0x80; 
    return relay_chn;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           
}
static int get_digital_output_state(const uint8_t *in, size_t len, uint8_t *out, size_t maxlen)
{
	if (len != 0) return(-DATA_ERR);

	out[0] = setting.dev_infor.state_digtal_output;
	return (1);
}

#define MAX_DIGITAL_OUTPUT_CHN 0x3F
 int set_digital_output_state(const uint8_t *in, size_t len, uint8_t *out, size_t maxlen)
{
	if (len != 0x01 ) return(-LEN_ERR);
    if ((in[0] & 0x7F) > MAX_DIGITAL_OUTPUT_CHN) return(-DATA_ERR);
	out[0] = relay_status_save(relay_separate_digital_output(in[0]), ON_OFF,REMOTE)<<4;
    out[0] |= led_status_save(led_separate_digital_output(in[0]), ON_OFF);
//    set_led_status(setting.encode.id,&dev_state.state_relay);
	return (1);
}

static int set_digital_output_reverse(const uint8_t *in, size_t len, uint8_t *out, size_t maxlen)
{
	if (len != 0x01) return(-DATA_ERR);
    if ((in[0]) > MAX_DIGITAL_OUTPUT_CHN) return(-DATA_ERR);

	out[0] = relay_status_save(relay_separate_digital_output(in[0]), REVERSE,REMOTE)<<4;
    out[0] |= led_status_save(led_separate_digital_output(in[0]), REVERSE);
	return (1);
}

//static int get_delay_tm(const uint8_t *in, size_t len, uint8_t *out, size_t maxlen)
//{
//    uint8_t i, chn, j = 0;
//    
//	if (len != 1 || !in[0] ) return(-DATA_ERR);
//    
//	chn = out[j++] = in[0];
//    
//    for (i = 0; i < RESTORE_RELAY_NUM; i++)
//   {
//      if (tst_bit(chn, i))
//      {
//         out[j++] =  setting.dev_infor.relay_tm[2*i];
//         out[j++] =  setting.dev_infor.relay_tm[2*i + 1];
//      }
//   }
//   
//   return (j);
//}

//static int set_delay_tm(const uint8_t *in, size_t len, uint8_t *out, size_t maxlen)
//{
//    uint8_t i, j = 1;
//    
//	if (len < 3 || !in[0]  ) return (-DATA_ERR);
//    
//	 for (i = 0; i < RESTORE_RELAY_NUM; i++)
//    {
//      if (tst_bit(in[0], i))
//      {
//         setting.dev_infor.relay_tm[2*i] = in[j++];
//         setting.dev_infor.relay_tm[2*i + 1] = in[j++];
//      }
//    }
//	setting_save();
//	return (NO_ERR);
//}

static int get_report(const uint8_t *in, size_t len, uint8_t *out, size_t maxlen)
{
	if (len != 0) return(-DATA_ERR);
	out[0] = 0x00;
	out[1] = setting.dev_infor.report;
    return (2);
}

static int set_report(const uint8_t *in, size_t len, uint8_t *out, size_t maxlen)
{
	if ((len != 2) || (in[1] > 3)) return (-DATA_ERR);
	if(setting.dev_infor.report != in[1])
	{
		setting.dev_infor.report = in[1];
		setting_save();
	}
	return (NO_ERR);
}

static int get_key_config(const uint8_t *in, size_t len, uint8_t *out, size_t maxlen)
{
    if(len != sizeof(switch_config_t))  return (-LEN_ERR);
    
    switch_config_t *switch_config = (switch_config_t *)in; 
    if(switch_config->act != INQUIRY ) return (-DATA_ERR);
    if(switch_config->chn == 0 ||switch_config->chn > RESTORE_KEY_NUM) return (-DATA_ERR);
    if(switch_config->virtual_chn_index == 0 ||switch_config->virtual_chn_index >= INVALID_KEY_TYPE) return (-DATA_ERR);
    
    uint8_t chn = switch_config->chn - 1;
    uint8_t vir_chn = switch_config->virtual_chn_index - 1;
    
    switch_param[chn].virtual_chn_id = switch_config->virtual_chn_index;
    
    memcpy(out, switch_config, sizeof(switch_config_t));
    
    memcpy(out + sizeof(switch_config_t), &switch_param[chn].signal_type, LEN_SWITCH_PARAM + switch_param[chn].name_len); 
    
    memcpy(out +sizeof(switch_config_t) + LEN_SWITCH_PARAM + switch_param[chn].name_len, \
    &switch_param[chn].vir_typebody[vir_chn],\
    switch_param[chn].vir_typebody[vir_chn].len +  + VIRTUAL_CHN_HEAD); 
    
    return(switch_param[chn].vir_typebody[vir_chn].len + switch_param[chn].name_len + MIN_SWITCH_SETTING_DATA_LEN);
}
static switch_param_t switch_param_new ;
static void clear_key_config()
{
    uint8_t chn ,vir_chn;
    chn = switch_param_new.chn_id - 1;
    if (switch_param_new.virtual_chn_id == 0xFF)
    {
        memset(&switch_param[chn], 0x00, sizeof(switch_param_t));
    }
    else
    {
        for(vir_chn = 0;vir_chn < LONG_PRESS;vir_chn++)
        {
            if(tst_bit(switch_param_new.virtual_chn_id,vir_chn))
            {
                memset(&switch_param[chn].vir_typebody[vir_chn], 0x00, MAX_TYPEBODY_LEN);
            }
        }
    }
    switch_param_save(chn,&switch_param[chn]);
    //check_chn_need_sync(chn);
    //led_status_save((1<<(chn)), ON_OFF);
    
}    
static int set_key_config(const uint8_t *in, size_t len, uint8_t *out, size_t maxlen)
{  
    
    uint8_t chn ;
    uint8_t vir_chn ;
    
    dev_state.flag_para_report = 0;
    if(len < sizeof(switch_config_t)) return (-LEN_ERR);
    
    memset(&switch_param_new, 0x00, sizeof(switch_param_t));  
    memcpy(&switch_param_new.action, in, sizeof(switch_config_t));
    
    if(switch_param_new.action == INQUIRY ||switch_param_new.action > REVERSE_CHN)
        return (-DATA_ERR);
    if(switch_param_new.chn_id == INVALID  || switch_param_new.chn_id > RESTORE_KEY_NUM)  
        return (-DATA_ERR);
    
    if(switch_param_new.action == CLEAR_CHN)
    {
        if(len > sizeof(switch_config_t)) return (-LEN_ERR);
        clear_key_config();
        return(NO_ERR);	
    }
          
    memcpy(&switch_param_new.action, in, SWITCH_PARAM_HEAD); 
    memcpy(&switch_param_new.name, in + SWITCH_PARAM_HEAD, switch_param_new.name_len); 
    
    if(switch_param_new.virtual_chn_id == INVALID  || switch_param_new.virtual_chn_id >= INVALID_KEY_TYPE ) 
        return (-DATA_ERR);  
    
    vir_chn = switch_param_new.virtual_chn_id - 1;
    memcpy(&switch_param_new.vir_typebody[vir_chn].send_type,\
    in+SWITCH_PARAM_HEAD + switch_param_new.name_len,\
    len - SWITCH_PARAM_HEAD -switch_param_new.name_len); 
    if(switch_param_new.action == INQUIRY ||switch_param_new.action > REVERSE_CHN)
        return (-DATA_ERR);
    
    if(switch_param_new.signal_type == 0 || switch_param_new.signal_type > NORMALLY_CLOSED_SENSOR) 
        return (-DATA_ERR);
    
    if(len != (MIN_SWITCH_SETTING_DATA_LEN + switch_param_new.name_len + switch_param_new.vir_typebody[vir_chn].len))  
    {
        return (-DATA_ERR);
    }
     
    chn = switch_param_new.chn_id - 1;
    clear_local_linkctrl(chn,vir_chn);
    
    if(switch_param_new.signal_type != switch_param[chn].signal_type) //reset the chn
    {
        memcpy(&switch_param[chn],&switch_param_new,sizeof(switch_param_t));
    }
    else                                                                                         //config the chn
    {
        switch_param[chn].name_len = switch_param_new.name_len;
        memcpy(switch_param[chn].name, switch_param_new.name, switch_param_new.name_len);
        memcpy(&switch_param[chn].vir_typebody[vir_chn].send_type,\
        &switch_param_new.vir_typebody[vir_chn].send_type,\
        sizeof(virtual_chn_typebody_t));
    }
    
    switch_param_save(chn,&switch_param[chn]);
    //check_chn_need_sync(chn);
    //led_status_save((1<<(chn)), ON_OFF);
    return(NO_ERR);	
}

static int get_key_lock(const uint8_t *in, size_t len, uint8_t *out, size_t maxlen)
{
	if (len != 0) return(-DATA_ERR);
	out[0] = setting.dev_infor.lock;
    return (1);
}

static int set_key_lock(const uint8_t *in, size_t len, uint8_t *out, size_t maxlen)
{
	if ((len != 1)) return (-DATA_ERR);
	if(setting.dev_infor.lock != in[0])
	{
		setting.dev_infor.lock = in[0];
		setting_save();
	}
	return (NO_ERR);
}

static int get_retry_infor(const uint8_t *in, size_t len, uint8_t *out, size_t maxlen)
{
	if (len != 0) return(-DATA_ERR);
	out[0] = setting.dev_infor.try_cnt;
	out[1] = setting.dev_infor.interval;
  return (2);
}

static int set_retry_infor(const uint8_t *in, size_t len, uint8_t *out, size_t maxlen)
{
	if ((len != 2) || (!in[1] || !in[0])) return (-DATA_ERR);
    if(in[0] > 10 || in[1] >100 || in[1] < 20) return (-DATA_ERR);

	setting.dev_infor.try_cnt = in[0];
    
	setting.dev_infor.interval = in[1];
	setting_save();

	return (NO_ERR);
}



#ifdef DEV_SHOW
static int set_dev_show(const uint8_t *in, size_t len, uint8_t *out, size_t maxlen)
{
	if (len ) return (-DATA_ERR);
	dev_search_param.dev_show_flag = 0x01;
	dev_show_start();
	return (NO_ERR);
}

static int get_password(const uint8_t *in, size_t len, uint8_t *out, size_t maxlen)
{
	if ( len!=0 ) return(-DATA_ERR);

	memcpy(out, setting.encode.pwd, PW_LEN);
    return (PW_LEN);
}

static int set_silent_time(const uint8_t *in, size_t len, uint8_t *out, size_t maxlen)
{
	if (len != 2) return (-DATA_ERR);
	dev_search_param.silent_time = get_le_val(&in[0], TIME_LEN);
	return (NO_ERR);
}
#endif

typedef int (*func_opt_handler_t)(const uint8_t *, size_t, uint8_t *, size_t);
struct func_ops
{
    u16 did;
    const char *tip;
    func_opt_handler_t read;
    func_opt_handler_t write;
};

static void switch_param_set(uint8_t chn)
{
    uint8_t vir_chn;
    if(chn >= RESTORE_RELAY_NUM) return;
    uint8_t str_traic[5] = {0x04,0x12,0xC0,0x01};
    
    switch_param[chn].action = INQUIRY;
    switch_param[chn].chn_id = 1 + chn ;
    switch_param[chn].name_len = 0;
    switch_param[chn].signal_type = SELF_LOCK_KEY;
    
    for(vir_chn = 0;vir_chn <= SELF_LOCK_KEY; vir_chn++)
    {
        switch_param[chn].vir_typebody[vir_chn].send_type = SINGLE_CAST;
        switch_param[chn].vir_typebody[vir_chn].len  = 0x09;
        memcpy(switch_param[chn].vir_typebody[vir_chn].body, setting.encode.id,ID_LEN);
        str_traic[4] = (vir_chn==0)?(1<<(chn+RESTORE_LED_NUM)):(1<<(chn+RESTORE_LED_NUM)|0x80);
        memcpy(switch_param[chn].vir_typebody[vir_chn].body + ID_LEN , &str_traic,sizeof(str_traic));
    }
}
 void switch_param_default(uint8_t chn)
{ 
    memset(SWITCH_addr(&switch_param[chn]), 0x00, SWITCH_DAT_SIZE(sizeof(switch_param_t))); 
    switch_param_set(chn);
    switch_param_save(chn,&switch_param[chn]);
}
static int set_para_reset(const uint8_t *in, size_t len, uint8_t *out, size_t maxlen)
{
    uint8_t chn;
	if (len != 1) return(-DATA_ERR);
    dev_restore_factory();
    refresh_current_state();
    for(chn = 0; chn < RESTORE_KEY_NUM;chn++)
    {
        switch_param_default(chn);
    }
    
    dev_state.flag_para_report = 0x64;
    
    log_d("set_para_reset!\r\n");
	return (NO_ERR);
}

static int set_relay_mode(const uint8_t *in, size_t len, uint8_t *out, size_t maxlen)
{
    
	if (len != 1 || in[0]>0x02 ) return(-DATA_ERR);

    if(setting.dev_infor.relay_poweron_mode != in[0])  
    {
        setting.dev_infor.relay_poweron_mode = in[0];
        setting_save();    
    }

	return (NO_ERR);
}

static int get_relay_mode(const uint8_t *in, size_t len, uint8_t *out, size_t maxlen)
{
	if (len != 0) return(-DATA_ERR);
	out[0] = setting.dev_infor.relay_poweron_mode;
    return (1);
}

//static int set_relay_para2(const uint8_t *in, size_t len, uint8_t *out, size_t maxlen)
//{
//    uint8_t chn = 0;
//    relay_para_t relay_para_temp;
//	
//    
//    chn = in[0];
//    if (len != 5 || !chn ||chn >RESTORE_RELAY_NUM ) return(-DATA_ERR);
//    memcpy(&relay_para_temp,&in[1],sizeof(relay_para_t));
//    
//    if ( !relay_para_temp.normal_state || relay_para_temp.normal_state > RELAY_NORMAL_CLOSED ) return(-DATA_ERR);
//    if ( !relay_para_temp.out_mode || relay_para_temp.out_mode > RELAY_OUT_PULSE ) return(-DATA_ERR);
//    if ( relay_para_temp.output_time > 300 ) return(-DATA_ERR);
//    log_d("relay_para_temp.output_time = %d\r\n",relay_para_temp.output_time);

//    if(memcmp(&relay_para_temp,&setting.dev_infor.relay_para[chn-1],sizeof(relay_para_t)))  
//    {
//        memcpy(&setting.dev_infor.relay_para[chn-1],&in[1],sizeof(relay_para_t));
//        setting_save();    
//    }

//	return (NO_ERR);
//}
static int set_relay_para(const uint8_t *in, size_t len, uint8_t *out, size_t maxlen)
{
    uint8_t chn = 0;
    int err = NO_ERR;
    if (len < 3 ) return(-DATA_ERR);  
    chn = in[0];
    if ( !chn || chn > 2) return(-DATA_ERR);
    
    switch (in[1])
	{
		case NORMAL_STATE:
			if(len != 3 || (in[2]!=RELAY_NORMAL_OPEN && in[2]!=RELAY_NORMAL_CLOSED))
				err =  -DATA_ERR;
			else
			{	
				setting.dev_infor.relay_para[chn-1].normal_state = in[2];
                refresh_current_state();
				dev_state.param_save = 1;
			}
			break;
            
        case OUT_MODE:
			if(len != 3 || (in[2]!=RELAY_OUT_LEVEL && in[2]!=RELAY_OUT_PULSE))
				err =  -DATA_ERR;
			else
			{	
				setting.dev_infor.relay_para[chn-1].out_mode = in[2];
				dev_state.param_save = 1;
			}
			break;

		case OUT_TIME:			
			if(len != 4)
				err =  -DATA_ERR;
			else
			{	
				memcpy(&setting.dev_infor.relay_para[chn-1].output_time,&in[2],2);
				dev_state.param_save = 1;
			}
			break;

		default:
			err = -DATA_ERR;
			break;
	}

	return (err);
}

static int get_relay_para(const uint8_t *in, size_t len, uint8_t *out, size_t maxlen)
{
    uint8_t chn = 0;
    int err = NO_ERR;
	if (len != 2 || !in[0] ||in[0]>0x02 ) return(-DATA_ERR);
    
	chn = out[0] = in[0];
    out[1] = in[1];
    switch (in[1])
	{
		case NORMAL_STATE:
			out[2] = setting.dev_infor.relay_para[chn-1].normal_state;
            err = 3;
			break;
            
        case OUT_MODE:
			out[2] = setting.dev_infor.relay_para[chn-1].out_mode;
            err = 3;
			break;

		case OUT_TIME:			
			memcpy(&out[2],&setting.dev_infor.relay_para[chn-1].output_time,2);
            err = 4;
			break;

		default:
			err = -DATA_ERR;
			break;
	}
    return (err);
}

static int set_relay_flag(const uint8_t *in, size_t len, uint8_t *out, size_t maxlen)
{
    if ((len != 1) || (in[0] > 1)) return (-DATA_ERR);
	if(setting.dev_infor.flag_relay_ctrl_mutex != in[0])
	{
		setting.dev_infor.flag_relay_ctrl_mutex = in[0];
		setting_save();
	}
	return (NO_ERR);
}

static int get_relay_flag(const uint8_t *in, size_t len, uint8_t *out, size_t maxlen)
{
	if (len != 0) return(-DATA_ERR);
	out[0] = setting.dev_infor.flag_relay_ctrl_mutex;
    return (1);
}

static int get_optcnt(const uint8_t *in, size_t len, uint8_t *out, size_t maxlen)
{
	if (len != 1) return(-DATA_ERR);
	int i,j = 1;
	out[0] = 0;
	for(i = 0; i < RESTORE_RELAY_NUM ; i++)
	{
		if(tst_bit(in[0], i))
		{
			set_bit(out[0], i);
			put_be_val(setting.dev_infor.opt_cnt[i],&out[j],2);
			j += 2;
		}
	}
    return (j);
}

static const struct func_ops func_items[] =
{
    {DID_REBOOT,    	"reboot",     	NULL,            	do_reboot},
    {DID_SOFTVER,   	"softver",    	do_get_soft_ver, 	NULL},
    {DID_DEVTYPE,   	"devtype",    	do_get_dev_type, 	NULL},
    {DID_DEVSN,     	"SN",         	get_dev_sn,      	NULL},
    {DID_DEVKEY,   	    "devkey",     	get_dev_key,     	NULL},
  	{DID_COMMVER,   	"commver",	  	get_app_comm_ver,	NULL},
    {DID_CHKPWD,    	"chkpwd",	    NULL,     	   	    check_password},
	{DID_ONOFF,    		"ctrl",       	get_digital_output_state, 	set_digital_output_state},
  	{DID_REVERSE,   	"reverse",    	NULL,     	   		set_digital_output_reverse},
  	{DID_REPORT,    	"report attr",	get_report,      	set_report},
    //{DID_CALTM,    	    "calibration",  get_delay_tm,       set_delay_tm},
    {DID_OPCNT,    	    "count",        get_optcnt,         NULL},
  	{DID_KEYCONF,       "key_config", 	get_key_config,		set_key_config},
	{DID_KEYLOCK,       "key_lock",   	get_key_lock,  		set_key_lock},
	{DID_RETRY_INFOR,   "retry_infor", 	get_retry_infor,    set_retry_infor},
    {DID_RESET_PARA,    "reset_para",   NULL,               set_para_reset},
    {DID_RELAY_MODE,    "relay_mode",   get_relay_mode,     set_relay_mode},
    {DID_OUT_PARA,      "relay_para",   get_relay_para,     set_relay_para},
    {DID_RELAY_MUTEX,   "relay_flag",   get_relay_flag,     set_relay_flag},
#ifdef DEV_SHOW
	{DID_DEVSHOW,   	"devshow",    	NULL,   		    set_dev_show},
  	{DID_PWD,       	"get pwd",    	get_password,       NULL},
  	{DID_SILENT,    	"silent",     	NULL,   		    set_silent_time},
#endif
};
static const struct func_ops *get_option(int did)
{
    int i;
    for (i = 0; i < array_size(func_items); i++)
        if (func_items[i].did == did)
            return &func_items[i];
    return NULL;
}


static inline int form_error_body(u16 did, uint8_t err, void *out, size_t maxlen)
{
    uint8_t data[2] = {0x00, 0x00};
	data[0] = err;
	return code_body(did, true, data, sizeof(data), out, maxlen);
}

static inline int smart_frame_body_len(const body_t *body)
{
    return get_bits(body->ctrl, 0, 6);
}

static int do_cmd(int cmd, uint8_t *data, int len)
{
    int outidx = 0, inidx = 0;

    uint8_t tmp[BUF_LEN];
    memcpy(tmp, data, len);

    while (len >= FBD_FRAME_HEAD)
    {
        body_t *body = (body_t *)&tmp[inidx];
        body_t *outbd = (body_t *)&data[outidx];

        uint16_t did = get_le_val(body->did, sizeof(body->did));
        int dlen = smart_frame_body_len(body);
        if (len < FBD_FRAME_HEAD + dlen)
        {
#ifdef PLC_SMART_DEBUG
            log_d("Smart frame, body length error!\n");
#endif
            outidx += form_error_body(did, LEN_ERR, outbd, BUF_LEN);
            break;
        }
        inidx += FBD_FRAME_HEAD + dlen;
        len -= FBD_FRAME_HEAD + dlen;

        const struct func_ops *op = get_option(did);
        int (*handler)(const uint8_t *in, size_t len, uint8_t *out, size_t maxlen) = NULL;
        handler = op ? ((cmd == CMD_GET) ? op->read : op->write) : NULL;
        if (handler)
        {
#ifdef PLC_SMART_DEBUG
			log_d("Do cmd[%s]...\n", op->tip);
#endif
            memcpy(outbd, body, sizeof(body_t) + dlen);
            uint8_t maxlen = 128;
            int ret = handler(body->data, dlen, outbd->data, maxlen); 
            if (ret < 0)
            {
                if (ret == -DATA_TRANS)
                    return -1;
                if (ret == -NO_RETURN)
                    continue;
                form_error_body(did, -ret, outbd, BUF_LEN);
            }
            else if (ret > 0)
            {
                memcpy(outbd->did, body->did, sizeof(body->did));
				if(outbd->did[0] == 0x18)
					outbd->did[0] = 0x12;
                outbd->ctrl = ret;
            }
            else
            {
                body->ctrl = 0;
            }
        }
        else
        {
            form_error_body(did, DID_ERR, outbd, BUF_LEN);
        }
        outidx += FBD_FRAME_HEAD + smart_frame_body_len(outbd);
    }
    return outidx;
}

//---------------------------------------------------------------------------------------
/* 
 data[0]: bit[0~5]: data length
          bit[6~7]: group type
 group_type: 0: bit
             1: 1byte  group id
             2: 2bytes group id
*/
bool is_gid_equal(const uint8_t *data, uint16_t mysid)
{
    int dlen = get_bits(data[0], 0, 5);
    int group_type = get_bits(data[0], 6, 7);//取data[0]的高两位。
    uint16_t gid = get_le_val(setting.para.sid, sizeof(setting.para.sid));
  
    #if MAX_ACTOR_NUM
    uint8_t k, connt_size = 0;
    uint16_t j = 0;
	k = (gid%8) ? (gid%8):8;
    #endif
  
    data++;
    if (group_type == 0) /* bit type */
    {
        gid--;
        if (dlen < get_bits(gid, 3, 7) + 1)
        {
          #if MAX_ACTOR_NUM
            goto deal_find_none;
          #else
            return false;
          #endif
        }
        if (tst_bit(data[get_bits(gid, 3, 7)], gid & 0x07))
        {
          #if SUBSCRIBER_NUM
          for (j = 0;j < (gid>>3);j++)
          {
            connt_size += get_1byte_bit1_number(data[j],8);
          }
          connt_size += get_1byte_bit1_number(data[(gid>>3)],k);
          if (0 == judge_data.find_myself) 
          {
            judge_data.equipment_gid += connt_size; 
          }
          judge_data.find_myself = 1;
         #endif
          return true;
        }
            
    }
    else			    /* bytes type */
    {
        int i;
        int gid_unit_len = group_type == 1 ? 1 : 2;

        for (i = 0; i < dlen; i += gid_unit_len)
        {
            int _gid = get_le_val(data + i, gid_unit_len);
            if ( gid == _gid)
            {
              #if MAX_ACTOR_NUM
              if(0 == judge_data.find_myself)
              {
                judge_data.equipment_gid += connt_size + 1;
              }
              judge_data.find_myself = 1;
              #endif
              return true;
            }
            else if(_gid==0)
            {
              #if MAX_ACTOR_NUM
              judge_data.equipment_gid += gid; 
              judge_data.find_myself = 1;
              #endif
              return true;
            }
            #if MAX_ACTOR_NUM
                connt_size++;
            #endif
                
        }
    }
#if MAX_ACTOR_NUM
deal_find_none:
	if(0 == judge_data.find_myself)
	{
	    if(0x00 == group_type)
	    {
			for (j = 0;j < dlen;j++)
			{
			  #if SUBSCRIBER_NUM
			  judge_data.equipment_gid += get_1byte_bit1_number(data[j],8);
			  #endif
			}
		}
		else if(0x01 == group_type)
		{
			judge_data.equipment_gid += dlen;
		}
		else if(0x02 == group_type)
		{
			judge_data.equipment_gid += dlen/2;
		}
	}
#endif

    return false;
}
//---------------------------------------------------------------------------------------
int do_group_cmd(uint8_t *data, int len)
{
    int inidx = 0;
    uint16_t mysid = get_le_val(setting.para.sid, SID_LEN);
    uint8_t tmp[BUF_LEN];

    int gid_len = get_bits(data[inidx], 0, 5) + 1;

    while (len >= FBD_FRAME_HEAD + gid_len)
    {
        body_t *body = (body_t *)&data[inidx + gid_len];
        int body_len = gid_len + offset_of(body_t, data) + smart_frame_body_len(body);

        if (len < body_len)
            break;

        if (is_gid_equal(&data[inidx], mysid))
        {
            const struct func_ops *op = get_option(get_le_val(body->did, sizeof(body->did)));
            if (op && op->write)
            {
#ifdef PLC_SMART_DEBUG
                log_d("Do cmd[%s]...", op->tip);
#endif
                op->write(body->data, get_bits(body->ctrl, 0, 6), tmp, 128);
            }
        }
        inidx += body_len;
        len -= body_len;

        gid_len = get_bits(data[inidx], 0, 5) + 1;
    }
    return 0;
}
static void check_flag_ask(struct SmartFrame *pframe)
{
    uint8_t i,j,k;
    for(i = 0;i<RESTORE_KEY_NUM;i++)
    {
        for(k = 0; k < MAX_KEY_TYPE; k++)
        {
            if(!dev_state.send_infor[i][k].flag_send) continue;
            
            for(j = 0;j < MAX_DEV_CNT;j++)
            {
#ifdef configUSING_DEBUG
              char str[0x30];
#endif
              log_d("SAID: %s\n", arr2str((void*)&pframe->said, sizeof(setting.encode.id), str, sizeof(str)));
              log_d("TAID: %s\n", arr2str((void*)&switch_param[i].vir_typebody[k].body[dev_state.send_infor[i][k].aid_pos[j]], sizeof(setting.encode.id), str, sizeof(str)));
              log_d("remote_frame_opt j = %d \r\n",j);
              log_d("0check_local_linkctrl seq = %d \r\n",dev_state.send_infor[i][k].seq[j]);
              if((!my_strcmp((void*)&pframe->said,(void*)&switch_param[i].vir_typebody[k].body[dev_state.send_infor[i][k].aid_pos[j]],AID_LEN))\
              &&((dev_state.send_infor[i][k].seq[j] == (pframe->seq&0x7F))))
                {
                    log_d("0check_local_linkctrl flag_send = %d flag_ask = %d\r\n",dev_state.send_infor[i][k].flag_send,dev_state.send_infor[i][k].flag_ask);
                    set_bit(dev_state.send_infor[i][k].flag_ask,j);
                    log_d("1check_local_linkctrl flag_send = %d flag_ask = %d\r\n",dev_state.send_infor[i][k].flag_send,dev_state.send_infor[i][k].flag_ask);
                    return;
                    
                }
            }           
        }
    }
}

int remote_frame_opt(struct SmartFrame *pframe)
{
    int ret = -1;
    struct AppFrame *app = (struct AppFrame *)pframe->data;    
    int len = pframe->len - offset_of(struct AppFrame, data);

	if (smart_frame_is_ack(pframe)) 
    {
        /* */

		if(CMD_RELI_REPORT == app->cmd)
        {
            report_frame_ack(pframe->seq,pframe->said,&pframe->data[1]);
        }
        
        if((0x12 == pframe->data[1])&&(0xc0 == pframe->data[2]))//dev ack frame
        {
            //set_led_status(&pframe->said[0],&pframe->data[4]);// SAID  DATA
        }
        if(pframe->data[0] == CMD_SET)
        { 
            check_flag_ask(pframe);
		}
				
		return ret;
               
    }

#ifdef DEV_SHOW
    if (dev_search_param.dev_show_flag) return 0;
#endif
    switch (app->cmd)
    {
    case CMD_SET:
		#if MAX_ACTOR_NUM
		clear_equipment_gid_flag();
        #endif
        if (smart_frame_is_broadcast(pframe))
		{
            ret = do_group_cmd(app->data, len);
			#if MAX_ACTOR_NUM
            save_subscriber_infor(app->cmd,pframe->said,pframe->taid);
            #endif
			return 0;
		}
        /* fall through */
    case CMD_GET:
		#if MAX_ACTOR_NUM
		if (CMD_SET == app->cmd)
		{
			save_subscriber_infor(app->cmd,pframe->said,pframe->taid);
		}
		#endif
        ret = do_cmd(app->cmd, app->data, len);
        break;
    case CMD_RELI_REPORT:
    case CMD_NRELI_REPORT:
//         set_led_status(&pframe->said[0],&pframe->data[4]);// SID  DATA
         break;
    case CMD_UPDATE:
        ret = update_frame_opt(app->data, len);
        break;
#ifdef DEV_SHOW
		case CMD_SHOW:
		ret = search_frame_opt(pframe);
        break;
#endif
    default:
        ret = 0;
        break;
    }

    if (ret > 0)
    {
        ret += sizeof(struct AppFrame);
    }

    return ret;
}

static base_t get_reg(struct SmartFrame *pframe)
{
    if (reg.type == PASSWORD_REG)
    {
        if (is_all_xx(setting.para.panid, 0x00, sizeof(setting.para.panid)))
        {
            // first time register, do not check password
        }
        else if (memcmp(setting.encode.pwd, &pframe->data[PANID_LEN + 1], PW_LEN))
        {
            reg.last_status = PASSWORD_ERR;
            state_machine_change(STATE_UNLINK);
            return 0;
        }
    }

    reg.type = PASSWORD_REG;
    state_machine_change(STATE_GET_GID);
	
	if (memcmp(setting.para.panid, &pframe->data[1], PANID_LEN))
	{
		memcpy(setting.para.panid, &pframe->data[1], PANID_LEN);
		setting_save();
	}
    return 0;
}
extern void boot_start_update(u8 *data, int len);
void local_frame_opt(struct SmartFrame *pframe)
{
    uint8_t cmd = pframe->data[0];

    switch (cmd)
    {
    case CMD_REGINFOR:
        get_reg(pframe);
        break;
    case CMD_REQ_AID:
        state_machine_change(STATE_SET_AID);
        break;
    case CMD_UNLINK:
        state_machine_change(STATE_SET_REG);
        break;
    case CMD_GET_GWAID:
        memcpy(setting.para.gid, &pframe->data[1], AID_LEN);
        setting_save();
        break;
#ifdef configUSING_UPWAY
		case CMD_REQ_UPDATE:
		boot_start_update(&pframe->data[1], pframe->len - 1);
		break;
#endif
    default:break;
    }
}


int smart_frame_handle(struct SmartFrame *pframe)
{
    int len = 0;

    
    if (!pframe) return 0;

    if (smart_frame_is_local(pframe))
    {
        local_frame_opt(pframe);
    }
    else
    {
        device_ctrl(dev_led, CTRL_OFF_ON, 0);//triggle_led_run
        len = remote_frame_opt(pframe);
        if (len > 0)
        {
            len = code_ret_frame(pframe, len);
        }
    }
    return len;
}
