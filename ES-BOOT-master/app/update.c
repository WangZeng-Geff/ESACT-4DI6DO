#include "comfunc.h"
#include "fifo.h"
#include "protocol.h"
#include "update.h"
#include "board.h"

//-------------------------------------------------------------------------------------------------
#define UPDATE_FINISHED 0xFFFF
//-------------------------------------------------------------------------------------------------
struct UpdateCache
{
    uint32_t file_size;
    uint16_t max_blk_cnt;
    uint16_t block_no;
    uint8_t block_size;
    uint16_t file_crc;
};
static struct UpdateCache update_cache;
//-------------------------------------------------------------------------------------------------
static int8_t check_file_crc(void)
{
	uint8_t tmp[0x80];;
    uint16_t _len, size = update_cache.file_size, _crc = 0, off = 0;

	while (size)
    {
        _len = min(size, sizeof(tmp));
        board_read_app(off, tmp, _len);
        size -= _len;
        off  += _len;
        _crc  = calc_crc16(_crc, tmp, _len);
    }

    return _crc == update_cache.file_crc;
}
//-------------------------------------------------------------------------------------------------
static void update_header_hook(const uint8_t *in, uint8_t len)
{
    struct UpdateFile *pfile = (struct UpdateFile *)in;
    struct UpdateCache *pcache = &update_cache;

    pcache->file_size = get_le_val(pfile->size, sizeof(pcache->file_size));
    pcache->file_crc = get_le_val(pfile->crc, sizeof(pcache->file_crc));
    pcache->block_size = pfile->block_size;
    pcache->max_blk_cnt = (pcache->file_size + pcache->block_size - 1) / pcache->block_size;

    pcache->block_no = 0;
	flash_erase(APP_ADDR, pcache->file_size);
}

#define DEV_RST_DELAY   20 //2s
extern int dev_rst_tmr;
static void update_tail_hook(void)
{
	flash_erase(UPDATE_PARA_ADDR, 4);
    dev_rst_tmr = DEV_RST_DELAY;
}
//-------------------------------------------------------------------------------------------------
static int8_t update_dframe_opt(struct Update *pupdate, uint8_t len, uint16_t seq)
{
    uint32_t addr_off;
	uint8_t tmp[0xFF];
    struct UpdateCache *pcache = &update_cache;

    if (seq == 0)//0 packet
    {
        update_header_hook(pupdate->data, len);
    }
    else
    {
        if (pcache->block_no != seq || len > pcache->block_size)
            goto end;
        addr_off = (pcache->block_no - 1) * pcache->block_size;
		memcpy(tmp, pupdate->data, len);
        if(board_write_app(addr_off, tmp, len) < 0)
		{
			pcache->block_no = 0;
            goto end;
		}
    }

    if (++pcache->block_no > pcache->max_blk_cnt)
    {
        if (!check_file_crc())
        {
            pcache->block_no = 0;
            goto end;
        }
        update_tail_hook();
        pcache->block_no = UPDATE_FINISHED;
    }

end:
    put_le_val(pcache->block_no, pupdate->seq, sizeof(pupdate->seq));
    return 0;
}

#define UPDATE_SEND_PERIOD  2
static uint8_t update_sec = UPDATE_SEND_PERIOD;
//-------------------------------------------------------------------------------------------------
int8_t do_update(const uint8_t *in, uint8_t len)
{
    int8_t ret = 0;
    uint16_t seq;

    struct Update *pupdate = (struct Update *)in;

    seq = get_le_val(pupdate->seq, sizeof(pupdate->seq));

    ret = update_dframe_opt(pupdate, pupdate->len, seq);

    pupdate->len = ret;
    ret = pupdate->ack ? offsetof(struct Update, data) + pupdate->len : 0;
    pupdate->ack = 0;
    update_sec = 0;
    return ret;
}
//-------------------------------------------------------------------------------------------------
void do_requst_update_packet(void)
{
	uint8_t tmp[UPDATE_HEAD + 1];
    memset(tmp, 0, sizeof(tmp));
	tmp[0] = CMD_UPDATE;
    put_le_val(update_cache.block_no, &tmp[1], 2);
    uint16_t len = code_local_frame(tmp, sizeof(tmp), g_frame_cache);
    uart_write(g_frame_cache, len);
}

void update_sec_hook(void)
{
    if (update_sec++ == UPDATE_SEND_PERIOD)
    {
        update_sec = 0;
        do_requst_update_packet();
    }
}
//-------------------------------------------------------------------------------------------------
