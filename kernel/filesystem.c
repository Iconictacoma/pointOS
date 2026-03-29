#include "../include/filesystem.h"
#include <string.h>

/* Simulated filesystem in memory */
static directory root_dir;
static int fs_initialized = 0;

/* Helper to add a file entry */
static void fs_add_entry(directory *dir, const char *name, file_type type, uint32_t size)
{
    if (!dir || !dir->entries || dir->entry_count >= dir->capacity) {
        return;
    }
    
    file_entry *entry = &dir->entries[dir->entry_count];
    strncpy(entry->name, name, sizeof(entry->name) - 1);
    entry->name[sizeof(entry->name) - 1] = '\0';
    entry->type = type;
    entry->size = size;
    entry->attributes = 0;
    
    dir->entry_count++;
}

void fs_init(void)
{
    if (fs_initialized) {
        return;
    }
    
    /* Initialize root directory */
    root_dir.entries = (file_entry *)0x300000;  /* Use high memory for file entries */
    root_dir.capacity = 64;
    root_dir.entry_count = 0;
    
    /* Add sample files and directories */
    fs_add_entry(&root_dir, "..", FILE_TYPE_DIRECTORY, 0);
    fs_add_entry(&root_dir, "boot", FILE_TYPE_DIRECTORY, 0);
    fs_add_entry(&root_dir, "kernel", FILE_TYPE_DIRECTORY, 0);
    fs_add_entry(&root_dir, "config.txt", FILE_TYPE_FILE, 256);
    fs_add_entry(&root_dir, "readme.txt", FILE_TYPE_FILE, 512);
    fs_add_entry(&root_dir, "system.log", FILE_TYPE_FILE, 1024);
    fs_add_entry(&root_dir, "drivers", FILE_TYPE_DIRECTORY, 0);
    
    fs_initialized = 1;
}

directory *fs_get_root(void)
{
    if (!fs_initialized) {
        fs_init();
    }
    return &root_dir;
}

directory *fs_open_directory(const char *path)
{
    /* For now, just return root directory */
    /* Full path traversal can be implemented later */
    if (!fs_initialized) {
        fs_init();
    }
    return &root_dir;
}

file_entry *fs_get_entry(directory *dir, uint32_t index)
{
    if (!dir || index >= dir->entry_count) {
        return NULL;
    }
    return &dir->entries[index];
}

uint32_t fs_get_entry_count(directory *dir)
{
    if (!dir) {
        return 0;
    }
    return dir->entry_count;
}

void fs_free_directory(directory *dir)
{
    /* For now, directories are not dynamically allocated */
    (void)dir;
}
