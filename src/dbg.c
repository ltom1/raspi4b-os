#include <types.h>
#include <dbg.h>
#include <uart.h>

#include <stdarg.h>


void dbg_init(void) {

#ifdef DBG_UART1
    uart1_init();
#endif
}


void dbg_info(const char *fmt, ...) {

    va_list vl;
    va_start(vl, fmt);

    char c;                 // go character by character
    for (u64 i = 0; (c = fmt[i]) != 0; i++) {
        
        if (c != '%') {     // no variables to paste
            dbg_putc(c);
            continue;
        }

        i++;                // get the character after the '%'
        c = fmt[i];
        switch (c) {        // determine type of the variable
            case 'd':
                dbg_putd((u64)va_arg(vl, u64));
                break;
            case 'u':
                dbg_putu((s64)va_arg(vl, u64));
                break;
            case 's':
                dbg_puts((const char*)va_arg(vl, u64));
                break;
            case 'c':
                dbg_putc((char)va_arg(vl, u64));
                break;
            case 'x':
            case 'p':
                dbg_putx((u64)va_arg(vl, u64));
                break;
            case '%':       // %% will result in a single % getting printed
                dbg_putc('%');
                break;
            default:        // no matching types -> print everything as it is
                dbg_putc('%');
                dbg_putc(c);
                break;
        }
    }

    va_end(vl);
}


void dbg_putc(char c) {

#ifdef DBG_UART1
    uart1_putc(c);
#endif
}


void dbg_puts(const char *str) {

    u64 i = 0;
    char c = str[i];
    while (c != 0) {
        dbg_putc(c);
        i++;
        c = str[i];
    }
}


void dbg_putx(u64 hex) {

    u64 tmp;
    // i -> (0 - 15) u64 has 16 hex digits
    // for each digit...
    for (s64 i = 15; i >= 0; i--) {
        tmp = hex;

        // eliminate all other digits
        tmp = tmp >> i * 4;
        tmp &= 0xf;

        // calculate ascii character
        tmp += ((tmp < 10) ? '0' : 'a' - 10);
        dbg_putc(tmp);
    }
}


void dbg_putu(u64 num) {

    // max value: 18 446 744 073 709 551 615 -> 20 digits
    char buf[20];
    s64 i = 0;

    do {
        buf[i++] = num % 10 + '0';  // calculate ascii character
    } while ((num /= 10) != 0);     // next decimal place

    // print in reverse order
    while (--i >= 0)
        dbg_putc(buf[i]);
}


void dbg_putd(s64 num) {

     if (num < 0) {
        dbg_putc('-');
        num *= -1;
    }
    dbg_putu(num);
}
