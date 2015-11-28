#include "monitor.h"
#include "TerminalState.h"

struct TerminalState* monitor;
struct TerminalState real_monitor;

void move_cursor() {
    u16 cursor_location = monitor->cursor_y * 80 + monitor->cursor_x;

    x86_outb(0x3D4, 14);
    x86_outb(0x3D5, cursor_location >> 8);
    x86_outb(0x3D4, 15);
    x86_outb(0x3D5, cursor_location);
}

void init_monitor() {
    monitor = &real_monitor;

    monitor->chars = (u16*) 0xB8000;

    terminal_state_set_colors(monitor, TS_WHITE, TS_BLACK);
    monitor_clear();
}

void monitor_clear() {
    terminal_state_clear(monitor);
    move_cursor();
}

void monitor_clear_with_bg(u16 bg) {
    terminal_state_set_bg(monitor, bg);
    monitor_clear();
}

void monitor_put(char c) {
    terminal_state_putchar(monitor, c);
    move_cursor();
}

void monitor_print_str(char* str) {
    terminal_state_print_str(monitor, str);
    move_cursor();
}

void monitor_print_hex(int n) {
    terminal_state_print_hex(monitor, n);
    move_cursor();
}

void monitor_print_int(int n) {
    terminal_state_print_int(monitor, n);
    move_cursor();
}

void monitor_print_dec(unsigned int n) {
    terminal_state_print_dec(monitor, n);
    move_cursor();
}
