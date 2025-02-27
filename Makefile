SRC = src/
INCLUDE = include/
BUILD = build/
LINKER_SCRIPT = kernel.ld

ASM_SRC = $(shell find $(SRC) -type f -name '*.asm')
ASM_OBJ = $(patsubst %.asm,%.o,$(ASM_SRC))

C_SRC = $(shell find $(SRC) -type f -name '*.c')
C_OBJ = $(patsubst %.c,%.o,$(C_SRC))

HEADERS = $(shell find $(INCLUDE) -type f -name '*.h')

all: 

kernel.img: kernel.elf


kernel.elf: $(ASM_OBJ) $(C_OBJ) $(HEADERS) $(LINKER_SCRIPT)
	$(LD) -nostdlib -nostartfiles -T $(LINKER_SCRIPT) $(ASM_OBJ) $(C_OBJ) -o $@
