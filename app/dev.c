#include <config.h>
#include <utils.h>
#include <os.h>
#include <device.h>
#include <printk.h>
#include <settings.h>
#include <syserr.h>
#include "board.h"
#include "plc_smart.h"
#include "encode.h"
#include "dev.h"
#include "dev_show.h"
#include "report\\auto_report_app.h"

static uint8_t dev_type[8] = {0xFF, 0xFF, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00};
static char *soft_ver = "ESACT-4DI6DO(v1.0)-20200929";

static tcb_t app_tcb;
device_t *dev_key, *dev_led, *dev_relay;



void dev_show(void)
{
	static char str[0x28];
	dev_ver_get(str, sizeof(str));
	log_d("VER: %s\n", str, sizeof(str));
    log_d("AID: %s\n", arr2str(setting.encode.id, sizeof(setting.encode.id), str, sizeof(str)));
    log_d("PWD: %s\n", arr2str(setting.encode.pwd, sizeof(setting.encode.pwd), str, sizeof(str)));
    log_d("SN: %s\n",  arr2str(setting.encode.sn, sizeof(setting.encode.sn), str, sizeof(str)));
    log_d("DKey: %s\n",arr2str(setting.encode.dkey, sizeof(setting.encode.dkey), str, sizeof(str)));
    log_d("PID: %s\n", arr2str(setting.para.panid, sizeof(setting.para.panid), str, sizeof(str)));
    log_d("GID: %s\n", arr2str(setting.para.gid, sizeof(setting.para.gid), str, sizeof(str)));
    log_d("SID: %s\n", arr2str(setting.para.sid, sizeof(setting.para.sid), str, sizeof(str)));
}

int dev_type_get(void *out, int maxlen)
{
    memcpy(out, dev_type, sizeof(dev_type));
    return sizeof(dev_type);
}
int dev_type_cmp(const void *dt, size_t len)
{
    return len == sizeof(dev_type) && !memcmp(dt, dev_type, sizeof(dev_type));
}


int dev_ver_get(void *out, int maxlen)
{
	int len = strlen(soft_ver);
	memcpy(out, soft_ver, len);
    return len;
}
int dev_ver_cmp(const void *ver, size_t len)
{
    char softver[0x20];
    int verlen = dev_ver_get(softver, sizeof(softver));
    return len == verlen && !memcmp(ver, softver, verlen);
}


#ifdef LED_CTRL_KEEP
static void led_opt(void)
{
	if(!dev_state.need_act_led)
        return;
    int i = 0;
    for(i = 0; i < RESTORE_LED_NUM; i++)
    {
        if(tst_bit(dev_state.need_act_led,i))
        {
            clr_bit(dev_state.need_act_led,i);
            if(tst_bit(dev_state.state_led,i))
            {
                device_ctrl(dev_led, CTRL_ON, &i);
            }
            else
            {
                device_ctrl(dev_led, CTRL_OFF, &i);
            }
        }
    }
        
}
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 100ms
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void send_broad_frame(uint8_t chn ,uint8_t vir_chn)
{
	device_t *serial_plc= device_find("uart3");
	uint8_t len = 0,taid[ID_LEN] = {0xFF, 0xFF, 0xFF, 0xFF},fbd[0x80];
    //search_broad_self(chn ,vir_chn);
	memcpy(fbd, switch_param[chn].vir_typebody[vir_chn].body , switch_param[chn].vir_typebody[vir_chn].len );
	len = code_frame(setting.encode.id, taid, -1, CMD_SET, fbd, switch_param[chn].vir_typebody[vir_chn].len,\
				g_frame_buf, sizeof(g_frame_buf));
    remote_frame_opt((struct SmartFrame*)g_frame_buf);
	device_write(serial_plc, 0, g_frame_buf, len);
}

void clear_local_linkctrl(uint8_t i ,uint8_t j)
{
	dev_state.send_infor[i][j].local_linkctrl = 0;
	dev_state.send_infor[i][j].retry = 0;
	memset(dev_state.send_infor[i][j].aid_pos,0,MAX_DEV_CNT);
	dev_state.send_infor[i][j].flag_send = 0;
	dev_state.send_infor[i][j].flag_ask = 0;
	dev_state.send_infor[i][j].pos = 0;
}

