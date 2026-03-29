#include "../include/idt.h"

/* IDT table - 256 entries */
static idt_entry idt[256];
static idt_descriptor idt_desc;

extern void idt_load(void);

void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags)
{
    idt[num].offset_low = (base & 0xFFFF);
    idt[num].selector = sel;
    idt[num].zero = 0;
    idt[num].flags = flags;
    idt[num].offset_high = (base >> 16) & 0xFFFF;
}

void idt_init(void)
{
    idt_desc.limit = sizeof(idt) - 1;
    idt_desc.base = (uint32_t)&idt;

    /* Clear IDT */
    for (int i = 0; i < 256; i++) {
        idt_set_gate(i, 0, 0x08, 0);
    }

    /* Load IDT using assembly stub */
    asm volatile("lidt %0" : : "m"(idt_desc));
    asm volatile("sti");  /* Enable interrupts */
}
