#include "HR8P296.h"
#include "lib_config.h"
#include "system_HR8P296.h"
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
		WDT_Clear();
    }while (delay--);
}

void board_mdelay(unsigned long ms)
{
    board_udelay(ms * 1000);
}


int hr8p296_flash_erase(uint32_t from, uint32_t size)
{
    uint32_t _size = size, _len;

    while (_size > 0)
    {
        _len = min(_size, 1024);
        if (FlashIap_ErasePage(from/1024) != SUCCESS)
            return -1;
        from  += 1024;
        _size -= _len;
		WDT_Clear();
    }
    return size;
}
int hr8p296_flash_write(uint32_t addr, const void *data, uint32_t len)
{
    int i;

    if ((len & 0x03) || ((uint32_t)data & 0x03))
        return -1;

    for (i = 0; i < len; i += 4)
    {
        if (FlashIap_WriteWord(((addr+i) & 0x3FF) >> 2, (addr+i) >> 10, *(uint32_t *)((uint8_t*)data + i)) == ERROR)
            return -1;
		WDT_Clear();
    }

    return len;
}
int hr8p296_flash_read(uint32_t addr, void *out, uint32_t len)
{
    uint32_t _len = len;

    while (len--)
    {
        *((volatile uint8_t *)out + len) = *((volatile uint8_t *)addr + len);
		WDT_Clear();
    }
    return _len;
}


void board_vector_move(void)
{
	SCU_RegUnLock();
	SCU_TBL_Offset(APP_ADDR);
	SCU_TBLRemap_Enable();
	SCU_RegLock();
}

void board_setup(void)
{
    SystemClockConfig();
    DeviceClockAllEnable();
}
