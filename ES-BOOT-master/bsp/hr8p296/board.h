#ifndef _BOARD_H_
#define _BOARD_H_

#include "main.h"
#include "HR8P296.h"
#include "lib_config.h"


#define BOOT_SIZE           (2*1024)
#define APP_SIZE            (62*1024)

/*
 * for internal flash
 */
#define IFLASH_START_ADDR   (0u)
#define APP_ADDR            (IFLASH_START_ADDR + BOOT_SIZE)
#define UPDATE_PARA_ADDR    (APP_ADDR + APP_SIZE)
#define UPDATE_PARA_SIZE    (8u)
#define UPDATE_DATA_ADDR    (UPDATE_PARA_ADDR + UPDATE_PARA_SIZE)

#define board_get_update_para(para, len)   \
     hr8p296_flash_read(UPDATE_PARA_ADDR, (para), (len))
#define board_get_update_data(off, data, len) \
     hr8p296_flash_read(UPDATE_DATA_ADDR+(off), (data), (len))
#define board_erase_update_para(off, len)   \
     hr8p296_flash_erase(UPDATE_PARA_ADDR+(off), (len))

#define board_erase_app(size)   \
     hr8p296_flash_erase(APP_ADDR, size)
#define board_read_app(off, data, len)	\
     hr8p296_flash_read(APP_ADDR+(off), (data), (len))
#define board_write_app(off, data, len)	\
     hr8p296_flash_write(APP_ADDR+(off), (data), (len))


#define enable_irq()      __enable_irq()
#define disable_irq()     __disable_irq()

#define OS_CPU_SR   uint32_t
#define enter_critical()        \
    do { cpu_sr = __get_PRIMASK(); disable_irq();} while (0)
#define exit_critical()         \
    do { __set_PRIMASK(cpu_sr);} while (0)

#define board_reset()   NVIC_SystemReset()

void board_udelay(unsigned long us);
void board_mdelay(unsigned long ms);

void board_setup(void);

int hr8p296_flash_erase(uint32_t from, uint32_t size);
int hr8p296_flash_write(uint32_t addr, const void *data, uint32_t len);
int hr8p296_flash_read(uint32_t addr, void *out, uint32_t len);
void board_vector_move(void); 
#endif
