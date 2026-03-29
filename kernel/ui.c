#include "../include/ui.h"

#define VGA_BUFFER 0xB8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

static struct vga_char *vga_buffer = (struct vga_char *)VGA_BUFFER;

void ui_draw_border(int row, int col, int width, int height, vga_color fg, vga_color bg)
{
    /* Top border */
    vga_write_char(row, col, 201, fg, bg);  /* Top-left corner */
    for (int i = 1; i < width - 1; i++) {
        vga_write_char(row, col + i, 205, fg, bg);  /* Horizontal line */
    }
    vga_write_char(row, col + width - 1, 187, fg, bg);  /* Top-right corner */

    /* Bottom border */
    vga_write_char(row + height - 1, col, 200, fg, bg);  /* Bottom-left corner */
    for (int i = 1; i < width - 1; i++) {
        vga_write_char(row + height - 1, col + i, 205, fg, bg);
    }
    vga_write_char(row + height - 1, col + width - 1, 188, fg, bg);  /* Bottom-right corner */

    /* Left and right borders */
    for (int i = 1; i < height - 1; i++) {
        vga_write_char(row + i, col, 186, fg, bg);  /* Vertical line */
        vga_write_char(row + i, col + width - 1, 186, fg, bg);
    }
}

void ui_draw_box(int row, int col, int width, int height, vga_color fg, vga_color bg)
{
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            vga_write_char(row + y, col + x, ' ', fg, bg);
        }
    }
    ui_draw_border(row, col, width, height, fg, bg);
}

void ui_draw_menu(menu *m, int row, int col)
{
    if (!m || m->item_count == 0) return;

    int menu_height = m->item_count + 2;
    int menu_width = 40;

    /* Draw menu box */
    ui_draw_box(row, col, menu_width, menu_height, VGA_COLOR_WHITE, VGA_COLOR_BLUE);

    /* Draw menu items */
    for (int i = 0; i < m->item_count; i++) {
        vga_color bg = (i == m->selected_index) ? VGA_COLOR_WHITE : VGA_COLOR_BLUE;
        vga_color fg = (i == m->selected_index) ? VGA_COLOR_BLUE : VGA_COLOR_WHITE;
        
        vga_write_char(row + 1 + i, col + 2, (i == m->selected_index) ? '>' : ' ', fg, bg);
        vga_write_string(row + 1 + i, col + 4, m->items[i].label, fg, bg);
    }
}

void ui_draw_status_bar(const char *text, vga_color fg, vga_color bg)
{
    /* Clear status bar line */
    for (int i = 0; i < VGA_WIDTH; i++) {
        vga_write_char(VGA_HEIGHT - 1, i, ' ', fg, bg);
    }
    /* Write status text */
    vga_write_string(VGA_HEIGHT - 1, 0, text, fg, bg);
}

void ui_update_menu_selection(menu *m, int direction)
{
    if (!m) return;
    
    m->selected_index += direction;
    
    if (m->selected_index < 0) {
        m->selected_index = m->item_count - 1;
    } else if (m->selected_index >= m->item_count) {
        m->selected_index = 0;
    }
}

void ui_init(void)
{
    /* Initialize UI subsystem */
}
