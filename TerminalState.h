#ifndef MONITOR_TERMINAL_H
#define MONITOR_TERMINAL_H

#include "common.h"

#define TS_BLACK 0
#define TS_BLUE 1
#define TS_GREEN 2
#define TS_CYAN 3
#define TS_RED 4
#define TS_MAGENTA 5
#define TS_BROWN 6
#define TS_LIGHT_GREY 7
#define TS_DARK_GREEN 8
#define TS_LIGHT_BLUE 9
#define TS_LIGHT_GREEN 10
#define TS_LIGHT_CYAN 11
#define TS_LIGHT_RED 12
#define TS_LIGHT_MAGENTA 13
#define TS_LIGHT_BROWN 14
#define TS_WHITE 15

struct TerminalState {
    int cursor_x;
    int cursor_y;
    u16 current_fg;
    u16 current_bg;
    u16* chars;
};

/* set foreground and background colors */
void terminal_state_set_fg(struct TerminalState* ts, u16 fg);
void terminal_state_set_bg(struct TerminalState* ts, u16 bg);
void terminal_state_set_colors(struct TerminalState* ts, u16 fg, u16 bg);

void terminal_state_clear(struct TerminalState* ts);
void terminal_state_fill(struct TerminalState* ts, char c);
void terminal_state_scroll(struct TerminalState* ts);
void terminal_state_scroll_nlines(struct TerminalState* ts, int lines);

void terminal_state_putchar(struct TerminalState* st, char c);
void terminal_state_print_str(struct TerminalState* ts, char* str);
void terminal_state_print_int(struct TerminalState* ts, int n);
void terminal_state_print_dec(struct TerminalState* ts, unsigned int n);
void terminal_state_print_hex(struct TerminalState* ts, int n);

void terminal_state_plot(struct TerminalState* ts);

#endif