//static void linkctr_sys_chn_led_on_off(uint8_t chn,uint8_t on_off)
//{
//     if(switch_param[chn].signal_type != NORMALLY_OPEND_SENSOR  && \
//        switch_param[chn].signal_type != NORMALLY_CLOSED_SENSOR && \
//        is_all_xx(dev_state.led_need_sync_chn[chn].aid , 0x00, AID_LEN))
//     led_status_save(1<<chn|on_off, ON_OFF);
//}

static uint8_t find_next_report(uint8_t index ,uint8_t vir_index)
{
	uint8_t i = 0;
	if(dev_state.send_infor[index][vir_index].pos >=MAX_DEV_CNT) 
	{
		i = 0;
	}
	else
	{
		i = ++dev_state.send_infor[index][vir_index].pos;
	}
	
	for(;i <= MAX_DEV_CNT;i++)
	{
		if(i >= MAX_DEV_CNT) 
		{
			if(++dev_state.send_infor[index][vir_index].retry >= setting.dev_infor.try_cnt)
				{
                    //linkctr_sys_chn_led_on_off(index,0);
					clear_local_linkctrl(index,vir_index);
				}
			return(MAX_DEV_CNT);
		}
		if(tst_bit(dev_state.send_infor[index][vir_index].flag_send, i) && ! tst_bit(dev_state.send_infor[index][vir_index].flag_ask, i))
        {
            
            return(i);
        }
	  
	};
	return(MAX_DEV_CNT);
}

static bool deal_self_ctrl(uint8_t index, uint8_t vir_index, uint8_t pos)
{
    uint8_t len;
    key_config_t key_config;
    len = switch_param[index].vir_typebody[vir_index].body[dev_state.send_infor[index][vir_index].aid_pos[pos]+AID_LEN] + SMART_DEBODY_HEAD;
    memcpy(&key_config, &(switch_param[index].vir_typebody[vir_index].body[dev_state.send_infor[index][vir_index].aid_pos[pos]]), len);
    body_t *body = (body_t *) key_config.did;
    
    
    if(!memcmp(setting.encode.id, key_config.aid, AID_LEN))
    {
        len = 0;
        while(key_config.len > len)
        {
            if(body->did[1] == 0xC0)
            {
                if(body->did[0] == 0x12 && body->ctrl == 0x01)
                {
                    log_d("relay_output_time_opt\r\n");
                    relay_status_save(relay_separate_digital_output(body->data[0]), ON_OFF,KEY);
                    led_status_save(led_separate_digital_output(body->data[0]), ON_OFF);
                }
                if(body->did[0] == 0x18 && body->ctrl == 0x01)
                {
                    log_d("relay_output_time_opt4\r\n");
                    relay_status_save(relay_separate_digital_output(body->data[0]), REVERSE,KEY);
                    led_status_save(led_separate_digital_output(body->data[0]), REVERSE);
                    //relay_status_save(body->data[0], REVERSE);
                }
               
            }
            len += SMART_BODY_HEAD + body->ctrl;
            body = (body_t *) &body->data[body->ctrl];
        }
//        set_led_status(setting.encode.id,&dev_state.state_relay);
        set_bit(dev_state.send_infor[index][vir_index].flag_ask,pos);
        local_key_report();
        log_d("deal_self_ctrl\r\n");
        return true;
    }
    return false; 
}

