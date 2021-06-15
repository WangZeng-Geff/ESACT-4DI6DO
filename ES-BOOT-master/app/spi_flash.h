#ifndef _SPI_FLASH_H_
#define _SPI_FLASH_H_

#include "board.h"

#define FLASH_25Q16_SIZE            (0x200000u)
#define FLASH_25Q16_SECTOR_SIZE     (4096u)

uint32_t w25qxx_flash_erase(uint32_t pos, uint32_t size);
uint32_t w25qxx_flash_read(uint32_t offset, void *buffer, uint32_t size);
uint32_t w25qxx_flash_write(uint32_t pos, const void *buffer, uint32_t size);

#endif
