#include "board.h"
#include "spi_bitbang.h"

#pragma diag_suppress 61
static u32 spi_bitbang_xfer_word(u32 word, u8 bits)
{
    u32 oldbit = (!(word & (1 << (bits - 1)))) << 31;

    for (word <<= (32 - bits); bits; bits--)
    {
        if ((u32)(word & (u32)(1 << 31)) != oldbit)
        {
            setmosi(word & (u32)(1 << 31));
            oldbit = word & (u32)(1 << 31);
        }

        //spidelay();
        setsck(1);
        //spidelay();

        word <<= 1;
        word |= getmiso();

        setsck(0);
    }
    return word;
}

u32 spi_bitbang_xfer(const void *in, void *out, u32 len, bool cs_take, bool cs_release)
{
    const u8 *send_ptr = (const u8 *)in;
    u8 *recv_ptr = (u8 *)out;

    u32 size = len;

    if (cs_take) active(1);

    while (size--)
    {
        u8 data = 0xFF;

        if (send_ptr) data = *send_ptr++;

        data = spi_bitbang_xfer_word(data, 8);

        if (recv_ptr) *recv_ptr++ = data;
    }

    if (cs_release) active(0);

    return len;
}