static void send_local_ctrl_frame(uint8_t index,uint8_t vir_index , uint8_t pos)
{
	device_t *serial_plc= device_find("uart3");
    int8_t cmd,len = 0,taid[4],body[128];
    
    if(deal_self_ctrl(index, vir_index, pos)) return;
    
	len = AID_LEN ;
	memcpy(taid, &(switch_param[index].vir_typebody[vir_index].body[dev_state.send_infor[index][vir_index].aid_pos[pos]]), len);  
	cmd =  CMD_SET;
	len = switch_param[index].vir_typebody[vir_index].body[dev_state.send_infor[index][vir_index].aid_pos[pos]+AID_LEN];
    log_d("send_local_ctrl_frame:index =%d vir_index = %d \r\n",index,vir_index);
    log_d("send_local_ctrl_frame:pos =%d aid_pos = %d len = %d\r\n",pos,dev_state.send_infor[index][vir_index].aid_pos[pos],len);
	memcpy(body, &(switch_param[index].vir_typebody[vir_index].body[dev_state.send_infor[index][vir_index].aid_pos[pos]+AID_LEN+CMD_LEN]), len);  
	
	len = code_frame(setting.encode.id,(void*)taid,-1,cmd,(void*)body,\
					len, g_frame_buf, sizeof(g_frame_buf));
	dev_state.send_infor[index][ vir_index].seq[pos] = g_frame_buf[AID_LEN+AID_LEN+STC_LEN ];
	device_write(serial_plc, 0, g_frame_buf, len);
}




static uint16_t check_local_linkctrl(uint8_t idx ,uint8_t vir_idx)
{
    de_body_t *de_body = (de_body_t *)switch_param[idx].vir_typebody[vir_idx].body;
	uint16_t flag = 0;
	uint8_t len = 0,i = 0;

	while(switch_param[idx].vir_typebody[vir_idx].len > len)
	{
        log_d("check_local_linkctrl i=%d,len = %d\r\n",i,len);
		dev_state.send_infor[idx][vir_idx].aid_pos[i] = len;
        if(is_all_xx(&switch_param[idx].vir_typebody[vir_idx].body[dev_state.send_infor[idx][vir_idx].aid_pos[i]],0xFF,AID_LEN))
            return 0;
        if(is_all_xx(&switch_param[idx].vir_typebody[vir_idx].body[dev_state.send_infor[idx][vir_idx].aid_pos[i]],0x00,AID_LEN))
            return 0;
		flag += 1<<i; 
		i++;
        len += SMART_DEBODY_HEAD + de_body->len;
        de_body = (de_body_t *) &switch_param[idx].vir_typebody[vir_idx].body[len];
        if(i > MAX_DEV_CNT) return 0;
	}
	//dev_state.send_infor[idx][vir_idx].aid_pos[i] = len;
    log_d("check_local_linkctrl i = %d\r\n",i);
    log_d("check_local_linkctrl flag= %d\r\n",flag);
	return flag;
}


static uint8_t send_local_linkctrl(uint8_t idx,uint8_t vir_idx)
{
    log_d("send_local_linkctrl 0 idx = %d vir_idx = %d\r\n",idx,vir_idx);
    if(!switch_param[idx].vir_typebody[vir_idx].len)  return 0;
        
	if(!dev_state.send_infor[idx][vir_idx].flag_send )
	{
		dev_state.send_infor[idx][vir_idx].flag_send = check_local_linkctrl(idx,vir_idx);
	}
	else
	{
		if(dev_state.send_infor[idx][vir_idx].flag_send == dev_state.send_infor[idx][vir_idx].flag_ask \
           || dev_state.send_infor[idx][vir_idx].retry >= setting.dev_infor.try_cnt )
		{
			clear_local_linkctrl(idx,vir_idx);
            //linkctr_sys_chn_led_on_off(idx,0);
		}
		else
		{
			//send the para
			if(dev_state.send_infor[idx][vir_idx].pos < MAX_DEV_CNT)
			{
                log_d("send_local_linkctrl 1 idx = %d vir_idx = %d\r\n",idx,vir_idx);
				send_local_ctrl_frame(idx,vir_idx,dev_state.send_infor[idx][vir_idx].pos);
			}
			dev_state.send_infor[idx][vir_idx].pos = find_next_report(idx,vir_idx);
		}
	}
	return(0);
}







#define DELAY_REPORT 5

