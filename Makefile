CROSS   := aarch64-linux-gnu
CC      := $(CROSS)-gcc
AS      := $(CROSS)-as
LD      := $(CROSS)-ld
OBJCOPY := $(CROSS)-objcopy

CFLAGS  := -ffreestanding -nostdlib -nostartfiles -O2 -Wall -mgeneral-regs-only -I./src
LDFLAGS := -T linker.ld -nostdlib

TARGET  := kernel8

SRCS_C  := src/main.c src/uart.c
SRCS_S  := src/boot.S

OBJS    := $(SRCS_S:.S=.o) $(SRCS_C:.c=.o)

all: $(TARGET).img

%.o: %.S
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET).elf: $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $^

$(TARGET).img: $(TARGET).elf
	$(OBJCOPY) -O binary $< $@

run: $(TARGET).img
	qemu-system-aarch64 \
		-M raspi3b \
		-kernel kernel8.img \
		-serial stdio \
		-display none

clean:
	rm -f src/*.o $(TARGET).elf $(TARGET).img 