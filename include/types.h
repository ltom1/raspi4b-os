#pragma once


typedef volatile unsigned    char        u8;
typedef volatile signed      char        s8;
typedef volatile unsigned    short       u16;
typedef volatile signed      short       s16;
typedef volatile unsigned    int         u32;
typedef volatile signed      int         s32;
typedef volatile unsigned    long long   u64;
typedef volatile signed      long long   s64;

#define NORETURN    __attribute((noreturn))
#define INTERRUPT   __attribute((interrupt))
#define PACKED      __attribute((packed))
#define INLINE      inline __attribute((always_inline))
#define ALIGNED(n)  __attribute((aligned(n)))

#define bool        _Bool
#define true        1
#define false       0

#define QWORD(high_dword, low_dword)    (((u64)(high_dword) << 32) + (u64)(low_dword))
#define DWORD(high_word, low_word)      (((u32)(high_word) << 16) + (u64)(low_word))
#define WORD(high_byte, low_byte)       (((u16)(high_byte) << 8) + (u64)(low_byte))
#define HIGH_BYTE(value)                ((value >> 8) & 0xff)
#define LOW_BYTE(value)                 (value & 0xff)

#define U8_MAX      0xff
#define U16_MAX     0xffff
#define U28_MAX     0xffffff
#define U32_MAX     0xffffffff
#define U48_MAX     0xffffffffffff
#define U64_MAX     0xffffffffffffffff

#define MIN(a, b)   ((a) < (b) ? (a) : (b))
#define MAX(a, b)   ((a) > (b) ? (a) : (b))