static void local_linker_report_hook(void)
{
	uint8_t i = 0, j = 0;
    

    //------------ interval time local linker ctrl------------
    if( ++dev_state.send_interval_count  < setting.dev_infor.interval)  
    return;
    dev_state.send_interval_count = 0;

    for( i = 0 ; i < RESTORE_KEY_NUM; i++)
    {  
        for(j = 0; j < MAX_KEY_TYPE; j++)
        {
            if(dev_state.send_infor[i][j].local_linkctrl)
            {	
                log_d("local_linker_report_hook  i = %d j = %d local_linkctrl = %d\r\n",i,j,dev_state.send_infor[i][j].local_linkctrl);                
                send_local_linkctrl(i,j);
            }
            else
            {
                clear_local_linkctrl(i,j);
            } 
        }
    }
}
static int8_t get_chn_from_bit(uint8_t chn)
{
	uint8_t i=0;
	for(i=0; i<8; i++)
	{
		if(tst_bit(chn, i))
			return i;
	}
	return -1;
}
static void relay_to_normal(uint8_t chn)
{
	dev_state.relay_to_normal |= chn;
	if(get_chn_from_bit(chn) >= 0)
		dev_state.relay_to_normal_cnt[get_chn_from_bit(chn)] = 10; //10*10ms 100ms keep level
}
static void relay_mutex_protect(uint8_t act_chn)
{
    uint8_t chn_mutex = 0;
    if(!setting.dev_infor.flag_relay_ctrl_mutex)
        return;
    chn_mutex = (act_chn == 1)?2:1;
    
    dev_state.output_time[chn_mutex-1] = 0;
    if(dev_state.state_relay & chn_mutex)
    {  
        relay_status_save(chn_mutex, ON_OFF,AUTO);
        //local_key_report();
    }
}

uint8_t relay_status_save(uint8_t state, uint8_t type,uint8_t source) //type(01--set(C012)  00--reverse(C018))
{
	int i;
	OS_CPU_SR cpu_sr;

	enter_critical();
	for (i = 0; i < RESTORE_RELAY_NUM; ++i)
	{
        
        if(tst_bit(state, i) && dev_state.output_time[i] && (source != AUTO) && (setting.dev_infor.relay_para[i].out_mode == RELAY_OUT_PULSE))
        {
            dev_state.output_time[i] = 1;            
            continue;
        }
        
        
		if (tst_bit(state, i))
		{

			switch(type)
			{
				case ON_OFF:
				{
					if (tst_bit(state, 7))
						set_bit(dev_state.state_relay, i);
					else
						clr_bit(dev_state.state_relay, i);
				}
				break;
				case REVERSE:
					dev_state.state_relay ^= (uint8_t)(1 << i);
				break;
				default:
					break;
			}
		}
	}
    log_d("dev_state.state_relay=%d \r\n",dev_state.state_relay);
    log_d("dev_state.state_relay_bak=%d \r\n",dev_state.state_relay_bak);
	if (dev_state.state_relay != dev_state.state_relay_bak)
	{
        dev_state.need_act_relay |= (uint8_t)(dev_state.state_relay ^ dev_state.state_relay_bak);
		dev_state.state_relay_bak = dev_state.state_relay;
        setting.dev_infor.state_digtal_output &= 0x0F;
		setting.dev_infor.state_digtal_output |= dev_state.state_relay<<4;        
        dev_state.param_save = 1;
	}
	exit_critical();
//	if(dev_state.need_act_relay)
//		device_ctrl(dev_zcp, ZCP_CTRL_OPEN, NULL);
//    log_d("relay_status_save state_relay=%d setting.dev_infor.state_relay=%d\r\n",dev_state.state_relay ,setting.dev_infor.state_relay);
	
//-------------------------------------------------------------------------------------	
    log_d("dev_state.need_act=%d \r\n",dev_state.need_act);
	if(dev_state.need_act_relay)   
	{
		int i = 0,chn = 0;
		for(i = 0; i < RESTORE_RELAY_NUM ; i++)
		{
			chn = 0;
			if(tst_bit(dev_state.need_act_relay,i))
			{
				set_bit(chn,i);
                
				if(tst_bit(dev_state.state_relay,i))
				{
                    relay_mutex_protect(1<<i);
					device_ctrl(dev_relay, CTRL_RELAY_ON, &chn);
                    dev_state.output_time[i] = setting.dev_infor.relay_para[i].output_time*10;
				}
				else
				{
					device_ctrl(dev_relay, CTRL_RELAY_OFF, &chn);
                    //device_ctrl(dev_led, CTRL_OFF, &chn);
				}
                relay_to_normal(chn);
				clr_bit(dev_state.need_act_relay,i);
			}
		}
	}
//-------------------------------------------------------------------------------------	
	return (dev_state.state_relay);
}

	
uint8_t led_status_save(uint8_t state, uint8_t type) //type(01--set(C012)  00--reverse(C018))
{
	int i;
	OS_CPU_SR cpu_sr;
	enter_critical();

	for (i = 0; i < RESTORE_KEY_NUM; ++i)
	{
		if (tst_bit(state, i))
		{
			switch(type)
			{
            case ON_OFF:
            {
                if (tst_bit(state, 7))
                    set_bit(dev_state.state_led, i);
                else
                    clr_bit(dev_state.state_led, i);
            }
            break;
            case REVERSE:
                dev_state.state_led ^= (uint8_t)(1 << i);
            break;
            default:
            break;
			}
		}
	}
	if (dev_state.state_led != dev_state.state_led_bak)
	{
        dev_state.need_act_led |= (uint8_t)(dev_state.state_led ^ dev_state.state_led_bak);
		dev_state.state_led_bak = dev_state.state_led;
        setting.dev_infor.state_digtal_output &= 0xF0;
        setting.dev_infor.state_digtal_output |= dev_state.state_led;        
        dev_state.param_save = 1;
    }
    exit_critical();
    return (dev_state.state_led);
}



