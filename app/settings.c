#include <config.h>
#include <utils.h>
#include <device.h>
#include "board.h"
#include "dev.h"
#include "settings.h"
#include "proto_smart.h"
#include "printk.h"
#include <ES8P508x.h>
#include "proto_smart.h"

setting_t setting;
switch_param_t switch_param[RESTORE_KEY_NUM];
dev_state_info_t dev_state;
static uint32_t setting_start_addr = SETTING_INVALID_ADDR;

static device_t *iflash;
static uint8_t tmp[FLASH_SECTOR_SIZE];
static uint32_t key_config_start_addr[RESTORE_KEY_NUM] = {SETTING_INVALID_ADDR, SETTING_INVALID_ADDR, SETTING_INVALID_ADDR, SETTING_INVALID_ADDR };
/*
 * for disk use
 */
static void check_disk_open(void)
{
    if (iflash) return;

    iflash = device_find("iflash");
    assert(iflash);

    device_open(iflash, 0);
}
int disk_erase(uint32_t off, uint32_t size)
{
    check_disk_open();
    struct flash_erase_cmd sfec;
    sfec.start = off;
    sfec.size  = size;
    return device_ctrl(iflash, DEVICE_CTRL_BLK_ERASE, &sfec);
}
int disk_write(uint32_t off, const void *data, uint32_t size)
{
    check_disk_open();

    return device_write(iflash, off, data, size);
}
int disk_read(uint32_t off, void *data, uint32_t size)
{
    check_disk_open();

    return device_read(iflash, off, data, size);
}

void dev_restore_factory(void)
{
    uint8_t i=0;
	//memset(&setting.para,0x00,sizeof(setting.para));
	setting.dev_infor.report = 3;
    setting.dev_infor.interval = 20;
	setting.dev_infor.try_cnt  = 3;
    setting.dev_infor.relay_poweron_mode = 2;
    setting.dev_infor.lock = 0;
    setting.dev_infor.flag_relay_ctrl_mutex =0;
    for( i=0; i < RESTORE_RELAY_NUM;i++)
    {
        setting.dev_infor.relay_para[i].normal_state = RELAY_NORMAL_OPEN;
        setting.dev_infor.relay_para[i].out_mode     = RELAY_OUT_LEVEL;
        setting.dev_infor.relay_para[i].output_time  = 0;
    }
    
    setting_save();
    log_d("dev_restore_factory!\r\n");
}

