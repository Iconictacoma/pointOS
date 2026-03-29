[BITS 16]
[ORG 0x7C00]

; Enhanced bootloader for pointOS with kernel loading
; Stage 1 bootloader - loads stage 2 and kernel from disk

KERNEL_LOAD_ADDR equ 0x10000   ; Load kernel at 64KB
KERNEL_SECTORS equ 100          ; Max kernel size in sectors

start:
    ; Initialize segment registers
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00
    
    ; Save boot drive number
    mov byte [boot_drive], dl

    ; Clear screen
    mov ax, 0x0600
    mov bh, 0x07
    mov cx, 0x0000
    mov dx, 0x184F
    int 0x10

    ; Display loading message
    mov si, loading_msg
    call print_string

    ; Load kernel from disk
    mov ax, KERNEL_LOAD_ADDR     ; Load address (segment)
    mov es, ax
    xor bx, bx                    ; Offset 0x0000
    mov ah, 0x02                  ; Read sectors function
    mov al, KERNEL_SECTORS        ; Number of sectors to read
    mov ch, 0                      ; Cylinder 0
    mov cl, 2                      ; Start at sector 2 (sector 1 is boot)
    mov dh, 0                      ; Head 0
    mov dl, [boot_drive]           ; Drive number
    int 0x13                       ; Read sectors from disk

    jc disk_error                  ; Check Carry Flag for error

    mov si, kernel_loaded_msg
    call print_string

    ; Prepare to enter protected mode
    ; Disable interrupts
    cli

    ; Load GDT
    lgdt [gdt_descriptor]

    ; Enable A20 line (allows access to 1MB+ memory)
    call enable_a20

    ; Set protection enable bit in CR0
    mov eax, cr0
    or eax, 1
    mov cr0, eax

    ; Far jump to protected mode (clear pipeline)
    jmp 0x08:protected_mode_start

disk_error:
    mov si, error_msg
    call print_string
    cli
    hlt

enable_a20:
    ; Use keyboard controller method (most compatible)
    mov al, 0xD1
    out 0x64, al
    mov al, 0xDF
    out 0x60, al
    ret

print_string:
    lodsb
    or al, al
    jz .done
    mov ah, 0x0E
    int 0x10
    jmp print_string
.done:
    ret

; GDT (Global Descriptor Table)
gdt_start:
    ; Null descriptor
    dd 0x0
    dd 0x0

    ; Code segment (0x08)
    db 0xFF, 0xFF, 0x0, 0x0, 0x0, 0x9A, 0xCF, 0x0

    ; Data segment (0x10)
    db 0xFF, 0xFF, 0x0, 0x0, 0x0, 0x92, 0xCF, 0x0

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

[BITS 32]

protected_mode_start:
    ; Set all segment registers to data segment
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    
    ; Set up stack in 32-bit mode
    mov esp, 0x200000

    ; Jump to kernel entry point
    jmp KERNEL_LOAD_ADDR

    cli
    hlt

[BITS 16]

boot_drive:
    db 0

loading_msg:
    db "pointOS bootloader", 0x0D, 0x0A, "Loading kernel...", 0

kernel_loaded_msg:
    db "Kernel loaded! Entering protected mode...", 0x0D, 0x0A, 0

error_msg:
    db "Error: Failed to load kernel!", 0x0D, 0x0A, 0

; Boot sector padding and signature
times (510 - ($-$$)) db 0
boot_signature:
    dw 0xAA55
