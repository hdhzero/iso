#ifndef DESCRIPTORS_H
#define DESCRIPTORS_H

#include "common.h"
#include "isr.h"

void idt_set_gate(u8* ptr, int idx, u32 base, u16 selector, u8 attr);
void set_gate(u8* ptr, u32 base, u16 selector, u8 attr);
void set_initial_idt();

#endif
