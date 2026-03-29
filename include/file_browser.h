#ifndef __FILE_BROWSER_H__
#define __FILE_BROWSER_H__

#include <stdint.h>
#include "filesystem.h"
#include "kernel.h"

typedef struct {
    directory *current_dir;
    uint32_t selected_index;
    int row;
    int col;
    int height;
    int width;
    int scroll_offset;
} file_browser;

/* Function prototypes */
file_browser *fb_create(int row, int col, int width, int height);
void fb_draw(file_browser *fb);
void fb_navigate_up(file_browser *fb);
void fb_navigate_down(file_browser *fb);
void fb_enter_directory(file_browser *fb);
void fb_go_parent_directory(file_browser *fb);
void fb_destroy(file_browser *fb);

#endif /* __FILE_BROWSER_H__ */
