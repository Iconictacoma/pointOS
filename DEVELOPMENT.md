# pointOS development guide.

## Project Structure

```
pointOS/
├── boot/              # Bootloader code
│   └── bootloader.asm # x86 16-bit real mode bootloader
├── kernel/            # Kernel code (C/ASM)
├── include/           # Header files
├── scripts/           # Build and utility scripts
├── build/             # Build output (generated)
├── Makefile           # Build system
├── .gitignore         # Git ignore rules
└── README.md          # Project info
```

## Prerequisites

Before building, ensure you have:

1. **NASM** - Netwide Assembler for x86 assembly
   - Windows: `choco install nasm` or download from [nasm.us](https://www.nasm.us/)
   - macOS: `brew install nasm`
   - Linux: `sudo apt-get install nasm`

2. **QEMU** - Emulator for testing
   - Windows: `choco install qemu` or download from [qemu.org](https://www.qemu.org/)
   - macOS: `brew install qemu`
   - Linux: `sudo apt-get install qemu-system-x86`

3. **Make** - Build automation (optional but recommended....)
   - Windows: `choco install make`
   - macOS: Usually pre-installed
   - Linux: `sudo apt-get install build-essential`

## Building

### Simple build:
```bash
make build
```

### Run in QEMU emulator:
```bash
make run
```

### Clean build artifacts:
```bash
make clean
```

## Project Phases

### Phase 1: bootloader
- [x] 16-bit real mode bootloader
- [x] Basic BIOS screen output
- [x] Boot signature (0xAA55)

### Phase 2: Kernel setup (Next)
- [ ] Switch to 32-bit protected mode
- [ ] Load kernel from disk
- [ ] Set up memory segmentation

### Phase 3: Core kernel
- [ ] Interrupt handler (IVT)
- [ ] Memory management
- [ ] Simple process scheduler

### Phase 4: User space
- [ ] Shell/command prompt
- [ ] Basic filesystem
- [ ] System calls

## Current Status

The bootloader is working and displays "Welcome to pointOS!" on startup. You can:
1. Build with `make build`
2. Run with QEMU using `make run`
3. See the boot message appear in the emulator window

## Next Steps

1. **Enhance bootloader**: Add disk reading capability
2. **Create kernel stub**: Prepare for protected mode
3. **Add serial output**: For debugging
4. **Implement memory management**: Basic paging/segmentation

## Resources

- [OSDev.org](https://wiki.osdev.org/) - Comprehensive OS dev wiki
- [Intel x86 Manual](https://software.intel.com/content/www/us/en/develop/articles/intel-sdm.html) - CPU instructions
- [NASM Manual](https://www.nasm.us/doc/) - Assembler documentation
- [QEMU Documentation](https://www.qemu.org/documentation/) - Emulator docs

## Debugging

### With GDB:
```bash
make debug
```

Then in another terminal:
```bash
gdb
(gdb) target remote :1234
(gdb) break *0x7c00
(gdb) continue
```
