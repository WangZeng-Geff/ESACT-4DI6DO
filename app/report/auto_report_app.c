#include <stdlib.h>
#include <config.h>
#include <utils.h>
#include "proto_smart.h"
#include "plc_smart.h"
#include "auto_report_app.h"
#include "device.h"
#include "dev.h"
#include "settings.h"
#include "printk.h"

/* please redefine send function */
int uart_write_report(const void *in,int len)
{
	device_t *serial_plc = device_find("uart3");
    log_d("auto report send report frame!\n");
	return(device_write(serial_plc, 0, in, len));
}

/* some sensor need report type infor, if you don't use it ,please delete */
int code_intype_body(uint16_t did, int err, const void *data, int len, void *out, int maxlen ,uint8_t type)
{
    int ret = 0;
    uint8_t buf[0x10];
    
    buf[0] = type;
    memcpy(&buf[1], data, len);

    ret = code_body(did, 0, buf, len + 1, out, maxlen);
    return ret;
}

//--------------------------------------------------------------------------------------
//------------------------------ init actor data ---------------------------------------
//--------------------------------------------------------------------------------------
//开关机+模式+风速+面板锁定+低温保护
//
#if MAX_ACTOR_NUM
void init_relay_data(void)
{
	uint8_t i = 0;
    relay_data_bak[i] = setting.dev_infor.state_digtal_output;//val     
    relay_data[i++].cur = &setting.dev_infor.state_digtal_output;//pointer
}
#endif

#if EX_SENSOR_NUM
void init_ex_sensor_data(void)
{
	uint8_t i = 0;
    ex_sensor_data_back[i] = dev_state.ex_sensor_state;//val     
    ex_sensor_data[i++].cur = &dev_state.ex_sensor_state;//pointer
}
#endif
//--------------------------------------------------------------------------------------
//------------------------------ init alarm data ---------------------------------------
//--------------------------------------------------------------------------------------
#if MAX_ALARM_NUM
extern uint8_t over_temp_flag;//低温保护动作标志。
//extern struct FLASH_STRUCT flash_struct;
void init_alarm_data(void)
{
	uint8_t i = 0;
    alarm_data_bak[i] = dev_state.temp_over.flag;//val 低温保护动作标志。
    alarm_data[i++].cur = &dev_state.temp_over.flag;//pointer
    //alarm_data_bak[i] = flash_struct.breakdown_start_flag;//val  温湿度传感器故障标志
    //alarm_data[i++].cur = &flash_struct.breakdown_start_flag;//pointer
}
#endif
  
//--------------------------------------------------------------------------------------
//------------------------------ init sensor data --------------------------------------
//--------------------------------------------------------------------------------------
#if MAX_SENSOR_NUM
void init_sensor_data(void)//初始化传感器上报参数
{
    uint8_t i = 0;
    sensor_data[i].sensor_window = TEMP_WINDOW_BIN;
    sensor_data[i].sensor_cur = (int32_t)dev_state.temp_over.temp;
    sensor_data[i].sensor_freq = dev_state.temp_report_freq;
    sensor_data[i].sensor_step = dev_state.temp_report_step;
    sensor_data[i++].sensor_type = TEMP;
    
    sensor_data[i].sensor_window =HUMI_WINDOW_BIN;
    sensor_data[i].sensor_cur = (int32_t)dev_state.humidity;
    sensor_data[i].sensor_freq = dev_state.humi_report_freq;
    sensor_data[i].sensor_step = dev_state.humi_report_step;
    sensor_data[i++].sensor_type = HUMI; 
}
#endif

