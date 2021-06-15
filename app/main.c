#include <config.h>
#include <board.h>
#include <croutine.h>

int main(void)
{
    disable_irq();
    board_setup();

#ifdef configTEST
    extern void test_setup(void);
    test_setup();
#endif
	
    extern void setup_app(void);
    setup_app();
    enable_irq();

    for (;;)
    {
        task_schedule();
    }
}
