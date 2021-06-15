#include <os.h>
#include <utils.h>
#include "device.h"

#ifdef configUSING_KEY

static struct soft_timer key_tmr;

static void key_generate_event(struct key_device *dev)
{
    int i;

    for (i = 0; i < KEY_POOL_MAX; ++i)
    {
        if (!tst_bit(dev->used, i)) continue;

        if (tst_bit(dev->trg, i))   /* key down */
        {
            dev->press_cnt[i] = KEY_LONG_PRESS_TM / KEY_SCAN_PERIOD;
        }

        if (tst_bit(dev->con, i))   /* key continuous pressed */
        {
            if (dev->press_cnt[i] > 0)
            {
                if (--dev->press_cnt[i] == 0)
                {
                    set_bit(dev->key_event.lpressed, i);
                }
            }
            if (!tst_bit(dev->last_con, i)) 
            {
               set_bit(dev->key_event.keydown, i);
            }
        }
        else if (tst_bit(dev->last_con, i))     /* key up, pre key down */
        {
            if (dev->press_cnt[i] > 0)
            {
                if(!dev->single_double_press[i])
                {
                    set_bit(dev->key_event.pressed, i);
                    dev->single_double_press[i] = KEY_DOUBLE_PRESS_TM / KEY_SCAN_PERIOD;
                }
                else
                {
                    set_bit(dev->key_event.double_pressed, i);
                    dev->single_double_press[i] = 0;
                }
            }
            if (!tst_bit(dev->con, i)) 
            {
                set_bit(dev->key_event.keyup, i);
            }
       }
       if(dev->single_double_press[i])
       {
            if(1 == (dev->single_double_press[i]--))
            {
                set_bit(dev->key_event.single_pressed, i);
            }
       }
    }

    dev->last_con = dev->con;
#ifndef USE_KEY_FIRST_STATE   
    if(dev->key_state_init_count > 0)
    {
        dev->key_state_init_count--;
        memset(&dev->key_event, 0, sizeof(dev->key_event));
    }
#endif
    if (dev->parent.owner && !is_all_xx(&dev->key_event,0x00,sizeof(dev->key_event)))
    {
        task_send_signal(dev->parent.owner, SIG_KEY);
    }
}

static void key_tmr_cb(struct soft_timer *st)
{
    int i;
    uint16_t raw = 0;  //eliminate the shaking
    struct key_device *dev = (struct key_device *)st->data;

    for (i = 0; i < KEY_POOL_MAX; ++i)
    {
        uint8_t map;
        if (!tst_bit(dev->used, i)) continue;

        map = dev->press_map[i];
        map <<= 1;
        map |= dev->ops->is_pressed(i) ? 1 : 0;

        if ((map & 0x03) == 0x03)
		{
            set_bit(raw, i);
		}
        dev->press_map[i] = map;
    }

    dev->trg = raw & (raw ^ dev->con);
    dev->con = raw;

    key_generate_event(dev);

    key_tmr.expires += pdMS_TO_TICKS(KEY_SCAN_PERIOD);
    soft_timer_add(&key_tmr);
}
static void key_task_init(struct key_device *key)
{
    key_tmr.cb = key_tmr_cb;
    key_tmr.data = (ubase_t)key;
    key_tmr.expires = INITIAL_JIFFIES + pdMS_TO_TICKS(KEY_SCAN_PERIOD);

    soft_timer_add(&key_tmr);
}

static err_t key_init(struct device *dev)
{
    err_t result = 0;
    struct key_device *key = (struct key_device *)dev;

    key->con = key->trg = key->last_con = 0;
    memset(&key->key_event, 0, sizeof(key->key_event));
    memset(key->press_map, 0, sizeof(key->press_map));
#ifndef USE_KEY_FIRST_STATE
    #define KEY_INIT_COUNT 3
    key->key_state_init_count = KEY_INIT_COUNT;
#endif
    key_task_init(key);

    return result;
}

static err_t key_control(struct device *dev, uint8_t cmd, void *args)
{
    err_t err = 0;
    struct key_device *key = (struct key_device *)dev;
	
    switch (cmd)
    {
    case KEY_CTRL_GET_KEY:
    {
        memcpy(args,&key->key_event,sizeof(key->key_event));
        memset(&key->key_event, 0, sizeof(key->key_event));
    }
    break;

    default :
        break;
    }

    return err;
}

static const struct device_ops key_ops =
{
    .init        = key_init,
    .ctrl        = key_control,
};

err_t key_device_register(struct key_device *key, const char *name, uint32_t flag, void *data)
{
    struct device *dev = &(key->parent);

    dev->ops         = &key_ops;
    dev->user_data   = data;

    device_register(dev, name, flag);
    return 0;
}
#endif