//--------------------------------------------------------------------------------------
//------------------------- code report data and put in chn ----------------------------
//--------------------------------------------------------------------------------------
static uint8_t default_key_para[128];
static uint8_t key_default_para(uint8_t chn, uint8_t vir_chn)
{
    //switch_param[chn].chn_id = chn;
    switch_param[chn].virtual_chn_id = vir_chn;
    vir_chn = vir_chn - 1;
    memcpy(default_key_para, &switch_param[chn].action,SWITCH_PARAM_HEAD);
    memcpy(&default_key_para[SWITCH_PARAM_HEAD], &switch_param[chn].vir_typebody[vir_chn],VIRTUAL_CHN_HEAD);
    memcpy(&default_key_para[SWITCH_PARAM_HEAD + VIRTUAL_CHN_HEAD], switch_param[chn].vir_typebody[vir_chn].body,switch_param[chn].vir_typebody[vir_chn].len);
    return(SWITCH_PARAM_HEAD + VIRTUAL_CHN_HEAD + switch_param[chn].vir_typebody[vir_chn].len);     
}

//static void set_para_reset()
//{
//    uint8_t chn;
//    dev_restore_factory();
//    for(chn = 0; chn < RESTORE_KEY_NUM;chn++)
//    {
//        switch_param_default(chn);
//    }

//}
void get_report_data_callback(report_type_t r_type)
{
    uint8_t len = 0,i = 0;
    clear_chn(r_type);
#if MAX_SENSOR_NUM
    uint8_t buf[2] = {0x00,0x00};
    if((POWER_ON == r_type) || (REGISTER == r_type) || (FREQUENCY == r_type) || (FIXED_LENGTH == r_type))
    {
        
        hex2bcd(abs(dev_state.temp_over.temp), buf, TEMP_LEN);
		len += code_intype_body(0xB701,0,buf,TEMP_LEN,g_frame_buf+len,sizeof(g_frame_buf), TEMP);
		hex2bcd(dev_state.humidity, buf, HUMI_LEN);
		len += code_intype_body(0xB701,0,buf,HUMI_LEN,g_frame_buf+len,sizeof(g_frame_buf),HUMI);
    }
#endif
	
#if MAX_ALARM_NUM
    if(ALARM == r_type)
    {
		len+=code_body(0xC0A0,0, &dev_state.temp_over.flag,1,g_frame_buf+len,sizeof(g_frame_buf));
    }
#endif
	
#if MAX_ACTOR_NUM
    if((POWER_ON == r_type) || (STATE_CHANGE == r_type))
    {
        log_d("dev_state.flag_para_reset_reg_step = 0\n");
        len+=code_body(0xC012,0,&setting.dev_infor.state_digtal_output,1,g_frame_buf+len,sizeof(g_frame_buf));
        if(POWER_ON == r_type)
        {
            memcpy(judge_data.taker_id,setting.encode.id,ID_LEN);
        }
        len += code_body(0xC01A,0,judge_data.taker_id,ID_LEN,g_frame_buf+len,sizeof(g_frame_buf));
    }
        
    if(REGISTER == r_type)
    {
        len+=code_body(0xC012,0,&setting.dev_infor.state_digtal_output,1,g_frame_buf+len,sizeof(g_frame_buf));
        memcpy(judge_data.taker_id,setting.encode.id,ID_LEN);
        len += code_body(0xC01A,0,judge_data.taker_id,ID_LEN,g_frame_buf+len,sizeof(g_frame_buf));

        if(!dev_state.flag_para_report)
        {      
            len = put_chn(r_type,g_frame_buf,len);
            return;
        }
        dev_state.flag_para_report = 0;
              
        log_d("set_para_reset\n");
        for(i = 0; i< RESTORE_RELAY_NUM*2; i++)
        {
            key_default_para(i/2, i%2+1);
            len += code_body(0xFC19,0,default_key_para,key_default_para(i/2, i%2+1),g_frame_buf+len,sizeof(g_frame_buf)); 
        }     
    }
#endif

#if EX_SENSOR_NUM
    if(EX_SENSOR_STATE_CHANGE == r_type)
    {
        uint8_t i,len2;
        uint8_t data[20];
        for(i = 0; i < RESTORE_KEY_NUM; i++)
        {
            if(tst_bit(dev_state.ex_sensor_state,i) != tst_bit(dev_state.ex_sensor_state_bak,i))
            {
                data[0] = (switch_param[i].signal_type == NORMALLY_OPEND_SENSOR)? 0x01:0x81;	
                data[1] = i + 1;
                memcpy(&data[2], switch_param[i].name, switch_param[i].name_len); 
                data[2 + switch_param[i].name_len] = 0x00;
                data[3 + switch_param[i].name_len] = (tst_bit(dev_state.ex_sensor_state,i)==0)?0:1;
                len2 = 4 + switch_param[i].name_len;
                len+=code_body(0xC062, 0, data, len2, g_frame_buf+len, sizeof(g_frame_buf));
            }  
        }
		dev_state.ex_sensor_state_bak = dev_state.ex_sensor_state;
        log_d("send_ex_sensor_data!!\n");
    }
#endif
    
#if KEY_SIGNAL_ENABLE
    if(KEY_CHN1_SINGLE <= r_type && KEY_CHN4_LONG>=r_type)
    {
        uint8_t len2;
        uint8_t data[20];
 
        data[0] = 0x01;	
        data[1] = (r_type-KEY_CHN1_SINGLE)/3 + 1;
        memcpy(&data[2], switch_param[data[1]-1].name, switch_param[data[1]-1].name_len); 
        data[2 + switch_param[data[1]-1].name_len] = 0x00;
        
        if(switch_param[data[1]-1].signal_type > SELF_RESET_KEY) return;
        
        if(switch_param[data[1]-1].signal_type == SELF_RESET_KEY)
        data[3 + switch_param[data[1]-1].name_len] = (r_type-KEY_CHN1_SINGLE)%3 + 1;
        else if(switch_param[data[1]-1].signal_type == SELF_LOCK_KEY)
        data[3 + switch_param[data[1]-1].name_len] = (tst_bit(dev_state.flag_key_state,data[1]-1)==0)?0:1;
        
        len2 = 4 + switch_param[data[1]-1].name_len;
        len+=code_body(0xC062, 0, data, len2, g_frame_buf+len, sizeof(g_frame_buf));

		
        log_d("sen_ex_sensor_data!!\n");
    }
#endif
    

	len = put_chn(r_type,g_frame_buf,len);
    
    log_d("Report type: %d,put data len:%d\n",r_type,len);
}


