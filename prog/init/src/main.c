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
    dbg_info("Hello from pid %u\n", pid);

    u8 child = sys_fork();

    if (child) {
        while (1) {
            dbg_info("Parent!\n");
            sleep_qs(1000000);
            sys_yield();
        }
    } else {
        while (1) {
            dbg_info("Child!\n");
            sleep_qs(1000000);
            sys_yield();
        }
    }

    while(1);
/*
    u8 child = sys_spawn("/PROG/SHELL.ELF");

    sys_print("HELLYEAH\n");
    sys_set_prio(child, 2);

    while (1) {
        dbg_info("I'm init\n");
        sleep_qs(1000000);
        sys_yield();
    }
*/
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
