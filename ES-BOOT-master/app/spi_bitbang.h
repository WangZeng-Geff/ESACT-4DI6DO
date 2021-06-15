#ifndef _SPI_BITBANG_H_
#define _SPI_BITBANG_H_

#include "main.h"

u32 spi_bitbang_xfer(const void *in, void *out, u32 len, bool cs_take, bool cs_release);


static inline u8 spi_sendrecv8(u8 data)
{
    u8 val;

    spi_bitbang_xfer(&data, NULL, 1, true, false);
    spi_bitbang_xfer(NULL,  &val, 1, false, true);

    return val;
}

static inline size_t spi_send(const void *send_buf, size_t len)
{
    return spi_bitbang_xfer(send_buf, NULL, len, true, true);
}

static inline size_t spi_send_then_recv(const void *send_buf, 
                                        size_t send_length, 
                                        void *recv_buf, 
                                        size_t recv_length)
{
    spi_bitbang_xfer(send_buf, NULL, send_length, true, false);
    spi_bitbang_xfer(NULL, recv_buf, recv_length, false, true);

    return recv_length;
}


static inline size_t spi_send_then_send(const void *send_buf1, 
                                        size_t send_length1, 
                                        const void *send_buf2, 
                                        size_t send_length2)
{
    spi_bitbang_xfer(send_buf1, NULL, send_length1, true, false);
    spi_bitbang_xfer(send_buf2, NULL, send_length2, false, true);

    return send_length1 + send_length2;
}
#endif
