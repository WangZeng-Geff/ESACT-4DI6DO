#include "comfunc.h"
#include "fifo.h"
#include "board.h"
#include "protocol.h"
#include "update.h"
#include "task.h"

//-------------------------------------------------------------------------------------------------
int dev_rst_tmr = 0;
static void dev_rst_100ms_hook(void)
{
    if (dev_rst_tmr > 0)
    {
        if (--dev_rst_tmr == 0)
        {
            board_reset();
        }
    }
}

static void on_100ms(void)
{
	dev_rst_100ms_hook();
}
static void on_1s(void)
{
    update_sec_hook();
}
//-------------------------------------------------------------------------------------------------
volatile uint32_t __sys_tick = 0;
static void sys_tick_handler(void)
{
    static volatile uint32_t last_tick = 0;
    static uint8_t _msec = 0;

	if (systick_time_out())
	{
		__sys_tick ++;
	}

    if (__sys_tick - last_tick >= 10)
    {
        last_tick += 10;
        on_100ms();
        if (++_msec < 10)
            return;
        _msec = 0;
        on_1s();
    }
}

//-------------------------------------------------------------------------------------------------
static void uart_handler(void)
{
	uint16_t len;
	if (!uart_tx_rx()) return;
	 len = uart_peek(g_frame_cache, sizeof(g_frame_cache));
    struct SmartFrame *frame = get_smart_frame(g_frame_cache, len);
    if (!frame) return;

    uart_read(g_frame_cache, sizeof(g_frame_cache));
    smart_frame_handle(frame);
}

//-------------------------------------------------------------------------------------------------
typedef void (*pv_t)(void);
static void jump_to_app(void)
{
    uint32_t user_sp = *(volatile uint32_t *) (APP_ADDR + 0x0000);
    uint32_t user_pc = *(volatile uint32_t *) (APP_ADDR + 0x0004);

    pv_t pv = (pv_t)user_pc;

	/* Remap the Vector Table */
	board_vector_move();

    __set_MSP(user_sp);

    pv();
}

//-------------------------------------------------------------------------------------------------
#define UPDATE_PROG_FLAG    0xAA55
int main(void)
{
    disable_irq();
    board_setup();

	uint32_t flag;
    board_get_update_para(&flag, sizeof(flag)); 
    if (flag != UPDATE_PROG_FLAG)
    {
        jump_to_app();
    }
    while (1)
    {
		sys_tick_handler();
		uart_handler();
		board_clr_wtd();
    }
}
//-------------------------------------------------------------------------------------------------
