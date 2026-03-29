[BITS 16]
[ORG 0x7C00]

; Simple bootloader for pointOS
; Loaded by BIOS at address 0x7C00

start:
    ; Set up segments
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00

    ; Clear screen
    mov ax, 0x0600        ; Clear screen function
    mov bh, 0x07          ; White on black
    mov cx, 0x0000        ; Start position (0,0)
    mov dx, 0x184F        ; End position (24,79)
    int 0x10

    ; Print boot message
    mov si, boot_msg
    call print_string

    ; Halt CPU
    cli
    hlt

print_string:
    ; SI contains address of string
    ; String ends with null terminator
    lodsb                 ; Load byte from DS:SI into AL and increment SI
    or al, al             ; Check if null terminator
    jz .done
    mov ah, 0x0E          ; TTY mode
    int 0x10              ; BIOS interrupt for character output
    jmp print_string
.done:
    ret

boot_msg:
    db "Welcome to pointOS!", 0

; Bootsector padding and boot signature
times (510 - ($-$$)) db 0   ; Pad to 510 bytes
boot_signature:
    dw 0xAA55             ; Boot signature (required by BIOS)