static void setting_default(void)
{
    memset(&setting, 0, sizeof(setting));
	//uint8_t sn[]= {0x31,0x71,0x40,0x70,0x00,0x51,0x74,0x21,0x01,0x00,0x00,0x89};
    //uint8_t dk[]= {0x30,0x30,0x30,0x30,0x33,0x50,0x53,0x35};
    
#ifdef configUSING_DEBUG
    /*
	 uint8_t sn[12]= {0x31,0x71,0x10,0x10,0x10,0x61,0x94,0x41,0x99,0x00,0x00,0x05};
 	 uint8_t dk[8]= {0x30,0x30,0x30,0x30,0x31,0x36,0x59,0x55};
	 put_le_val(335933, setting.encode.id, 4);
     put_le_val(55382, setting.encode.pwd, 2);
    */
      
	 uint8_t sn[12]= {0x31,0x71,0x11,0x50,0x00,0x12,0x03,0x81,0x99,0x00,0x00,0x02};
 	 uint8_t dk[8]= {0x30,0x30,0x30,0x30,0x33,0x33,0x46,0x38};
	 put_le_val(588926, setting.encode.id, 4);
     put_le_val(27292, setting.encode.pwd, 2);
 

     memcpy(setting.encode.sn, sn, sizeof(sn));
     memcpy(setting.encode.dkey, dk, sizeof(dk));
#endif
    
//    uint8_t chn = 0;
//    for(chn = 0; chn < RESTORE_RELAY_NUM ;chn++)
//    {
//       setting.dev_infor.relay_tm[2*chn] = 0x46; 
//       setting.dev_infor.relay_tm[2*chn + 1] = 0x32;          
//    }
    
	dev_restore_factory();
    log_d("setting_default!\r\n");
}
/**/
static void flash_save(uint32_t start_addr, uint32_t *valid_addr, void* block, uint32_t block_size)
{
    check_disk_open();

    setting_hdr_t hdr;

    uint32_t addr = *valid_addr + block_size, erase_addr = NULL;

    bool new_sec = false;
    
    recheck_lb:
    
	if (addr + block_size >= start_addr + SETTING_SIZE)
    {
        erase_addr = addr = start_addr;
        new_sec = true;
    }

    if ((addr & FLASH_SECTOR_MASK) + block_size >= FLASH_SECTOR_SIZE)
    {
        erase_addr = (addr + FLASH_SECTOR_SIZE) & (~FLASH_SECTOR_MASK);
        new_sec = true;
    }

    // erase next sector if a new sector 
    if (new_sec)
    {
        struct flash_erase_cmd sfec;
        sfec.start = erase_addr;
        sfec.size  = FLASH_SECTOR_SIZE;
        device_ctrl(iflash, DEVICE_CTRL_BLK_ERASE, &sfec);
    }

    // check memory 
    device_read(iflash, addr, tmp, block_size);
	log_d("the next valid addr: 0x%06X!\r\n", addr);
    
    if(!is_all_xx((uint8_t *)&tmp, 0xFF, block_size))//
    {
        addr += block_size;
        goto recheck_lb;//
    }
    assert(is_all_xx(tmp, 0xFF, block_size));

	if (!device_write(iflash, addr + SETTING_HDR_SIZE, setting_addr(block), SETTING_DAT_SIZE(block_size)))
		log_d("setting_save write error!\r\n");

	hdr.crc = crc16(0, setting_addr(block), SETTING_DAT_SIZE(block_size));
    
    if(start_addr == SETTING_FROM )
    {
        hdr.magic = SETTING_MAGIC;
    }
    else
    {
        hdr.magic = SWITCH_PARAM_MAGIC  ;
    }
    device_write(iflash, addr, &hdr, sizeof(hdr));
	
    if (*valid_addr < start_addr + SETTING_SIZE)
    {
        memset(&hdr, 0, sizeof(hdr));
        device_write(iflash, *valid_addr, &hdr, sizeof(hdr));
    }
    *valid_addr = addr;
}
static bool flash_load(uint32_t start_addr, uint32_t *valid_addr, void* block, uint32_t block_size)
{
    check_disk_open();

    setting_hdr_t hdr;
    uint32_t addr = start_addr; 
	
    for (; addr < start_addr + SETTING_SIZE; addr += block_size)
    {
        board_feed_wdg();
        /* check magic no */
        device_read(iflash, addr, &hdr, sizeof(setting_hdr_t));
        if ((hdr.magic != SETTING_MAGIC)&&(SWITCH_PARAM_MAGIC != hdr.magic)) continue;
        /* check crc */
        device_read(iflash, addr, block, block_size);
        if (hdr.crc != crc16(0, setting_addr(block), SETTING_DAT_SIZE(block_size))) continue;
        /* got really settings */
        *valid_addr = addr;
        log_d("flash_load here has data addr: 0x%06X!\r\n", addr);
        return true;
    }
    return false;
}
/*
typedef struct 
{ 
    uint8_t cmd;
    uint8_t did[2];
    uint8_t data[2];
} app_switch_set_t;
*/
#define CMD_LEN             0x01
#define CTRL_LEN            0x01
#define DID_LEN         	0x02
#define SID_LEN         	0x02
#define AID_LEN         	0x04



void switch_param_save(uint8_t idx, void * data)
{
	flash_save(SWITCH_PARAM_FROM+idx*SETTING_SIZE, &key_config_start_addr[idx], data, sizeof(switch_param_t));
}

bool switch_param_load(uint8_t idx, void * data)
{
	if(flash_load(SWITCH_PARAM_FROM+idx*SETTING_SIZE, &key_config_start_addr[idx], data, sizeof(switch_param_t)))
        return true;
#ifndef configUSING_DEBUG
    if(!dev_state.flag_write_SN) return false;
#endif
    if(!is_all_xx(setting.encode.id, 0x00, ID_LEN))
    {
        switch_param_default(idx);
    }
        return false; 
}

void setting_save(void)
{
   flash_save(SETTING_FROM, &setting_start_addr, (void *)&setting, sizeof(setting_t));
}

void setting_load(void)
{
    uint8_t i;
	
    if(flash_load(SETTING_FROM, &setting_start_addr, (void *)&setting, sizeof(setting_t)))
    {
        if(is_all_xx(setting.encode.id, 0x00, ID_LEN)) return;
        
        for(i = 0; i < RESTORE_KEY_NUM; i++)
        {
            switch_param_load(i, &switch_param[i]);   
            //check_chn_need_sync(i);
        }
        log_d("setting_load successful\r\n");
        return ;
    }

    setting_default();
}
