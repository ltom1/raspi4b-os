SD_DEV1 = /dev/sdb1
SD_DEV2 = /dev/sdb2

SRC = src/
PROG = prog/
INCLUDE = include/
BUILD = build/
LINKER_SCRIPT = kernel.ld

ASM_SRC = $(shell find $(SRC) -type f -name '*.S')
ASM_OBJ = $(patsubst $(SRC)%.S,$(BUILD)%.S.o,$(ASM_SRC))

C_SRC = $(shell find $(SRC) -type f -name '*.c')
C_OBJ = $(patsubst $(SRC)%.c,$(BUILD)%.c.o,$(C_SRC))

HEADERS = $(shell find $(INCLUDE) -type f -name '*.h')

CFLAGS += -Wall -O0 -ffreestanding -nostdinc -nostdlib -nostartfiles -I$(INCLUDE) -DDEBUG -ggdb3 

PROGRAMS = $(shell find $(PROG) -mindepth 1 -maxdepth 1 -type d)

all: run


progs-sd:
	$(foreach p, $(PROGRAMS), make -C $(p) all;)
	mkdir -p mnt
	sudo mount $(SD_DEV2) mnt
	sudo rm -rf -- mnt/PROG
	sudo mkdir -p mnt/PROG
	$(foreach f, $(shell find $(PROG) -type f -name '*.elf'), \
  sudo cp $(f) mnt/PROG/$(shell basename $(f) | awk '{print toupper($$0)}');)
	sudo umount mnt

progs-qemu:
	$(foreach p, $(PROGRAMS), make -C $(p) all;)
	mkdir -p mnt
	sudo losetup -P /dev/loop8 drive.img
	sudo mount /dev/loop8p2 mnt
	sudo rm -rf -- mnt/PROG
	sudo mkdir -p mnt/PROG
	$(foreach f, $(shell find $(PROG) -type f -name '*.elf'), \
  sudo cp $(f) mnt/PROG/$(shell basename $(f) | awk '{print toupper($$0)}');)
	sudo umount mnt
	sudo losetup -d /dev/loop8


mount:
	sudo losetup -P /dev/loop8 drive.img
	sudo mount /dev/loop8p2 mnt

umount:
	sudo umount mnt
	sudo losetup -d /dev/loop8



debug: kernel.img
		sudo qemu-system-aarch64 \
		-s -S -d unimp,int,cpu_reset,guest_errors,page -no-reboot \
		-machine raspi4b -m 2G \
		-kernel $(BUILD)$< \
		-dtb raspi4b.dtb \
		-sd drive.img > output.txt &
	$(TERM) --working-directory $(shell pwd) --command \
	$(DBG) $(BUILD)/kernel.elf \
		-ex "target remote localhost:1234" \
        -ex "layout src" \
        -ex "layout regs"

run: kernel.img
	qemu-system-aarch64 \
	    -serial null -serial stdio \
		-machine raspi4b -m 2G \
		-dtb raspi4b.dtb \
		-kernel $(BUILD)$< \
		-sd drive.img

sd: kernel.img
	mkdir -p mnt
	sudo mount $(SD_DEV1) mnt
	sudo cp $(BUILD)$< mnt/kernel8.img
	sudo umount mnt

kernel.img: kernel.elf
	$(OBJCOPY) -O binary $(BUILD)$< $(BUILD)$@

kernel.elf: $(ASM_OBJ) $(C_OBJ) $(HEADERS) $(LINKER_SCRIPT)
	$(LD) -nostdlib -T $(LINKER_SCRIPT) $(ASM_OBJ) $(C_OBJ) -o $(BUILD)$@


$(BUILD)%.S.o: $(SRC)%.S
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)%.c.o: $(SRC)%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf -- $(BUILD)
