#ifndef MONITOR_H
#define MONITOR_H

#include "common.h"

void monitor_init();
void monitor_put(char c);
void monitor_clear();
void monitor_clear_with_bg(u16 bg);
void monitor_print_str(char* str);
void monitor_print_hex(int n);
void monitor_print_int(int n);
void monitor_print_dec(unsigned int n);

#endif