// key
static void triggle_key_ctrl(uint8_t idx, uint8_t vir_idx)
{
    vir_idx = vir_idx -1;

    if(switch_param[idx].vir_typebody[vir_idx].send_type == KEY_REPORT && (switch_param[idx].signal_type < NORMALLY_OPEND_SENSOR))
    {
        if(switch_param[idx].signal_type == SELF_LOCK_KEY) vir_idx =0;//key_down key_up no diffience
        start_key_signal_report(idx*3+vir_idx);
        dev_state.flag_key_report |= 1 << idx;
        log_d("KEY_REPORT\r\n");
        return;
    }
    
    if(!switch_param[idx].vir_typebody[vir_idx].len || (switch_param[idx].vir_typebody[vir_idx].send_type > KEY_REPORT)) 
    {
        log_d("len = 0\r\n");
        return;
    }
    if(switch_param[idx].vir_typebody[vir_idx].send_type == BROAD_CAST)
    {
        if(switch_param[idx].signal_type != NORMALLY_OPEND_SENSOR && switch_param[idx].signal_type != NORMALLY_CLOSED_SENSOR)
        dev_state.flag_broadcast |= 1 << idx;
        send_broad_frame(idx ,vir_idx);
        return;
    }
        
    clear_local_linkctrl(idx,vir_idx);
        
    dev_state.send_infor[idx][vir_idx].local_linkctrl = 1;
    log_d("triggle_key_ctrl idx=%d,vir_idx=%d\r\n",idx,vir_idx);
    dev_state.send_interval_count = setting.dev_infor.interval;

    //linkctr_sys_chn_led_on_off(idx,0x80);
}

static void on_key_short_pressed(uint8_t idx)
{
    if(setting.dev_infor.lock) return;
    
    if(switch_param[idx].signal_type != SELF_RESET_KEY ) return;
    
    if(!switch_param[idx].vir_typebody[DOUBLE_PRESS-1].len && switch_param[idx].vir_typebody[SHORT_PRESS-1].send_type !=KEY_REPORT) 
    {
        triggle_key_ctrl(idx, SHORT_PRESS);
    }
}

static void on_key_single_pressed(uint8_t idx)
{
    if(setting.dev_infor.lock) return;
    
    if(switch_param[idx].signal_type != SELF_RESET_KEY ) return;
    
    if(switch_param[idx].vir_typebody[DOUBLE_PRESS-1].len || switch_param[idx].vir_typebody[SHORT_PRESS-1].send_type ==KEY_REPORT) 
    {
        triggle_key_ctrl(idx, SHORT_PRESS);
    }
}

