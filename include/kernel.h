#ifndef __KERNEL_H__
#define __KERNEL_H__

#include <stdint.h>
#include <stddef.h>

/* Color attributes for VGA */
typedef enum {
    VGA_COLOR_BLACK = 0,
    VGA_COLOR_BLUE = 1,
    VGA_COLOR_GREEN = 2,
    VGA_COLOR_CYAN = 3,
    VGA_COLOR_RED = 4,
    VGA_COLOR_MAGENTA = 5,
    VGA_COLOR_BROWN = 6,
    VGA_COLOR_LIGHT_GREY = 7,
    VGA_COLOR_DARK_GREY = 8,
    VGA_COLOR_LIGHT_BLUE = 9,
    VGA_COLOR_LIGHT_GREEN = 10,
    VGA_COLOR_LIGHT_CYAN = 11,
    VGA_COLOR_LIGHT_RED = 12,
    VGA_COLOR_LIGHT_MAGENTA = 13,
    VGA_COLOR_LIGHT_BROWN = 14,
    VGA_COLOR_WHITE = 15,
} vga_color;

/* VGA Buffer structure */
struct vga_char {
    uint8_t character;
    uint8_t color;
};

/* Keyboard scan codes */
#define KEY_ESCAPE 0x01
#define KEY_UP 0x48
#define KEY_DOWN 0x50
#define KEY_LEFT 0x4B
#define KEY_RIGHT 0x4D
#define KEY_ENTER 0x1C

/* Function prototypes - VGA */
void vga_clear_screen(void);
void vga_write_char(int row, int col, char character, vga_color fg, vga_color bg);
void vga_write_string(int row, int col, const char *string, vga_color fg, vga_color bg);

/* Function prototypes - UI */
void ui_init(void);

/* Function prototypes - Keyboard */
void keyboard_init(void);
void keyboard_wait_key(void);
uint8_t keyboard_get_scancode(void);

/* Function prototypes - IDT */
void idt_init(void);

/* Main kernel entry */
void kernel_main(void);

#endif /* __KERNEL_H__ */
