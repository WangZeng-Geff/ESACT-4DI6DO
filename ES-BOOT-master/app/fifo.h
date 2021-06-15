#ifndef _FIFO_H_
#define _FIFO_H_

#include <stdint.h>

struct fifo
{
    uint16_t tx, rx;
    uint16_t len;
    uint8_t *buf;
};

#define fifo_full(f) ((((f)->rx + 1) & ((f)->len-1)) == (f)->tx)
#define fifo_empty(f) ((f)->tx == (f)->rx)
#define fifo_bytes(f) (((f)->rx + (f)->len - (f)->tx) & ((f)->len - 1))
#define fifo_empty_bytes(f) ((f)->tx + (f)->len - (f)->rx - 1) & ((f)->len - 1)

void fifo_init(struct fifo *f, uint8_t * buf, uint16_t len);
uint16_t fifo_put(struct fifo *f, const uint8_t * buf, uint16_t len);
uint16_t fifo_peek(struct fifo * f, uint8_t * buf, uint16_t len);
uint16_t fifo_get(struct fifo *f, uint8_t * buf, uint16_t len);

#endif
