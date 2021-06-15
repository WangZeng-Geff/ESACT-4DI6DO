#ifndef _BOARD_H_
#define _BOARD_H_

#include "ES8P508x.h"
#include "lib_config.h"

//# define configUSING_UART0
#ifdef configUSING_UART0
#define PLC_UART                UART0
#define NVIC_PLC_UART_IRQ       NVIC_UART0_IRQn
#endif

# define configUSING_UART3
#ifdef configUSING_UART3
#define PLC_UART                UART3
#define NVIC_PLC_UART_IRQ       NVIC_UART3_IRQn
#endif

#define BOOT_SIZE           (4*1024)
#define APP_SIZE            (48*1024)

/*
 * for internal flash
 */
#define IFLASH_START_ADDR   (0u)
#define APP_ADDR            (IFLASH_START_ADDR + BOOT_SIZE)
#define UPDATE_PARA_ADDR    (APP_ADDR + APP_SIZE)

#define board_get_update_para(para, len)   \
     flash_read(UPDATE_PARA_ADDR, (para), (len))
#define board_erase_update_para(off, len)   \
     flash_erase(UPDATE_PARA_ADDR+(off), (len))

#define board_erase_app(size)   \
     flash_erase(APP_ADDR, size)
#define board_read_app(off, data, len)	\
     flash_read(APP_ADDR+(off), (data), (len))
#define board_write_app(off, data, len)	\
     flash_write(APP_ADDR+(off), (data), (len))


#define enable_irq()      __enable_irq()
#define disable_irq()     __disable_irq()

#define OS_CPU_SR   uint32_t
#define enter_critical()        \
    do { cpu_sr = __get_PRIMASK(); disable_irq();} while (0)
#define exit_critical()         \
    do { __set_PRIMASK(cpu_sr);} while (0)

#define board_reset()   	NVIC_SystemReset()
#define board_clr_wtd()   	IWDT_Clear()


int systick_time_out(void);
__INLINE uint16_t uart_write(uint8_t *data, uint16_t len);
__INLINE uint16_t uart_peek(uint8_t *data, uint16_t len);
__INLINE uint16_t uart_read(uint8_t *data, uint16_t len);
int uart_tx_rx(void);

void board_vector_move(void);
void board_setup(void);

int flash_erase(uint32_t from, uint32_t size);
int flash_write(uint32_t addr, const void *data, uint32_t len);
int flash_read(uint32_t addr, void *out, uint32_t len);
 
#endif
