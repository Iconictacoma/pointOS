#ifndef __IDT_H__
#define __IDT_H__

#include <stdint.h>

/* IDT Entry Structure */
typedef struct {
    uint16_t offset_low;
    uint16_t selector;
    uint8_t zero;
    uint8_t flags;
    uint16_t offset_high;
} __attribute__((packed)) idt_entry;

/* IDT Descriptor */
typedef struct {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) idt_descriptor;

/* Function prototypes */
void idt_init(void);
void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags);

#endif /* __IDT_H__ */
