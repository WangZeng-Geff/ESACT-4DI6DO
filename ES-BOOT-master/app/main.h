#ifndef __MAIN_H
#define __MAIN_H

#include <stddef.h>
#include <stdarg.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))

#define UPDATE_PROG_FLAG    0xAA55

typedef struct
{
	uint32_t size;
	uint16_t crc;
	uint16_t flag;
} update_para_t;

#endif
