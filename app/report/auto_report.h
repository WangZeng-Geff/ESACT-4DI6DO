#ifndef _RREPORT_H_
#define _RREPORT_H_
#include <stdint.h>
#include <string.h>
//---------------------------------------------------------------------------------------------------------

#define TYPE_NUM				     	        15 /* support sensor or alarm or actor type number,please modify depend on your system */
#define SUBSCRIBER_NUM                          3 /* please modify depend on your system */
#define KEY_SIGNAL_ENABLE                       1

#define MAX_ACTOR_NUM				            1 /* please modify depend on your system */
#if MAX_ACTOR_NUM
#define SOFT_ACTOR                              0 /* soft actor or not */
#define TAKER_DATA                              7 /* please don't modify this */
#endif

#define EX_SENSOR_NUM				            1 /* please modify depend on your system */
#if EX_SENSOR_NUM	
#define SOFT_EX_SENSOR_NUM                      0 /* soft ex sensor or not */
#endif

#define MAX_ALARM_NUM                           1 /* please modify depend on your system */
#if MAX_ALARM_NUM
#define ALARM_REPORT_SB                         0 /* alarm data report subscriber or not */
#endif

#define MAX_SENSOR_NUM                          0 /* please modify depend on your system */
#if MAX_SENSOR_NUM                          
#define SENSOR_REPORT_SB                        0  /* sensor data report subscriber or not */
#define  DIGITAL_WIN                            0  /* digital sensor window val */
#endif

#define MAX_TRY_CNT                             2
#define MAX_SUBSCRIBER_NUM                      (TYPE_NUM + SUBSCRIBER_NUM)
#define CHANNEL_COMMUNICATE_ABILITY             20/*assess the channel communicate ability,for example 20 means trans waits 2seconds for the ack*/
#define FIRST_RETRY_INTERVAL                    100//0.1s
#define SECOND_RETRY_INTERVAL                   1000//0.1s
#define POWER_ON_COMPARE_DATA                   100  /* the data maybe diffrerent */

//---------------------------------------------------------------------------------------------------------
#ifndef STC
#define STC   0x7E
#endif

#ifndef ID_LEN
#define ID_LEN   0x04
#endif

#ifndef DID_LEN
#define DID_LEN 0X02
#endif

#ifndef NULL
#define NULL  0
#endif
typedef enum _data_type_t
{
	UNKNOWN = 0,
	VOLTAGE,
	CURRENT,
	TEMP,
	HUMI,
	BRIGHTNESS,
	SMOKE,
	GAS,
	CO,
	INFRARED,
	ACTOR,
	ILLUMINATION,
	POWER,
    BODY
    //add your sensor type
}data_type_t;

typedef enum _wait_channel_ability_t
{
	WAIT_CHANNEL_ABILITY = 1,
	WAIT_FOR_TIME
}wait_channel_ability_t;

typedef enum _report_ack_t
{
	NO_ACK,
	YES_ACK
}report_ack_t;

typedef enum _report_type_t
{
	POWER_ON = 1,
	REGISTER,
	STATE_CHANGE,
    ALARM,
    EX_SENSOR_STATE_CHANGE,
    KEY_CHN1_SINGLE,
    KEY_CHN1_DOUBLE,
    KEY_CHN1_LONG,
    KEY_CHN2_SINGLE,
    KEY_CHN2_DOUBLE,
    KEY_CHN2_LONG,
    KEY_CHN3_SINGLE,
    KEY_CHN3_DOUBLE,
    KEY_CHN3_LONG,
    KEY_CHN4_SINGLE,
    KEY_CHN4_DOUBLE,
    KEY_CHN4_LONG,
    REPORT_NUM
}report_type_t;

typedef enum _report_process_t
{
	IDLE,
	STOP,
	SETTING,
	WAITING
}report_process_t;

typedef enum _report_attribute_t
{
	UNRELIABLE = 0,
	RELIABLE
}report_attribute_t;

enum 
{
	REPORT_NO,
	REPORT_GW,
	REPORT_SB
};

typedef enum _state_change_mode_t
{
    FREE,
    P2P,
    BROADCAST,
    LOCAL
}state_change_mode_t;

typedef enum _flag_type_t
{
	UNFINISHED,
	FINISHED
}flag_type_t;

enum
{
    FALSE = 0,
    TRUE
};

typedef struct _report_para_t
{
	uint8_t state_flag;//状态标志
	uint8_t ack_state;//ACK状态
	report_type_t report_typ;//上报类型
	uint8_t report_seq;//上报帧序号
	uint16_t time_count;//时间计数
	uint8_t retry_count;//重试计数
}report_para_t;

