#include "descriptors.h"
#include "x86_asm.h"
#include "monitor.h"

#define IDT_START 0x508
#define IDT_PTR 0x500
#define IDT_SIZE (256 * 8) - 1

extern void asm_isr0();

void idt_set_gate(u8* ptr, int idx, u32 base, u16 selector, u8 attr) {
    set_gate(ptr + (idx * 8), base, selector, attr);
}

void set_gate(u8* ptr, u32 base, u16 selector, u8 attr) {
    *((u16*) (ptr + 0)) = base & 0xFFFF;
    *((u16*) (ptr + 2)) = selector;
    *((u8*)  (ptr + 4)) = 0;
    *((u8*)  (ptr + 5)) = attr;
    *((u16*) (ptr + 6)) = (base >> 16) & 0xFFFF;
}

void set_initial_idt() {
    int i;
    u8* idt_ptr = (u8*) IDT_PTR;
    u8* idt = (u8*) IDT_START;

    *((u16*) idt_ptr) = IDT_SIZE;
    *((u32*) (idt_ptr + 2)) = IDT_START;

    kmemset(idt, 0, 8 * 256);

    for (i = 0; i < 256; ++i) {
        idt_set_gate(idt, i, (u32) asm_isr0, 0x08, 0x8E);
    }

    monitor_print_str("\nidt flushing...\n");
    x86_idt_flush((u32*) idt_ptr);
    monitor_print_str("Done!\n");
}

