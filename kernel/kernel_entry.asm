[BITS 32]

; Kernel entry point
; Called by bootloader in protected mode

extern kernel_main

section .text
    align 4
    global _start

_start:
    ; Call kernel_main function (defined in C)
    call kernel_main
    
    ; Halt if kernel returns
    cli
    hlt

    ; Infinite loop (should never reach here)
.loop:
    jmp .loop
