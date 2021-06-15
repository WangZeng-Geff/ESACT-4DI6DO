#include "comfunc.h"

//---------------------------------------------------------------------------------------
uint8_t checksum(const void *data, uint8_t len)
{
    uint8_t cs = 0;

    while (len-- > 0)
        cs += *((uint8_t *) data + len);
    return cs;
}

int is_all_xx(const void *s1, uint8_t val, int n)
{
    while (n && *(uint8_t *) s1 == val)
    {
        s1 = (uint8_t *) s1 + 1;
        n--;
    }
    return !n;
}

//---------------------------------------------------------------------------------------
uint32_t get_le_val(const uint8_t *p, uint8_t bytes)
{
    uint32_t ret = 0;
    while (bytes-- > 0)
    {
        ret <<= 8;
        ret |= *(p + bytes);
    }

    return ret;
}

void put_le_val(uint16_t val, uint8_t *p, uint8_t bytes)
{
    while (bytes-- > 0)
    {
        *p++ = val & 0xFF;
        val >>= 8;
    }
}
//---------------------------------------------------------------------------------------
uint16_t get_be_val(const uint8_t *p, uint8_t bytes)
{
    uint16_t ret = 0;
    while (bytes-- > 0)
    {
        ret <<= 8;
        ret |= *p++;
    }

    return ret;
}
//---------------------------------------------------------------------------------------
uint16_t calc_crc16(uint16_t crc, const void *buf, uint16_t size)
{
	uint8_t i;
    uint8_t *_buf = (uint8_t *) buf;

    while (size-- != 0)
    {
        for (i = 0x80; i != 0; i >>= 1)
        {
            if ((crc & 0x8000) != 0)
            {
                crc <<= 1;
                crc ^= 0x1021;
            }
            else
            {
                crc <<= 1;
            }
            if (((*_buf) & i) != 0)
            {
                crc ^= 0x1021;
            }
        }
        _buf++;
    }
    return crc;
}
//---------------------------------------------------------------------------------------
