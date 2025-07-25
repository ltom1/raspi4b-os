#pragma once

#include <types.h>
#include <proc.h>
#include <fs/vfs.h>


#define ELF_MAGIC0          0x7f
#define ELF_MAGIC1          'E'
#define ELF_MAGIC2          'L'
#define ELF_MAGIC3          'F'

#define ELF_IDENT_CLASS     4
#define ELF_CLASS64         2

#define ELF_IDENT_ORDER     5
#define ELF_ORDER_LE        1

#define ELF_IDENT_VERSION   5
#define ELF_VERSION1        1

#define ELF_TYPE_NONE       0
#define ELF_TYPE_REL        1
#define ELF_TYPE_EXE        2

#define ELF_ARCH_X86_64     0x3e
#define ELF_ARCH_AARCH64    0xb7

#define ELF_FLAGS_EXE       (1 << 0)
#define ELF_FLAGS_WRITE     (1 << 1)
#define ELF_FLAGS_READ      (1 << 2)


typedef enum {
    SEG_NULL,
    SEG_LOAD,
    SEG_DYNAMIC,
    SEG_INTERP,
    SEG_NOTE,
    SEG_SHLIB,
    SEG_PHDR,
    SEG_LOPROC = 0x70000000,
    SEG_HIPROC = 0x7FFFFFFF
} elf64_seg_type_t;


typedef struct PACKED {
	u8  ident[16];
	u16 type;
	u16 arch;
	u32 version;
	u64 code_entry;
	u64 pht_off;
	u64 sht_off;
	u32 flags;
	u16 size;
	u16 pht_entry_size;
	u16 pht_entries;
	u16 sht_entry_size;
	u16 sht_entries;
	u16 string_tbl_header_index;
} elf64_header_t;


typedef struct PACKED ElfPHeader64 {
	u32 type;
	u32 flags;
	u64 off;
	u64 vaddr;
	u64 paddr;
	u64 size_file;
	u64 size_mem;
	u64 align;
} elf64_pheader_t;


bool elf64_check(file_t *f);
u64 elf64_extract(pcb_t *proc);
u64 *elf64_copy_address_space(pcb_t *old);
