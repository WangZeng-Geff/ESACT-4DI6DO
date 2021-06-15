#include <utils.h>
#include <os.h>
#include <printk.h>
#include <settings.h>
#include "shell.h"
#include "dev.h"
#include "board.h"

#if defined(configUSING_SHELL)

int msh_help(int argc, char **argv)
{
    struct shell_syscall *index;

    printk("OS shell commands:\r\n");
    for (index = _syscall_table_begin; index < _syscall_table_end; index++)
        printk("%-10s - %s\r\n", index->name, index->desc);
    printk("\r\n");
    return 0;
}
SHELL_CMD_EXPORT(msh_help, help, "os shell help.");

static int cmd_reboot(int argc, char **argv)
{
    board_reboot(0);

    return 0;
}
SHELL_CMD_EXPORT(cmd_reboot, reboot, "reboot");


#endif
