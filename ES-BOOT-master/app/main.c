#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "board.h"
#include "main.h"

static update_para_t up;

static void load_new_app(uint32_t size)
{
    uint8_t tmp[0x80];
    uint32_t _len, off = 0;

    board_erase_app(size);
    while (size)
    {
        _len = min(size, sizeof(tmp));
        board_get_update_data(off, tmp, _len);
        board_write_app(off, tmp, _len);
        size -= _len;
        off  += _len;
    }
}

static uint16_t crc16(uint16_t crc, const uint8_t *buf, uint32_t size)
{
    unsigned char i;

    while (size--)
    {
        for (i = 0x80; i != 0; i >>= 1)
        {
            if (crc & 0x8000)
            {
                crc <<= 1;
                crc  ^= 0x1021;
            } else
            {
                crc <<= 1;
            }
            if ((*buf) & i)
            crc ^= 0x1021;
        }
        buf++;
    }
    return (crc);
}

static int check_app_crc(uint16_t crc, uint32_t size)
{
    uint8_t tmp[0x80];
    uint16_t _crc = 0, _len;
    uint32_t off = 0;

    while (size)
    {
        _len = min(size, sizeof(tmp));
        board_read_app(off, tmp, _len);
        size -= _len;
        off  += _len;
        _crc  = crc16(_crc, tmp, _len);
    }
    return (crc == _crc);
}

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

int main(void)
{
    board_setup();

    board_get_update_para(&up, sizeof(up)); 
    if (up.flag != UPDATE_PROG_FLAG)
    {
        jump_to_app();
    }
    else
    {
        load_new_app(up.size);
        if (check_app_crc(up.crc, up.size))
        {
            board_erase_update_para(0, sizeof(up));
            board_reset();
        }
    }
    return 0;
}
