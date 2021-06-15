#include "ES8P508x.h"
#include "lib_config.h"
#include "system_ES8P508x.h"
#include "board.h"
#include "comfunc.h"
#include "fifo.h"


void systick_init(void)
{
	SysTick->LOAD = (SystemCoreClock / 100) - 1UL;
	SysTick->VAL = 0;
    SysTick->CTRL = (SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk);
}

int systick_time_out(void)
{
    return (SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk);
}

static void uart_init(void)
{
#ifdef configUSING_UART0	
	GPIO->PBINEB.INEB_1 = 0; //TXD - PB1
	GPIO->PBDIR.DIR_1 = 0;
	GPIO->PBDS.DS_1 = 1;
	GPIO->PBFUNC0.PB1 = 2;

	GPIO->PBINEB.INEB_0 = 0; //RXD - PB0
	GPIO->PBDIR.DIR_0 = 1;
	GPIO->PBDS.DS_0 = 1;
	GPIO->PBFUNC0.PB0 = 2;
#endif

#ifdef configUSING_UART3    
    GPIO->PAINEB.INEB_13 = 0; //TXD - PA13
	GPIO->PADIR.DIR_13 = 0;
	GPIO->PADS.DS_13 = 1;
	GPIO->PAFUNC1.PA13 = 2;

	GPIO->PAINEB.INEB_14 = 0; //RXD - PA14
	GPIO->PADIR.DIR_14 = 1;
	GPIO->PADS.DS_14 = 1;
	GPIO->PAFUNC1.PA14 = 2;
#endif    
    
    NVIC_Init(NVIC_PLC_UART_IRQ,NVIC_Priority_0,ENABLE);
    UART_InitStruType y;
    y.UART_TxMode = UART_DataMode_8;
    y.UART_RxMode = UART_DataMode_8;

    y.UART_StopBits = UART_StopBits_1;
    y.UART_TxPolar  = UART_Polar_Normal;
    y.UART_RxPolar  = UART_Polar_Normal;
    y.UART_BaudRate = 9600;
    y.UART_ClockSet = UART_Clock_1;
    UART_Init(PLC_UART, &y);

    /* enable interrupt */
    UART_TBIMConfig(PLC_UART, UART_TRBIM_Byte);
    UART_RBIMConfig(PLC_UART, UART_TRBIM_Byte);
//    UART_ITConfig(PLC_UART, UART_IT_RB, ENABLE);
	  
    PLC_UART->CON.TXEN = 1;
    PLC_UART->CON.RXEN = 1;
}
//-------------------------------------------------------------------------------------------------
struct fifo uart_tx_fifo, uart_rx_fifo;
static uint8_t uart_tx_cache[0x100], uart_rx_cache[0x100];
static void uart_chn_init(void)
{
    fifo_init(&uart_tx_fifo, uart_tx_cache, sizeof(uart_tx_cache));
    fifo_init(&uart_rx_fifo, uart_rx_cache, sizeof(uart_rx_cache));
}
__STATIC_INLINE uint16_t uart_chn_rx_byte(uint8_t c)
{
    return fifo_put(&uart_rx_fifo, &c, 1);
}

__STATIC_INLINE uint16_t uart_chn_tx_byte(uint8_t *c)
{
    return fifo_get(&uart_tx_fifo, c, 1);
}

__INLINE uint16_t uart_write(uint8_t *data, uint16_t len)
{
    return fifo_put(&uart_tx_fifo, (const uint8_t*)data, len);
}

__INLINE uint16_t uart_peek(uint8_t *data, uint16_t len)
{
    return fifo_peek(&uart_rx_fifo, data, len);
}

__INLINE uint16_t uart_read(uint8_t *data, uint16_t len)
{
    return fifo_get(&uart_rx_fifo, data, len);
}

int uart_tx_rx(void)
{
	uint8_t c;
	if (UART_GetFlagStatus(PLC_UART, UART_FLAG_TB) != RESET)
	{
        if (uart_chn_tx_byte(&c))
        {
            UART_SendByte(PLC_UART, c);
        }
	}

	if (UART_GetFlagStatus(PLC_UART, UART_FLAG_RB) != RESET)
	{
        c = UART_RecByte(PLC_UART);
        if (uart_chn_rx_byte(c)) return 1;
	}
	return 0;
}

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
int flash_erase(uint32_t from, uint32_t size)
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
int flash_write(uint32_t addr, const void *data, uint32_t len)
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
int flash_read(uint32_t addr, void *out, uint32_t len)
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

static int board_setup_rcc(void)
{
	extern unsigned long SystemCoreClock;

	PLLClock_Config(ENABLE, SCU_PLL_HRC, SCU_PLL_48M, ENABLE);  //PLL使能，倍频到48M作为系统时钟 //SCU_PLL_IN16M
    SystemCoreClock = 48000000U;
	
	DeviceClockAllEnable();
    return 0;
}

void board_setup(void)
{
	board_setup_rcc();
	systick_init();
    wdg_init();
	uart_init();
	uart_chn_init();
}
