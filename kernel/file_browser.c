#include "../include/file_browser.h"
#include "../include/kernel.h"
#include <string.h>

file_browser *fb_create(int row, int col, int width, int height)
{
    file_browser *fb = (file_browser *)0x400000;  /* Allocate in high memory */
    
    fb->row = row;
    fb->col = col;
    fb->width = width;
    fb->height = height;
    fb->selected_index = 0;
    fb->scroll_offset = 0;
    fb->current_dir = fs_get_root();
    
    return fb;
}

void fb_draw(file_browser *fb)
{
    if (!fb || !fb->current_dir) {
        return;
    }
    
    /* Draw file browser border */
    ui_draw_box(fb->row, fb->col, fb->width, fb->height, 
                VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
    
    /* Draw header */
    vga_write_string(fb->row, fb->col + 2, "File Browser", 
                     VGA_COLOR_LIGHT_CYAN, VGA_COLOR_BLACK);
    
    /* Draw column headers */
    vga_write_string(fb->row + 1, fb->col + 2, "Name", 
                     VGA_COLOR_WHITE, VGA_COLOR_BLACK);
    vga_write_string(fb->row + 1, fb->col + 30, "Type", 
                     VGA_COLOR_WHITE, VGA_COLOR_BLACK);
    vga_write_string(fb->row + 1, fb->col + 42, "Size", 
                     VGA_COLOR_WHITE, VGA_COLOR_BLACK);
    
    /* Draw entries */
    uint32_t entry_count = fs_get_entry_count(fb->current_dir);
    int display_height = fb->height - 4;
    
    for (int i = 0; i < display_height && i + fb->scroll_offset < entry_count; i++) {
        file_entry *entry = fs_get_entry(fb->current_dir, i + fb->scroll_offset);
        if (!entry) continue;
        
        int line = fb->row + 2 + i;
        vga_color fg, bg;
        
        if (i + fb->scroll_offset == fb->selected_index) {
            fg = VGA_COLOR_BLACK;
            bg = VGA_COLOR_LIGHT_GREY;
        } else {
            fg = VGA_COLOR_WHITE;
            bg = VGA_COLOR_BLACK;
        }
        
        /* Draw entry row */
        for (int x = 0; x < fb->width - 2; x++) {
            vga_write_char(line, fb->col + 1 + x, ' ', fg, bg);
        }
        
        /* Draw entry details */
        vga_write_char(line, fb->col + 2, 
                      (entry->type == FILE_TYPE_DIRECTORY) ? '[' : ' ', 
                      fg, bg);
        vga_write_string(line, fb->col + 3, entry->name, fg, bg);
        
        if (entry->type == FILE_TYPE_DIRECTORY) {
            vga_write_string(line, fb->col + 30, "<DIR>", fg, bg);
        } else {
            vga_write_string(line, fb->col + 30, "FILE", fg, bg);
        }
        
        /* Format and display size */
        char size_str[16];
        if (entry->size < 1024) {
            /* Simple size formatting */
            int size = entry->size;
            int digit1 = size / 100;
            int digit2 = (size / 10) % 10;
            int digit3 = size % 10;
            size_str[0] = '0' + digit1;
            size_str[1] = '0' + digit2;
            size_str[2] = '0' + digit3;
            size_str[3] = 'B';
            size_str[4] = '\0';
        } else {
            size_str[0] = 'K';
            size_str[1] = 'B';
            size_str[2] = '\0';
        }
        vga_write_string(line, fb->col + 42, size_str, fg, bg);
    }
    
    /* Draw scrollbar indicator if needed */
    if (entry_count > display_height) {
        vga_write_string(fb->row + fb->height - 1, fb->col + fb->width - 4, 
                        "[..]", VGA_COLOR_YELLOW, VGA_COLOR_BLACK);
    }
}

void fb_navigate_up(file_browser *fb)
{
    if (!fb) return;
    
    if (fb->selected_index > 0) {
        fb->selected_index--;
        
        /* Adjust scroll offset */
        if (fb->selected_index < fb->scroll_offset) {
            fb->scroll_offset = fb->selected_index;
        }
    }
}

void fb_navigate_down(file_browser *fb)
{
    if (!fb || !fb->current_dir) return;
    
    uint32_t entry_count = fs_get_entry_count(fb->current_dir);
    if (fb->selected_index < entry_count - 1) {
        fb->selected_index++;
        
        /* Adjust scroll offset */
        int display_height = fb->height - 4;
        if (fb->selected_index >= fb->scroll_offset + display_height) {
            fb->scroll_offset = fb->selected_index - display_height + 1;
        }
    }
}

void fb_enter_directory(file_browser *fb)
{
    if (!fb || !fb->current_dir) return;
    
    file_entry *entry = fs_get_entry(fb->current_dir, fb->selected_index);
    if (entry && entry->type == FILE_TYPE_DIRECTORY) {
        /* For simulation, just reset to root - actual traversal later */
        fb->selected_index = 0;
        fb->scroll_offset = 0;
    }
}

void fb_go_parent_directory(file_browser *fb)
{
    if (!fb) return;
    
    /* For simulation, just stay at root */
    fb->selected_index = 0;
    fb->scroll_offset = 0;
}

void fb_destroy(file_browser *fb)
{
    if (fb) {
        fs_free_directory(fb->current_dir);
    }
}
