#include "../include/kernel.h"
#include "../include/ui.h"
#include "../include/keyboard.h"
#include "../include/idt.h"
#include "../include/filesystem.h"
#include "../include/file_browser.h"

#define VGA_BUFFER 0xB8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

static struct vga_char *vga_buffer = (struct vga_char *)VGA_BUFFER;

void vga_clear_screen(void)
{
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        vga_buffer[i].character = ' ';
        vga_buffer[i].color = VGA_COLOR_LIGHT_GREY | (VGA_COLOR_BLACK << 4);
    }
}

void vga_write_char(int row, int col, char character, vga_color fg, vga_color bg)
{
    if (row >= 0 && row < VGA_HEIGHT && col >= 0 && col < VGA_WIDTH) {
        int index = row * VGA_WIDTH + col;
        vga_buffer[index].character = character;
        vga_buffer[index].color = fg | (bg << 4);
    }
}

void vga_write_string(int row, int col, const char *string, vga_color fg, vga_color bg)
{
    for (int i = 0; string[i] != '\0'; i++) {
        if (col >= VGA_WIDTH) {
            row++;
            col = 0;
        }
        if (row >= VGA_HEIGHT) {
            break;
        }
        vga_write_char(row, col, string[i], fg, bg);
        col++;
    }
}

void kernel_main(void)
{
    /* Initialize kernel systems */
    vga_clear_screen();
    idt_init();
    keyboard_init();
    ui_init();

    /* Draw header */
    vga_write_string(0, 0, "pointOS - System Menu", VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK);
    vga_write_string(1, 0, "====================", VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK);

    /* Create menu items */
    menu_item items[] = {
        {"System Information", 1},
        {"Memory Status", 2},
        {"Process List", 3},
        {"File Browser", 4},
        {"Exit", 5}
    };

    menu main_menu = {
        .items = items,
        .item_count = 5,
        .selected_index = 0,
        .row = 4,
        .col = 20
    };

    /* Draw UI */
    ui_draw_menu(&main_menu, main_menu.row, main_menu.col);
    ui_draw_status_bar("Use UP/DOWN arrows to navigate, ENTER to select, ESC to quit", 
                       VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);

    /* Simple menu loop */
    int running = 1;
    while (running) {
        keyboard_wait_key();
        uint8_t scancode = keyboard_get_scancode();
        
        switch (scancode) {
            case KEY_UP:
                ui_update_menu_selection(&main_menu, -1);
                ui_draw_menu(&main_menu, main_menu.row, main_menu.col);
                break;
                
            case KEY_DOWN:
                ui_update_menu_selection(&main_menu, 1);
                ui_draw_menu(&main_menu, main_menu.row, main_menu.col);
                break;
                
            case KEY_ENTER:
                {
                    int selected_id = main_menu.items[main_menu.selected_index].id;
                    
                    /* Handle menu selection */
                    switch (selected_id) {
                        case 1:
                            vga_write_string(12, 0, "System Information: pointOS v0.1", VGA_COLOR_CYAN, VGA_COLOR_BLACK);
                            vga_write_string(13, 0, "Boot Mode: 32-bit Protected Mode", VGA_COLOR_CYAN, VGA_COLOR_BLACK);
                            vga_write_string(14, 0, "Memory: 256 MB (configured)", VGA_COLOR_CYAN, VGA_COLOR_BLACK);
                            break;
                            
                        case 2:
                            vga_write_string(12, 0, "Memory Status:", VGA_COLOR_YELLOW, VGA_COLOR_BLACK);
                            vga_write_string(13, 0, "Available: Calculating...", VGA_COLOR_YELLOW, VGA_COLOR_BLACK);
                            break;
                            
                        case 3:
                            vga_write_string(12, 0, "Process List:", VGA_COLOR_YELLOW, VGA_COLOR_BLACK);
                            vga_write_string(13, 0, "[Kernel] - Running", VGA_COLOR_YELLOW, VGA_COLOR_BLACK);
                            break;
                            
                        case 4:
                            /* File Browser */
                            {
                                vga_clear_screen();
                                vga_write_string(0, 0, "File Browser", VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK);
                                
                                /* Initialize filesystem */
                                fs_init();
                                
                                /* Create file browser */
                                file_browser *fb = fb_create(2, 5, 70, 20);
                                fb_draw(fb);
                                
                                ui_draw_status_bar("UP/DOWN to navigate, ENTER to open, ESC to return", 
                                                   VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
                                
                                /* File browser loop */
                                int fb_running = 1;
                                while (fb_running) {
                                    keyboard_wait_key();
                                    uint8_t fb_scancode = keyboard_get_scancode();
                                    
                                    switch (fb_scancode) {
                                        case KEY_UP:
                                            fb_navigate_up(fb);
                                            fb_draw(fb);
                                            break;
                                            
                                        case KEY_DOWN:
                                            fb_navigate_down(fb);
                                            fb_draw(fb);
                                            break;
                                            
                                        case KEY_ENTER:
                                            fb_enter_directory(fb);
                                            fb_draw(fb);
                                            break;
                                            
                                        case KEY_ESCAPE:
                                            fb_running = 0;
                                            fb_destroy(fb);
                                            break;
                                    }
                                }
                                
                                /* Return to main menu */
                                vga_clear_screen();
                                vga_write_string(0, 0, "pointOS - System Menu", VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK);
                                vga_write_string(1, 0, "====================", VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK);
                                ui_draw_menu(&main_menu, main_menu.row, main_menu.col);
                                ui_draw_status_bar("Use UP/DOWN arrows to navigate, ENTER to select, ESC to quit", 
                                                   VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
                            }
                            break;
                            
                        case 5:
                            running = 0;
                            break;
                    }
                }
                break;
                
            case KEY_ESCAPE:
                running = 0;
                break;
        }
    }

    /* Shutdown */
    vga_clear_screen();
    vga_write_string(12, 25, "System Halting...", VGA_COLOR_RED, VGA_COLOR_BLACK);
    
    asm volatile("cli");
    asm volatile("hlt");
}
