#include <types.h>
#include <log.h>
#include <dbg.h>
#include <func.h>
#include <sl.h>


log_t log[N_LOGS];
u64 log_idx = 0;

sl_t log_fuse = 0;

const char *log_msg[] = {
	" NONE\n",

    " DBG_TRY\n",
    " DBG_ACQUIRE\n",
    " DBG_RELEASE\n",

    " SCHED_TRY\n",
    " SCHED_ACQUIRE\n",
    " SCHED_RELEASE\n",

    " PROC_SWITCH\n",
};

void log_print(void) {

    sl_acquire(&log_fuse);

    for (u64 i = 0; i < log_idx; i++) {

        dbg_puts("CORE ");
        dbg_putu(log[i].core);
        dbg_puts(" PID ");
        dbg_putu(log[i].pid);
        dbg_puts(" in ");
        dbg_puts(func_names[log[i].func]);
        dbg_puts(" at line ");
        dbg_putu(log[i].line);
        dbg_puts("\n");


    }

}
