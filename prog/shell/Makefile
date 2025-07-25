SRC = src/
INCLUDE = include/
BUILD = build/

ASM_SRC = $(shell find $(SRC) -type f -name '*.S')
ASM_OBJ = $(patsubst $(SRC)%.S,$(BUILD)%.S.o,$(ASM_SRC))

C_SRC = $(shell find $(SRC) -type f -name '*.c')
C_OBJ = $(patsubst $(SRC)%.c,$(BUILD)%.c.o,$(C_SRC))

HEADERS = $(shell find $(INCLUDE) -type f -name '*.h')


CFLAGS += -Wall -O0 -ffreestanding -nostdinc -nostdlib -nostartfiles -I$(INCLUDE) -DDEBUG -ggdb3 

PROG_NAME = $(shell basename $(shell pwd))


all: clean $(PROG_NAME).elf

$(PROG_NAME).elf: $(C_OBJ) $(ASM_OBJ) $(HEADERS)
	$(LD) -nostdlib $(ASM_OBJ) $(C_OBJ) -o $(BUILD)$@


$(BUILD)%.S.o: $(SRC)%.S
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)%.c.o: $(SRC)%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf -- $(BUILD)
