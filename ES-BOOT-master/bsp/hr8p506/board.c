#include "HR8P506.h"
#include "lib_config.h"
#include "system_HR8P506.h"
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
//	GPIO_InitSettingType x;
//	x.Signal = GPIO_Pin_Signal_Digital;
//	x.Dir = GPIO_Direction_Output;
//	x.Func = GPIO_Reuse_Func2;
//	x.ODE = GPIO_ODE_Output_Disable;
//	x.DS = GPIO_DS_Output_Strong;
//	x.PUE = GPIO_PUE_Input_Disable;
//	x.PDE = GPIO_PDE_Input_Disable;
//	GPIO_Init(GPIO_Pin_B1, &x); //TXD - PB1

//	x.Dir = GPIO_Direction_Input;
//	GPIO_Init(GPIO_Pin_B0, &x); //RXD - PB0
	
	GPIO->PBINEB.INEB_1 = 0; //TXD - PB1
	GPIO->PBDIR.DIR_1 = 0;
	GPIO->PBDS.DS_1 = 1;
	GPIO->PBFUNC0.PB1 = 2;

	GPIO->PBINEB.INEB_0 = 0; //RXD - PB0
	GPIO->PBDIR.DIR_0 = 1;
	GPIO->PBDS.DS_0 = 1;
	GPIO->PBFUNC0.PB0 = 2;

	UART_InitStruType y;
    y.UART_StopBits = UART_StopBits_1;      //停止位：1
    y.UART_TxMode   = UART_DataMode_8;      //发送数据格式：8位数据
    y.UART_TxPolar  = UART_Polar_Normal;    //发送端口极性：正常
    y.UART_RxMode   = UART_DataMode_8;      //接收数据格式：8位数据
    y.UART_RxPolar  = UART_Polar_Normal;    //接收端口极性：正常
    y.UART_BaudRate = 9600;                 //波特率
    y.UART_ClockSet = UART_Clock_1;         //时钟选择：Pclk
    UART_Init(UART1, &y);

    UART_TBIMConfig(UART1, UART_TBIM_Byte);
    UART_RBIMConfig(UART1, UART_TBIM_Byte);

    UART1_TxEnable();
    UART1_RxEnable();
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

    if (UART_GetFlagStatus(UART1, UART_FLAG_TB) != RESET)
    {
        if (uart_chn_tx_byte(&c))
        {
            UART_SendByte(UART1, c);
        }
    }

    if (UART_GetFlagStatus(UART1, UART_FLAG_RB) != RESET)
    {
        c = UART_RecByte(UART1);
        if (uart_chn_rx_byte(c)) return 1;
    }
	return 0;
}

int flash_erase(uint32_t from, uint32_t size)
{
    uint32_t _size = size, _len;

    while (_size > 0)
    {
        _len = min(_size, 1024);
        if (IAP_PageErase(from) != SUCCESS)
            return -1;
        from  += 1024;
        _size -= _len;
		WDT_Clear();
    }
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
		WDT_Clear();
    }

    return len;
}
int flash_read(uint32_t addr, void *out, uint32_t len)
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
	systick_init();
	uart_init();
	uart_chn_init();
}
