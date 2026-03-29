.PHONY: all clean run build qemu

# Directories
BUILD_DIR = build
BOOT_DIR = boot
KERNEL_DIR = kernel

# Tools
ASM = nasm
CC = i686-elf-gcc
LD = i686-elf-ld
OBJCOPY = i686-elf-objcopy
QEMU = qemu-system-i386

# Compiler flags
ASMFLAGS = -f bin
KERNEL_ASMFLAGS = -f elf32
CFLAGS = -c -ffreestanding -fno-builtin -Wall -Wextra -Werror -std=c99
LDFLAGS = -T $(KERNEL_DIR)/linker.ld -m elf_i386

# Build targets
BOOTLOADER = $(BUILD_DIR)/bootloader.bin
KERNEL_ENTRY = $(BUILD_DIR)/kernel_entry.o
KERNEL_C_SOURCES = $(KERNEL_DIR)/kernel.c $(KERNEL_DIR)/ui.c $(KERNEL_DIR)/keyboard.c $(KERNEL_DIR)/idt.c $(KERNEL_DIR)/filesystem.c $(KERNEL_DIR)/file_browser.c
KERNEL_OBJS = $(patsubst $(KERNEL_DIR)/%.c,$(BUILD_DIR)/%.o,$(KERNEL_C_SOURCES))
KERNEL_ELF = $(BUILD_DIR)/kernel.elf
KERNEL_BIN = $(BUILD_DIR)/kernel.bin
DISK_IMAGE = $(BUILD_DIR)/pointos.img

all: $(DISK_IMAGE)

# Build bootloader
$(BOOTLOADER): $(BOOT_DIR)/bootloader.asm
	@mkdir -p $(BUILD_DIR)
	$(ASM) $(ASMFLAGS) $< -o $@
	@echo "✓ Bootloader built"

# Build kernel entry point
$(KERNEL_ENTRY): $(KERNEL_DIR)/kernel_entry.asm
	@mkdir -p $(BUILD_DIR)
	$(ASM) $(KERNEL_ASMFLAGS) $< -o $@
	@echo "✓ Kernel entry point built"

# Build all kernel C object files
$(BUILD_DIR)/%.o: $(KERNEL_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -o $@
	@echo "✓ Compiled $<"

# Link kernel
$(KERNEL_ELF): $(KERNEL_ENTRY) $(KERNEL_OBJS)
	@mkdir -p $(BUILD_DIR)
	$(LD) $(LDFLAGS) $^ -o $@
	@echo "✓ Kernel linked"

# Convert ELF to binary
$(KERNEL_BIN): $(KERNEL_ELF)
	$(OBJCOPY) -O binary $< $@
	@echo "✓ Kernel converted to binary"

# Create disk image (bootloader + kernel)
$(DISK_IMAGE): $(BOOTLOADER) $(KERNEL_BIN)
	@mkdir -p $(BUILD_DIR)
	cat $(BOOTLOADER) $(KERNEL_BIN) > $@
	@echo "✓ Disk image created: $@"

# Run in QEMU
run: $(DISK_IMAGE)
	$(QEMU) -drive format=raw,file=$(DISK_IMAGE) -m 256

# Debug with GDB
debug: $(DISK_IMAGE)
	$(QEMU) -drive format=raw,file=$(DISK_IMAGE) -m 256 -s -S

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR)/*
	@echo "✓ Build artifacts cleaned"

build: clean all
	@echo "✓ Build complete!"