//--------------------------------------------------------------------------------------
#if MAX_SENSOR_NUM
static uint8_t get_sensor_type(struct Body* fbd)
{
    /* B701 ACK, get sensor type */
    if ((fbd->did[0] == 0x01) && (fbd->did[1] == 0xB7))
    {
        return(fbd->data[0]);
    }
	/* please add you did and sensor type */
    return UNKNOWN;
}
#endif


//--------------------------------------------------------------------------------------
//-------------------- receive gateway ack ,refresh sensor baseline --------------------
//--------------------------------------------------------------------------------------
void report_finish_refresh_infor(uint8_t *data, uint8_t len)
{
    #if MAX_SENSOR_NUM
    uint8_t i,type;
    int8_t j = -1;
    struct Body *body;
    for (i = 0; i < len; i += FBD_FRAME_HEAD)
    {
        body = (struct Body*)&data[i];
        type = get_sensor_type(body); 
        switch (type)
        {
            
            case TEMP:
                j = get_sensor_from_type(TEMP);
                if (j >= 0)
                {
                    sensor_data[j].sensor_base = (int32_t)bcd2hex(&body->data[1],TEMP_LEN);
                    sensor_data[j].last_report = sensor_data[j].sensor_base;
                }
                break;
            case HUMI:
                j = get_sensor_from_type(HUMI);
                if (j >= 0)
                {
                   sensor_data[j].sensor_base = (int32_t)bcd2hex(&body->data[1],HUMI_LEN);
                   sensor_data[j].last_report = sensor_data[j].sensor_base;
                }
                
                break;
            default:
                break;
        }
        i += body->ctrl;
    }
    #endif
}

