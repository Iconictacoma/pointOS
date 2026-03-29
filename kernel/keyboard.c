/* Inline assembly for port I/O */
static inline uint8_t inb(uint16_t port)
{
    uint8_t ret;
    asm volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

static inline void outb(uint16_t port, uint8_t val)
{
    asm volatile("outb %0, %1" : : "a"(val), "Nd"(port));
}

#include "../include/keyboard.h"

/* Keyboard controller ports */
#define KEYBOARD_STATUS_PORT 0x64
#define KEYBOARD_DATA_PORT 0x60

uint8_t keyboard_get_scancode(void)
{
    return inb(KEYBOARD_DATA_PORT);
}

int keyboard_has_key(void)
{
    return (inb(KEYBOARD_STATUS_PORT) & 0x01);
}

void keyboard_wait_key(void)
{
    while (!keyboard_has_key()) {
        asm volatile("hlt");
    }
}

void keyboard_init(void)
{
    /* Keyboard initialized (basic mode) */
}
