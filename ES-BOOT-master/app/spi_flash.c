#include "spi_bitbang.h"
#include "spi_flash.h"


#define SECTOR_SIZE           (4096)
#define PAGE_SIZE             (256)

/* JEDEC Manufacturer's ID */
#define MF_ID                 (0x1C)

/* JEDEC Device ID: Memory type and Capacity */
#define MTC_W25Q80_BV         (0x4014) /* W25Q80BV */
#define MTC_EN25Q16A          (0x3015) /* EN25Q16A  */
#define MTC_W25Q16_DW         (0x6015) /* W25Q16DW  */
#define MTC_W25Q32_BV         (0x4016) /* W25Q32BV */
#define MTC_W25Q32_DW         (0x6016) /* W25Q32DW */
#define MTC_W25Q64_BV_CV      (0x4017) /* W25Q64BV W25Q64CV */
#define MTC_W25Q64_DW         (0x4017) /* W25Q64DW */
#define MTC_W25Q128_BV        (0x4018) /* W25Q128BV */
#define MTC_W25Q256_FV        (TBD)    /* W25Q256FV */

/* command list */
#define CMD_WRSR              (0x01)   /* Write Status Register */
#define CMD_PP                (0x02)   /* Page Program */
#define CMD_READ              (0x03)   /* Read Data */
#define CMD_WRDI              (0x04)   /* Write Disable */
#define CMD_RDSR1             (0x05)   /* Read Status Register-1 */
#define CMD_WREN              (0x06)   /* Write Enable */
#define CMD_FAST_READ         (0x0B)   /* Fast Read */
#define CMD_ERASE_4K          (0x20)   /* Sector Erase:4K */
#define CMD_RDSR2             (0x35)   /* Read Status Register-2 */
#define CMD_ERASE_32K         (0x52)   /* 32KB Block Erase */
#define CMD_JEDEC_ID          (0x9F)   /* Read JEDEC ID */
#define CMD_ERASE_full        (0xC7)   /* Chip Erase */
#define CMD_ERASE_64K         (0xD8)   /* 64KB Block Erase */

#define DUMMY                 (0xFF)


static u8 w25qxx_read_status(void)
{
    return spi_sendrecv8(CMD_RDSR1);
}

static void w25qxx_wait_busy(void)
{
    while (w25qxx_read_status() & (0x01));
}

static u32 w25qxx_read(u32 offset, u8 *buffer, u32 size)
{
    u8 send_buffer[4];

    send_buffer[0] = CMD_WRDI;
    spi_send(send_buffer, 1);

    send_buffer[0] = CMD_READ;
    send_buffer[1] = (uint8_t)(offset >> 16);
    send_buffer[2] = (uint8_t)(offset >> 8);
    send_buffer[3] = (uint8_t)(offset);

    spi_send_then_recv(send_buffer, 4, buffer, size);

    return size;
}
u32 w25qxx_flash_read(u32 offset, void *buffer, u32 size)
{
    return w25qxx_read(offset, (u8*)buffer, size);
}

static u32 w25qxx_sector_erase(u32 page_addr)
{
    u8 send_buffer[4];

    send_buffer[0] = CMD_WREN;
    spi_send(send_buffer, 1);

    send_buffer[0] = CMD_ERASE_4K;
    send_buffer[1] = (page_addr >> 16);
    send_buffer[2] = (page_addr >> 8);
    send_buffer[3] = (page_addr);
    spi_send(send_buffer, 4);

    w25qxx_wait_busy();

    send_buffer[0] = CMD_WRDI;
    spi_send(send_buffer, 1);

    return SECTOR_SIZE;
}

u32 w25qxx_flash_erase(u32 pos, u32 size)
{
    u32 _size, _len;

    _size = size;
    while (_size > 0)
    {
        _len = min(_size, SECTOR_SIZE);
        w25qxx_sector_erase(pos);
        pos += SECTOR_SIZE;
        _size -= _len;
    }

    return size;
}

static u32 w25qxx_page_write(u32 page_addr, const u8 *buffer, u32 size)
{
    u8 send_buffer[4];

    send_buffer[0] = CMD_WREN;
    spi_send(send_buffer, 1);

    send_buffer[0] = CMD_PP;
    send_buffer[1] = (uint8_t)(page_addr >> 16);
    send_buffer[2] = (uint8_t)(page_addr >> 8);
    send_buffer[3] = (uint8_t)(page_addr);

    spi_send_then_send(send_buffer,4,buffer,size);

    w25qxx_wait_busy();

    send_buffer[0] = CMD_WRDI;
    spi_send(send_buffer, 1);

    return size;
}

u32 w25qxx_flash_write(u32 pos, const void *buffer, u32 size)
{
    u32 _size = size, idx = 0, _len;

    while (_size > 0)
    {
        _len = pos & 0xFF;
        _len = 0x100 - _len;
        if (_len > _size)
            _len = _size;
        w25qxx_page_write(pos, (u8 *)buffer + idx, _len);
        _size -= _len;
        idx   += _len;
        pos   += _len;
    }

    return size;
}
