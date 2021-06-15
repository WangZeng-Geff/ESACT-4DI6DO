#include "ES8P508x.h"
#include "lib_config.h"
#include "system_ES8P508x.h"
#include "board.h"
#include "main.h"

/*
 * common delay
 */
void board_udelay(unsigned long us)
{
    volatile unsigned long delay = us * (SystemCoreClock / 8U / 1000000U);
    do
    {
        __NOP();
		IWDT_Clear();
    }while (delay--);
}

void board_mdelay(unsigned long ms)
{
    board_udelay(ms * 1000);
}

/*
 * lock and unlock
 */
static void flash_lock(void)
{
	FlashIAP_RegLock();
    FlashIAP_Disable();
    FlashIap_OpenAll_WPROT();
}
static void flash_unlock(void)
{
	FlashIAP_RegUnLock();
    FlashIAP_Enable();
    FlashIap_CloseAll_WPROT();
}

int es8p508_flash_erase(uint32_t from, uint32_t size)
{
    uint32_t _size = size, _len;
	flash_unlock();
    while (_size > 0)
    {
        _len = min(_size, 1024);
        if (IAP_PageErase(from) != SUCCESS)
            return -1;
        from  += 1024;
        _size -= _len;
		IWDT_Clear();
    }
	flash_lock();
    return size;
}
int es8p508_flash_write(uint32_t addr, const void *data, uint32_t len)
{
    int i;

    if ((len == 0) || (len & 0x03) || ((uint32_t)data & 0x03))
        return -1;

    for (i = 0; i < len; i += 4)
    {
         if (IAP_WordProgram(addr, *((uint32_t *)data + i / 4)) == ERROR)
            return -1;
		 addr += 4;
		IWDT_Clear();
    }

    return len;
}
int es8p508_flash_read(uint32_t addr, void *out, uint32_t len)
{
    uint32_t _len = len;
	flash_unlock();
    while (len--)
    {
        *((volatile uint8_t *)out + len) = *((volatile uint8_t *)addr + len);
		IWDT_Clear();
    }
	flash_lock();
    return _len;
}


void board_vector_move(void)
{
	SCU_RegUnLock();
	SCU_TBL_Offset(APP_ADDR);
	SCU_TBLRemap_Enable();
	SCU_RegLock();
}

static void wdg_init(void)
{
    IWDT_InitStruType x;

    IWDT_RegUnLock();
    x.WDT_Tms = 2000;           //period 2s
    x.WDT_IE  = DISABLE;
    x.WDT_Rst = ENABLE;
    x.WDT_Clock = WDT_CLOCK_WDT;
    IWDT_Init(&x);
    IWDT_Enable();
    IWDT_RegLock();
}

void board_setup(void)
{
    SystemClockConfig();
    DeviceClockAllEnable();
    wdg_init();
}
