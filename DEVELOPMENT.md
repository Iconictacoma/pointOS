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

### Phase 1: Bootloader
- [x] 16-bit real mode bootloader
- [x] Basic BIOS screen output
- [x] Boot signature (0xAA55)

### Phase 2: Kernel Loading ✓
- [x] Switch to 32-bit protected mode
- [x] Load kernel from disk (BIOS INT 0x13)
- [x] Enable A20 line
- [x] Load GDT
- [x] Jump to kernel entry point

### Phase 3: Core Kernel with UI ✓
- [x] VGA text mode output
- [x] Keyboard input handling
- [x] IDT setup (interrupt descriptor table)
- [x] Interactive menu system
- [x] UI framework (windows, borders, status bar)
- [x] File browser UI
- [x] Simulated filesystem (in-memory)
- [ ] Interrupt handlers (keyboard, timer)
- [ ] Memory management
- [ ] Process scheduler

### Phase 4: Advanced Features
- [ ] Real filesystem support (FAT12/FAT16)
- [ ] Disk I/O for file operations
- [ ] Graphics mode support (optional)
- [ ] Advanced input handling
- [ ] Shell/command prompt

### Phase 5: System Services
- [ ] System calls
- [ ] Process management
- [ ] Inter-process communication

## Current Status

The kernel now features:
1. **Bootloader** - Loads kernel from disk and enters protected mode ✓
2. **UI System** - Interactive menu system with keyboard navigation ✓
3. **VGA Output** - Colored text display with box-drawing characters ✓
4. **Keyboard Input** - PIT and basic interrupt handling ✓
5. **File Browser** - Visual file/directory browser with navigation ✓

**Current features at boot:**
- Main menu with 5 options (System Info, Memory Status, Process List, File Browser, Exit)
- File browser showing simulated filesystem:
  - Displays files with icons and sizes
  - Navigate with UP/DOWN arrow keys
  - Open directories with ENTER key
  - Return to menu with ESC
- UP/DOWN arrow navigation
- ENTER to select, ESC to quit
- Status bar with instructions
- Custom borders using box-drawing characters

**To build and run:**
```bash
make build    # Compile bootloader + kernel + file browser
make run      # Launch in QEMU
```

Inside QEMU:
- Navigate main menu with **UP/DOWN** arrow keys
- Select "File Browser" to browse the simulated filesystem
- Use arrow keys to navigate files
- Press ENTER to open directories
- Press ESC to return to main menu
- Each menu item displays custom information

## Resources

- [OSDev.org](https://wiki.osdev.org/) - Comprehensive OS dev wiki
- [Intel x86 Manual](https://software.intel.com/content/www/us/en/develop/articles/intel-sdm.html) - CPU instructions
- [NASM Manual](https://www.nasm.us/doc/) - Assembler documentation
- [QEMU Documentation](https://www.qemu.org/documentation/) - Emulator docs

## File Browser Architecture

The file browser is implemented as a modular component:

**Filesystem Module** (`filesystem.c/h`):
- In-memory simulated filesystem
- Single root directory with sample files/folders
- File entries contain: name, type, size, attributes
- Supports traversal (foundation for real FS later)

**File Browser Module** (`file_browser.c/h`):
- Visual browser component with scrolling
- Navigation: UP/DOWN arrows, ENTER to open
- Displays filename, type indicator, and size
- Highlight shows current selection
- Integration with UI framework

**Current Simulated Files:**
- `boot/` (directory)
- `kernel/` (directory)
- `drivers/` (directory)
- `config.txt` (256 bytes)
- `readme.txt` (512 bytes)
- `system.log` (1024 bytes)

**Future Enhancements:**
1. Real filesystem driver (FAT12/16)
2. Disk I/O integration
3. File operations (rename, delete, copy)
4. File content viewer
5. Directory creation
6. File info dialog

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
