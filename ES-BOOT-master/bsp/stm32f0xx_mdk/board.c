#include <stm32f0xx.h>
#include "main.h"
#include "board.h"

static void board_setup_rcc(void)
{
    /* Start with the clocks in their expected state. */
    RCC_DeInit();

    /* Enable HSE (high speed external clock). */
    RCC_HSEConfig(RCC_HSE_ON);

    /* Wait till HSE is ready. */
    while (RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET)
    {
    }

    /* Enable Prefetch Buffer */
    FLASH->ACR |= FLASH_ACR_PRFTBE;
    FLASH->ACR |= (uint32_t)FLASH_ACR_LATENCY;

    /* HCLK = SYSCLK */
    RCC_HCLKConfig(RCC_SYSCLK_Div1);

    /* PCLK2 = HCLK */
    //RCC_PCLK2Config(RCC_HCLK_Div1);

    /* PCLK = HCLK */
    RCC_PCLKConfig(RCC_HCLK_Div1);

    /* PLLCLK = 8MHz * 6 = 48 MHz. */
    RCC_PLLConfig(RCC_PLLSource_PREDIV1, RCC_PLLMul_6);

    /* Enable PLL. */
    RCC_PLLCmd(ENABLE);

    /* Wait till PLL is ready. */
    while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
    {
    }

    /* Select PLL as system clock source. */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    /* Wait till PLL is used as system clock source. */
    while (RCC_GetSYSCLKSource() != 0x08)
    {
    }
}

void flash_read(uint32_t addr, uint8_t *out, uint32_t len)
{
	while (len--)
		*(out++) = *(__IO uint8_t *)(addr++);
}

void flash_write(uint32_t addr, uint8_t *in, uint32_t len)
{
	uint32_t halfword_len = (len + 1) / 2;
	uint16_t halfword;

	FLASH_Unlock();
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPERR);
	while (halfword_len--)
	{
		halfword  = *(in++);
		halfword |= *(in++) << 8;
		while (FLASH_COMPLETE != FLASH_ProgramHalfWord(addr, halfword))
			;
		addr += 2;
		IWDG_ReloadCounter();
	}
	FLASH_Lock();
}

void flash_erase(uint32_t addr, uint32_t len)
{
	FLASH_Unlock();
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPERR);
	uint8_t page_num = (len + FLASH_PAGE_SIZE - 1) / FLASH_PAGE_SIZE;
	while (page_num--)
	{
		while (FLASH_COMPLETE != FLASH_ErasePage(addr))
			;
		addr += FLASH_PAGE_SIZE;
		IWDG_ReloadCounter();
	}
	FLASH_Lock();
}

void board_setup(void)
{
    board_setup_rcc();
}

