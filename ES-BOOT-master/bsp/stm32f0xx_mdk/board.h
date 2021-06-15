#ifndef _BOARD_H_
#define _BOARD_H_

#include <stdint.h>
#include <stm32f0xx.h>

void board_setup(void);

void flash_read(uint32_t addr, uint8_t *out, uint32_t len);
void flash_write(uint32_t addr, uint8_t *in, uint32_t len);
void flash_erase(uint32_t addr, uint32_t len);
#endif