typedef struct _delay_data_t
{
	uint32_t invalid_time;//无效时间
	uint32_t freq;//频率
	int32_t power_on_time_count;//上电时间计数
	int32_t register_time_count;//注册时间计数
	int32_t relay_time_count;//继电器时间计数
	int32_t sensor_time_count;//传感器时间计数
    int32_t ex_sensor_time_count;//外部传感器计数
    int32_t alarm_time_count;//外部传感器计数
}delay_data_t;

typedef struct _flags_t
{
	uint8_t power_on_report;//上电上报
	uint8_t register_report;//注册上报
	uint8_t state_report_start;//开始状态上报
	uint8_t dev_state;//设备状态
    uint8_t ex_sensor_state;
	uint8_t sensor_fixed_length_reporting;//传感器定长上报
	uint8_t sensor_fixed_length_report_start;//传感器开始定长上报
	uint8_t sensor_fixed_freq_report_start;//传感器开始定频上报
	uint8_t alarm_report_start;//告警上报
}flags_t;

typedef struct _sensor_data_t
{
	data_type_t sensor_type;//传感器类型
	uint8_t fixed_len_reporting;//定长上报
    int32_t sensor_cur;//传感器最新值
    uint16_t sensor_step;//传感器步长
	uint16_t sensor_freq;//传感器频率
	int32_t last_report;//最近一次上报
	int32_t sensor_base;//传感器基准
	int32_t sensor_window;//传感器窗口
}sensor_data_t;

typedef struct _relay_data_t//8bits
{
	uint8_t *cur;
}relay_data_t;

typedef struct _ex_sensor_data_t//8bits
{
	uint8_t *cur;
}ex_sensor_data_t;

typedef struct _judge_data_t
{
#if MAX_ACTOR_NUM
	uint8_t  taker_id[ID_LEN];
	uint8_t  last_id[ID_LEN];
	uint8_t  equipment_gid;
    uint8_t  find_myself;
#endif
#if SUBSCRIBER_NUM
	uint8_t subs_pos;
	uint8_t subs_id[ID_LEN * SUBSCRIBER_NUM];
#endif

	uint8_t * report_object;
	uint8_t  gw_id[ID_LEN];
	uint8_t  my_id[ID_LEN];
    uint8_t  default_report;
}judge_data_t;

typedef struct _sub_pos_t
{
#if MAX_ACTOR_NUM
	uint8_t state_change_pos;
#endif
#if MAX_SENSOR_NUM
	uint8_t fixed_length_pos;
	uint8_t fixed_freq_pos;
#endif
#if MAX_ALARM_NUM
	uint8_t alarm_pos;
#endif
}sub_pos_t;

#if MAX_ACTOR_NUM
extern relay_data_t relay_data[MAX_ACTOR_NUM];
extern uint8_t relay_data_bak[MAX_ACTOR_NUM];
extern void local_key_report(void);
#endif

#if EX_SENSOR_NUM
extern ex_sensor_data_t ex_sensor_data[EX_SENSOR_NUM];//u8数据类型
extern uint8_t ex_sensor_data_back[EX_SENSOR_NUM];
#endif

#if MAX_ALARM_NUM
extern relay_data_t alarm_data[MAX_ALARM_NUM];
extern uint8_t alarm_data_bak[MAX_ALARM_NUM];
#endif

#if MAX_SENSOR_NUM
extern sensor_data_t sensor_data[MAX_SENSOR_NUM];
extern int8_t get_sensor_from_type(data_type_t data_type);
extern void reload_freq_infor(void);
#endif

extern judge_data_t judge_data;

extern void start_key_signal_report(int idx);
void auto_report_100msec_task(void);
extern void stop_report_para(uint8_t pos);
extern void report_frame_ack(uint8_t seq, uint8_t source_addr[], uint8_t *fbd);
extern void auto_report_init(uint32_t invalid_time, uint8_t gateway_id[], uint8_t my_id[], uint8_t sid[], uint8_t *report_object);
extern void register_report(uint8_t gateway_id[]);
extern void save_subscriber_infor(uint8_t cmd,uint8_t *said,uint8_t *taid);
extern void save_subscriber_infor_ready(void);

uint8_t get_1byte_bit1_number(uint8_t data, uint8_t pos);
extern int peek_chn(int chn, uint8_t buf[], int len);
extern int get_chn(int chn, uint8_t buf[], int len);
extern int put_chn(int chn, const void *in, int len);
extern void clear_chn(int chn);
void clear_equipment_gid_flag(void);//清除设备GID信息
#endif
