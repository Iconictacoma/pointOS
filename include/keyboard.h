#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

#include <stdint.h>

/* Keyboard scan codes */
#define KEY_ESCAPE 0x01
#define KEY_UP 0x48
#define KEY_DOWN 0x50
#define KEY_LEFT 0x4B
#define KEY_RIGHT 0x4D
#define KEY_ENTER 0x1C

/* Function prototypes */
void keyboard_init(void);
uint8_t keyboard_get_scancode(void);
void keyboard_wait_key(void);
int keyboard_has_key(void);

#endif /* __KEYBOARD_H__ */
