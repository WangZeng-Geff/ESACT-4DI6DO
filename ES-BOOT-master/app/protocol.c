#include "comfunc.h"
#include "fifo.h"
#include "update.h"
#include "protocol.h"
#include "board.h"

//---------------------------------------------------------------------------------------
uint8_t g_frame_cache[0x100];
//---------------------------------------------------------------------------------------
struct SmartFrame *get_smart_frame(const uint8_t *in, uint8_t len)
{
    struct SmartFrame *pframe;
    uint8_t i = 0;
    uint8_t dlen;

 start_lbl:
    while (i < len)
    {
        if (STC == in[i])
            break;
        i++;
    }
    if (len - i < SMART_FRAME_HEAD)
        return NULL;
    pframe = (struct SmartFrame *)&in[i];
    dlen = pframe->len;

    if (i + SMART_FRAME_HEAD + dlen + 1 > len)
    {
        i++;
        goto start_lbl;
    }

    if (pframe->data[dlen] != checksum(pframe, dlen + SMART_FRAME_HEAD))
    {
        i++;
        goto start_lbl;
    }
    pframe = (struct SmartFrame *)&in[i];
    return pframe;
}
//---------------------------------------------------------------------------------------
uint16_t code_local_frame(const uint8_t *in, uint16_t len, void *out)
{
	static uint8_t _seq = 0;
    struct SmartFrame *pframe = (struct SmartFrame *)out;

    pframe->stc = STC;
    memset(pframe->said, 0x00, AID_LEN);
    memset(pframe->taid, 0x00, AID_LEN);
    pframe->seq = _seq++ & 0x7F;
    memcpy(&pframe->data, in, len);
    pframe->len = len;
    pframe->data[pframe->len] = checksum(pframe, SMART_FRAME_HEAD + pframe->len);
    return SMART_FRAME_HEAD + pframe->len + 1;
}
//---------------------------------------------------------------------------------------
static int8_t do_local_frame(struct SmartFrame *pframe)
{
    int8_t ret = 0;

    if (pframe->data[0] == CMD_UPDATE) 
    {
        ret = do_update(&pframe->data[1], pframe->len - 1);
    }

    if (ret > 0)
    {
        ret = code_local_frame(&pframe->data[0], ret+1, pframe);
        uart_write((uint8_t *)pframe, ret);
    }
    return 0;
}
//---------------------------------------------------------------------------------------
int8_t smart_frame_handle(struct SmartFrame *frame)
{
    if (is_all_xx(frame->said, 0x00, AID_LEN) && is_all_xx(frame->taid, 0x00, AID_LEN))
    {
        return do_local_frame(frame);
    }
    return 0;
}
//---------------------------------------------------------------------------------------
