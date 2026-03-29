#ifndef __FILESYSTEM_H__
#define __FILESYSTEM_H__

#include <stdint.h>

/* File types */
typedef enum {
    FILE_TYPE_DIRECTORY = 0,
    FILE_TYPE_FILE = 1,
    FILE_TYPE_DEVICE = 2,
} file_type;

/* File entry structure */
typedef struct {
    char name[32];
    file_type type;
    uint32_t size;
    uint8_t attributes;
} file_entry;

/* Directory structure */
typedef struct {
    file_entry *entries;
    uint32_t entry_count;
    uint32_t capacity;
} directory;

/* Function prototypes */
void fs_init(void);
directory *fs_get_root(void);
directory *fs_open_directory(const char *path);
file_entry *fs_get_entry(directory *dir, uint32_t index);
uint32_t fs_get_entry_count(directory *dir);
void fs_free_directory(directory *dir);

#endif /* __FILESYSTEM_H__ */
