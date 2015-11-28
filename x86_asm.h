#ifndef X86_ASM_H
#define X86_ASM_H

#include "common.h"

//void x86_outb(u16 port, u8 value);
//u8 x86_inb(u16 port);
//u16 x86_inw(u16 port);

void x86_outb(u32 port, u32 value);
u32 x86_inb(u32 port);
u32 x86_inw(u32 port);

void x86_int(void);

void x86_idt_flush(u32* ptr);

#endif

