#include <string.h>
#include "fifo.h"

//-------------------------------------------------------------------------------------------------
void fifo_init(struct fifo *f, uint8_t * buf, uint16_t len)
{
    f->rx = f->tx = 0;
    f->buf = buf;
    f->len = len;
}
//-------------------------------------------------------------------------------------------------
uint16_t fifo_put(struct fifo *f, const uint8_t * buf, uint16_t len)
{
    uint16_t i = 0;

    while (!fifo_full(f) && i < len)
    {
        f->buf[f->rx++] = buf[i++];
        f->rx &= f->len - 1;
    }
    return i;
}
//-------------------------------------------------------------------------------------------------
uint16_t fifo_peek(struct fifo * f, uint8_t * buf, uint16_t len)
{
    struct fifo tmp;
    uint16_t i = 0;

    memcpy(&tmp, f, sizeof(struct fifo));
    while (!fifo_empty(&tmp) && i < len)
    {
        buf[i++] = tmp.buf[tmp.tx++];
        tmp.tx &= tmp.len - 1;
    }
    return i;
}
//-------------------------------------------------------------------------------------------------
uint16_t fifo_get(struct fifo * f, uint8_t * buf, uint16_t len)
{
    uint16_t i = 0;

    while (!fifo_empty(f) && i < len)
    {
        buf[i++] = f->buf[f->tx++];
        f->tx &= f->len - 1;
    }
    return i;
}
//-------------------------------------------------------------------------------------------------

