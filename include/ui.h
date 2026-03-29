#ifndef __UI_H__
#define __UI_H__

#include <stdint.h>
#include "kernel.h"

/* UI Element Types */
typedef struct {
    int row;
    int col;
    int width;
    int height;
    vga_color fg_color;
    vga_color bg_color;
} ui_element;

typedef struct {
    const char *label;
    int id;
} menu_item;

typedef struct {
    menu_item *items;
    int item_count;
    int selected_index;
    int row;
    int col;
} menu;

/* Function prototypes */
void ui_draw_border(int row, int col, int width, int height, vga_color fg, vga_color bg);
void ui_draw_box(int row, int col, int width, int height, vga_color fg, vga_color bg);
void ui_draw_menu(menu *m, int row, int col);
void ui_draw_status_bar(const char *text, vga_color fg, vga_color bg);
void ui_init(void);
void ui_update_menu_selection(menu *m, int direction);

#endif /* __UI_H__ */
