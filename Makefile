.PHONY: all clean run build qemu

# Directories
BUILD_DIR = build
BOOT_DIR = boot
KERNEL_DIR = kernel

# Tools
ASM = nasm
CC = gcc
LD = ld
QEMU = qemu-system-i386

# Compiler flags
ASMFLAGS = -f bin
CFLAGS = -c -ffreestanding -Wall -Wextra
LDFLAGS = -T linker.ld

# Build targets
BOOTLOADER = $(BUILD_DIR)/bootloader.bin
KERNEL = $(BUILD_DIR)/kernel.bin
DISK_IMAGE = $(BUILD_DIR)/pointos.img

all: $(DISK_IMAGE)

# Build bootloader
$(BOOTLOADER): $(BOOT_DIR)/bootloader.asm
	@mkdir -p $(BUILD_DIR)
	$(ASM) $(ASMFLAGS) $< -o $@

# Create disk image
$(DISK_IMAGE): $(BOOTLOADER)
	@mkdir -p $(BUILD_DIR)
	cp $(BOOTLOADER) $@
	@echo "Disk image created: $@"

# Run in QEMU
run: $(DISK_IMAGE)
	$(QEMU) -drive format=raw,file=$(DISK_IMAGE) -m 256

# Debug with GDB
debug: $(DISK_IMAGE)
	$(QEMU) -drive format=raw,file=$(DISK_IMAGE) -m 256 -s -S

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR)/*
	@echo "Build artifacts cleaned"

build: clean all
	@echo "Build complete!"
