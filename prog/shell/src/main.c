#include <types.h>
#include <sys.h>
#include <asm.h>
#include <dbg.h>


void sleep_qs(u64 qs) {

    register u64 freq, goal, count;

    freq = asm_get_timer_freq();

    // get current count
    goal = asm_get_time();

    goal += ((freq / 1000) * qs) / 1000; 

    do (count = asm_get_time());
    while (count < goal);
}

void _start(void) {

    dbg_init();

    u8 pid = sys_get_pid();
    dbg_info("SHELL from pid %u\n", pid);

    while (1) {
        dbg_info("I'm shell\n");
        sleep_qs(1000000);
        sys_yield();
    }

    /*
    sys_print("Hello world from init!\n");
    u8 pid = sys_get_pid();

    dbg_init();
    dbg_info("Hello from pid %u\n", pid);
    dbg_info("YEAH \n");

    u64 *x = (u64*)sys_alloc_heap(4096);
    *x = 0xdddddddddddddddd;

    sys_debug((u64)x);

    u8 child_pid = sys_fork();

    if (child_pid) {
        sys_print("just forked!\n");
        *x = 0xffffffffffffffff;
        //sys_exit();
    }

    while (1) {
        sleep_qs(1000000);
        sys_debug(*x);
        sys_yield();
    }
    */
}
