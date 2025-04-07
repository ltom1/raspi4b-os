SRC = src/
INCLUDE = include/
BUILD = build/
LINKER_SCRIPT = kernel.ld

ASM_SRC = $(shell find $(SRC) -type f -name '*.S')
ASM_OBJ = $(patsubst %.S,%.o,$(ASM_SRC))

C_SRC = $(shell find $(SRC) -type f -name '*.c')
C_OBJ = $(patsubst %.c,%.o,$(C_SRC))

HEADERS = $(shell find $(INCLUDE) -type f -name '*.h')

CFLAGS = -Wall -O2 -ffreestanding -nostdinc -nostdlib -nostartfiles -I$(INCLUDE) -DDEBUG -ggdb3

all: run

debug: kernel.img
		qemu-system-aarch64 \
	    -s -S -d int,cpu_reset,guest_errors,page -no-reboot -serial null -serial stdio \
		-machine raspi4b -m 2G \
		-kernel kernel.img \
		-dtb raspi4b.dtb \
		-drive if=none,id=sd,format=raw,file=drive.img &
	$(TERM) --working-directory $(shell pwd) --command \
	$(DBG) kernel.elf \
		-ex "target remote localhost:1234" \
        -ex "layout src" \
        -ex "layout regs"
run: kernel.img
	qemu-system-aarch64 \
	    -serial null -serial stdio \
		-machine raspi4b -m 2G \
		-kernel kernel.img \
		-dtb raspi4b.dtb \
		-drive if=none,id=sd,format=raw,file=drive.img

kernel.img: kernel.elf
	$(OBJCOPY) -O binary $< $@

kernel.elf: $(ASM_OBJ) $(C_OBJ) $(HEADERS) $(LINKER_SCRIPT)
	$(LD) -nostdlib -T $(LINKER_SCRIPT) $(ASM_OBJ) $(C_OBJ) -o $@

%.o: %.S
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f -- kernel.img
	rm -f -- *.elf
	rm -f -- *.mem
	rm -f -- *.o
	rm -f -- */*.o
	rm -f -- */*/*.o
	rm -f -- */*/*/*.o
	rm -f -- */*/*/*/*.o
