#ifndef _COMFUNC_H_
#define _COMFUNC_H_

#include <stdint.h>
#include <string.h>

//#define NULL ((void*)0)

#define SIZE_1K 1024L
#define SIZE_1M (SIZE_1K*SIZE_1K)

#define set_bit(x, bit) ((x) |= 1 << (bit))
#define clr_bit(x, bit) ((x) &= ~(1 << (bit)))
#define tst_bit(x, bit) ((x) & (1 << (bit)))
#define get_bits(val,x1,x2) (((val)>>(x1))&((1<<((x2)-(x1)+1))-1))

#define min(a, b) ((a)<(b) ? (a):(b))
#define max(a, b) ((a)>(b) ? (a):(b))

#define array_size(array) (sizeof(array)/sizeof(*array))

#define _swab16(x) ((uint16_t)(				\
    (((uint16_t)(x) & (uint16_t)0x00ffU) << 8) |			\
    (((uint16_t)(x) & (uint16_t)0xff00U) >> 8)))

#define _swab32(x) ((uint32_t)(				\
    (((uint32_t)(x) & (uint32_t)0x000000ffUL) << 24) |		\
    (((uint32_t)(x) & (uint32_t)0x0000ff00UL) <<  8) |		\
    (((uint32_t)(x) & (uint32_t)0x00ff0000UL) >>  8) |		\
    (((uint32_t)(x) & (uint32_t)0xff000000UL) >> 24)))

#ifdef BIG_ENDIAN
#define cpu_to_le32(x) ((uint32_t)_swab32((x)))
#define le32_to_cpu(x) ((uint32_t)_swab32((uint32_t)(x)))
#define cpu_to_le16(x) ((uint16_t)_swab16((x)))
#define le16_to_cpu(x) ((uint16_t)_swab16((uint16_t)(x)))
#define cpu_to_be32(x) ((uint32_t)(x))
#define be32_to_cpu(x) ((uint32_t)(x))
#define cpu_to_be16(x) ((uint16_t)(x))
#define be16_to_cpu(x) ((uint16_t)(x))
#else
#define cpu_to_le32(x) ((uint32_t)(x))
#define le32_to_cpu(x) ((uint32_t)(x))
#define cpu_to_le16(x) ((uint16_t)(x))
#define le16_to_cpu(x) ((uint16_t)(x))
#define cpu_to_be32(x) ((uint32_t)_swab32((x)))
#define be32_to_cpu(x) ((uint32_t)_swab32((uint32_t)(x)))
#define cpu_to_be16(x) ((uint16_t)_swab16((x)))
#define be16_to_cpu(x) ((uint16_t)_swab16((uint16_t)(x)))
#endif

#define offsetof(TYPE, MEMBER) ((int) &((TYPE *)0)->MEMBER)

#define bcd2bin(val) (((val) & 0x0f) + ((val) >> 4) * 10)
#define bin2bcd(val) ((((val) / 10) << 4) + (val) % 10)

uint8_t checksum(const void *data, uint8_t len);

/* byte order */
uint32_t get_le_val(const uint8_t *p, uint8_t bytes);
uint16_t get_be_val(const uint8_t * p, uint8_t bytes);
void put_le_val(uint16_t val, uint8_t * p, uint8_t bytes);
void put_be_val(uint16_t val, uint8_t * p, uint8_t bytes);

int is_all_xx(const void *s1, uint8_t val, int n);

uint16_t calc_crc16(uint16_t crc, const void *buf, uint16_t size);

#endif