static void on_key_long_press(uint8_t idx)
{
    if(setting.dev_infor.lock) return;
    
    if(switch_param[idx].signal_type != SELF_RESET_KEY ) return;
    
    triggle_key_ctrl(idx, LONG_PRESS);
}

static void on_key_double_pressed(uint8_t idx)
{
    if(setting.dev_infor.lock) return;
    
    if(switch_param[idx].signal_type != SELF_RESET_KEY ) return;
    
    triggle_key_ctrl(idx, DOUBLE_PRESS);
}

static void on_key_up(uint8_t idx)
{
    if(setting.dev_infor.lock) return;
    
    if(switch_param[idx].signal_type == INVALID_SIG || switch_param[idx].signal_type == SELF_RESET_KEY) return;
    
    dev_state.flag_key_state &= ~(1<<idx);
    
    triggle_key_ctrl(idx, KEY_UP);
    
    if(switch_param[idx].signal_type == NORMALLY_OPEND_SENSOR)
    {
       dev_state.ex_sensor_state &= ~(1<<idx) ;
    }
    else if(switch_param[idx].signal_type == NORMALLY_CLOSED_SENSOR)
    {
       dev_state.ex_sensor_state |= 1<<idx ;
    }
}
static void on_key_down(uint8_t idx)
{
    if(setting.dev_infor.lock) return;
    
    if(switch_param[idx].signal_type == INVALID_SIG || switch_param[idx].signal_type == SELF_RESET_KEY) return;
    
    dev_state.flag_key_state |= 1<<idx;
    
    triggle_key_ctrl(idx, KEY_DOWN);
    if(switch_param[idx].signal_type == NORMALLY_OPEND_SENSOR)
    {
        dev_state.ex_sensor_state |= 1<<idx ;
    }
    else if(switch_param[idx].signal_type == NORMALLY_CLOSED_SENSOR)
    {
        dev_state.ex_sensor_state &= ~(1<<idx) ;
    }
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void relay_normal_opt(void)
{
	uint8_t i = 0, chn = 0;
	for(i = 0; i < RESTORE_KEY_NUM  ; i++)
	{
		chn = 0;
		if(tst_bit(dev_state.relay_to_normal, i))
		{
			if(dev_state.relay_to_normal_cnt[i]-- <= 0)
			{
				clr_bit(dev_state.relay_to_normal, i);
				set_bit(chn, i);
				device_ctrl(dev_relay, CTRL_RELAY_NORMAL, &chn);
                if(setting.dev_infor.relay_para[i].out_mode == RELAY_OUT_PULSE && tst_bit(dev_state.state_relay, i))
                {
                    log_d("relay_output_time_opt3\r\n");
                    relay_status_save(1<<i, ON_OFF,AUTO);  
                    local_key_report();
                }
			}
		}
	}
}

static void on_10ms_tmr_cb(struct soft_timer *st)
{
	st->expires += configHZ/100;
    soft_timer_add(st);
	local_linker_report_hook();
    relay_normal_opt();
}

static int _10msec_tmr_init(void)
{
    static struct soft_timer st =
    {
        .cb      = on_10ms_tmr_cb,
    };
	st.expires = jiffies + configHZ/100;
    soft_timer_add(&st);

    return 0;
}




//#define RELAY_ZCP_CNT 			2   //CNT*100ms
//static void relay_zcp_protect_opt(void)
//{
//	uint8_t i = 0, chn = 0;
//	static uint8_t relay_zcp_cnt[3] = {0x00, 0x00 , 0x00};
//	for(i = 0; i < RESTORE_RELAY_NUM  ; i++)
//	{
//		chn = 0;
//		if(tst_bit(dev_state.need_act_relay, i))
//		{
//            log_d("relay_zcp_protect_opt%d\r\n",dev_state.need_act_relay);
//			if(++relay_zcp_cnt[i] > RELAY_ZCP_CNT)
//			{
//				relay_zcp_cnt[i] = 0;
//				set_bit(chn, i);
//				device_ctrl(dev_zcp, ZCP_CTRL_CLOSE, NULL);
//			}
//		}
//		else
//		{
//			relay_zcp_cnt[i] = 0;
//		}
//	}
//}

static void set_led_keyreport(void)
{
	uint8_t i;
	if(dev_state.flag_key_report)
	{
		for(i = 0;i < RESTORE_KEY_NUM ;i++)
		{
			if(tst_bit(dev_state.flag_key_report,i))
			{
				dev_state.time_keyreport[i]++;
			}
			if(dev_state.time_keyreport[i] == TRIGGER_KEYREPORT_LED)
			{
				clr_bit(dev_state.flag_key_report, i);
				dev_state.time_keyreport[i] = 0;
			}
		}
	}
}
static void relay_output_time_opt(void)
{
    uint8_t i = 0;
    for(i = 0; i< RESTORE_RELAY_NUM;i++)
    {
        if(!dev_state.output_time[i])
            continue;
        dev_state.output_time[i]--;
        
        if(!dev_state.output_time[i])
        {
            if(setting.dev_infor.relay_para[i].out_mode == RELAY_OUT_LEVEL)
            {
                log_d("relay_output_time_opt1\r\n");
                relay_status_save(1<<i, ON_OFF,AUTO);
            }
            else if(setting.dev_infor.relay_para[i].out_mode == RELAY_OUT_PULSE)
            {
                log_d("relay_output_time_opt2\r\n");
                relay_status_save(1<<i|0x80, ON_OFF,AUTO);
                dev_state.output_time[i] = 0;
            }
            local_key_report();
        }
    }
}
static void on_100ms_tmr_cb(struct soft_timer *st)
{
	st->expires += configHZ/10;
    soft_timer_add(st);
	
    relay_output_time_opt();
    //relay_normal_opt();
    auto_report_100msec_task();
    set_led_keyreport();
#ifdef TRIAC_CTRL_KEEP
//	triac_opt();
#endif
	
#ifdef LED_CTRL_KEEP
	led_opt();
#endif
}

static int _100msec_tmr_init(void)
{
    static struct soft_timer st =
    {
        .cb      = on_100ms_tmr_cb,
    };
	st.expires = jiffies + configHZ/10;
    soft_timer_add(&st);

    return 0;
}

// sec
//static void set_led_broadcast(void)
//{
//	uint8_t i;
//	if(dev_state.flag_broadcast)
//	{
//		for(i = 0;i < RESTORE_KEY_NUM ;i++)
//		{
//			if(tst_bit(dev_state.flag_broadcast,i))
//			{
//				//led_status_save(dev_state.flag_broadcast|0x80, ON_OFF);
//				dev_state.time_broadcast[i]++;
//			}
//			if(dev_state.time_broadcast[i] == TRIGGER_BROADCAST_LED)
//			{
//				//led_status_save(1<<i, ON_OFF);
//				clr_bit(dev_state.flag_broadcast, i);
//				dev_state.time_broadcast[i] = 0;
//			}
//		}
//	}
//}


static void flash_param_save(void)
{
	if(dev_state.param_save)
	{
		dev_state.param_save = 0;
		setting_save();
	}
}
static void sec_tmr_cb(struct soft_timer *st)
{
    st->expires += configHZ;
    soft_timer_add(st);
    
	//set_led_broadcast();
    
    flash_param_save();
    
    if(dev_state.flag_para_report) dev_state.flag_para_report--;
	
#ifdef DEV_SHOW
	check_search_delay();
#endif
}

static int sec_tmr_init(void)
{
    static struct soft_timer st =
    {
        .cb      = sec_tmr_cb,
    };
	st.expires = jiffies + configHZ;
    soft_timer_add(&st);
    return 0;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// dev show timer, 1s
static void dev_show_timer_cb(struct soft_timer *st)
{
	static uint8_t cnt = 0;
	cnt++;
	
	if(cnt <= 6)
	{
		//dev_show_display_start();
        relay_status_save(01 , REVERSE ,AUTO);
        st->expires = jiffies + configHZ ;//1S
        soft_timer_add(st);
	}
	else
	{
		cnt = 0;
		dev_search_param.dev_show_flag = 0x00;
	}
    
}
void dev_show_start(void)
{
    
    static struct soft_timer dev_show_timer = 
    {
        .cb = dev_show_timer_cb,
    };

    dev_show_timer.expires = jiffies + configHZ ;//1s
    soft_timer_add(&dev_show_timer);
    
}


static void setup_key(void)
{
    dev_key = device_find("key");
    assert(dev_key);
    device_open(dev_key, DEVICE_FLAG_FASYNC);
}

static void do_key(void)
{
    key_event_t key_event;
    device_ctrl(dev_key, KEY_CTRL_GET_KEY, &key_event);
#ifdef DEV_SHOW
    if (dev_search_param.dev_show_flag)
        return;
#endif

	int i;
	for (i = 0; i < KEY_POOL_MAX; i++)
	{
		if (key_event.pressed & (1 << i))
		{
			log_d("Key %d short pressed!\r\n", i + 1);
			on_key_short_pressed(i);
		}
        
        if (key_event.lpressed & (1 << i))
		{
			log_d("Key %d long pressed!\r\n", i + 1);
			on_key_long_press(i);
		}
        
        if (key_event.keyup & (1 << i))
		{
			log_d("Key %d up!\r\n", i + 1);
			on_key_up(i);
		}
        
        if (key_event.keydown & (1 << i))
		{
			log_d("Key %d down!\r\n", i + 1);
			on_key_down(i);
		}
        
        if (key_event.double_pressed & (1 << i))
		{
			log_d("Key %d double pressed!\r\n", i + 1);
			on_key_double_pressed(i);
		}
         if (key_event.single_pressed & (1 << i))
		{
			log_d("Key %d single pressed!\r\n", i + 1);
			on_key_single_pressed(i);
		}
	}
}


//led

static void setup_led(void)
{
    dev_led = device_find("led");
    assert(dev_led);
    device_open(dev_led, 0);
}

//static void setup_zcp(void)
//{
//    dev_zcp = device_find("zcp");
//    assert(dev_zcp);
//    device_open(dev_zcp, DEVICE_FLAG_FASYNC);
//}



static void setup_relay(void)
{
    dev_relay = device_find("relay");
    assert(dev_relay);
	device_open(dev_relay, 0);
}

void refresh_current_state(void)
{
    relay_status_save(relay_separate_digital_output(setting.dev_infor.state_digtal_output|0x80), ON_OFF,AUTO);
    led_status_save(led_separate_digital_output(setting.dev_infor.state_digtal_output|0x80), ON_OFF);
    dev_state.state_relay_bak  = 0x03;
    relay_status_save(relay_separate_digital_output(~setting.dev_infor.state_digtal_output & 0x3F), ON_OFF,AUTO);
    led_status_save(led_separate_digital_output(~setting.dev_infor.state_digtal_output & 0x3F), ON_OFF);
}
static void setup_power_on(void)
{
    switch(setting.dev_infor.relay_poweron_mode)
    {
        case 0: setting.dev_infor.state_digtal_output = 0x00;dev_state.state_relay_bak  = 0x03;setting_save();relay_status_save(0x03, ON_OFF,AUTO); 
                dev_state.state_led_bak = 0x0F;led_status_save(0x0F, ON_OFF);
        break;
        case 1: setting.dev_infor.state_digtal_output = 0x3F;setting_save();relay_status_save(0x83, ON_OFF,AUTO); 
                led_status_save(0x8F, ON_OFF);
        break;
        case 2: refresh_current_state();
        break;
        default:break;
    }
    local_key_report();
}



static void app_task_cb(struct task_ctrl_blk *tcb, ubase_t data)
{
    tSTART(tcb);

    setup_key();
    setup_led();
    //setup_zcp();
    setup_relay();
    setup_power_on();
    

    for (;;)
    {
        task_wait_signal(tcb);

        sig_t sig = task_signal(tcb);

        if (sigget(sig, SIG_KEY))
            do_key();
    }

    tEND();
}


void setup_app(void)
{
    setting_load();
    device_encode_opt();
	_100msec_tmr_init();
	_10msec_tmr_init();
	sec_tmr_init();
	dev_show();
	plc_init();
	task_create(&app_tcb, app_task_cb, 0);
	auto_report_init(600, setting.para.gid,setting.encode.id,setting.para.sid, &setting.dev_infor.report);
}
