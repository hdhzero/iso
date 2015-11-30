#include "TerminalState.h"

u16 build_char_for_terminal(char c, u16 fg, u16 bg) {
    return (bg << 12) | (fg << 8) | c;
}

void terminal_state_set_fg(struct TerminalState* ts, u16 fg) {
    ts->current_fg = fg;
}

void terminal_state_set_bg(struct TerminalState* ts, u16 bg) {
    ts->current_bg = bg;
}

void terminal_state_set_colors(struct TerminalState* ts, u16 fg, u16 bg) {
    ts->current_fg = fg;
    ts->current_bg = bg;
}

void terminal_state_clear(struct TerminalState* ts) {
    ts->cursor_x = 0;
    ts->cursor_y = 0;
    terminal_state_fill(ts, ' ');
}

void terminal_state_fill(struct TerminalState* ts, char c) {
    int i;
    u16 ch;

    ch = build_char_for_terminal(c, ts->current_fg, ts->current_bg);

    for (i = 0; i < 25 * 80; ++i) {
        ts->chars[i] = ch;
    }
}

void terminal_state_scroll(struct TerminalState* ts) {
    int i;
    u16 ch;

    if (ts->cursor_y >= 25) {
        ts->cursor_y = 24;
        ch = build_char_for_terminal(' ', TS_WHITE, ts->current_bg);

        for (i = 0; i < 24 * 80; ++i) {
            ts->chars[i] = ts->chars[i + 80];
        }

        for (i = 24 * 80; i < 25 * 80; ++i) {
            ts->chars[i] = ch;
        }
    }
}

void terminal_state_putchar(struct TerminalState* ts, char c) {
    int x;
    int y;
    u16 ch;

    x = ts->cursor_x;
    y = ts->cursor_y;
    ch = build_char_for_terminal(c, ts->current_fg, ts->current_bg);

    if (c >= ' ') {
        ts->chars[y * 80 + x] = ch;
        x += 1;
    } else if (c == '\n') {
        x = 0;
        y += 1;
    } else if (c == '\t') {
        x = (x + 8) & ~(8 - 1); /* this mask truncate x to an 8's multiple */
    } else if (c == '\r') {
        x = 0;
    } else if (c == 0x08 && x > 0) { /* A backspace */
        x -= 1;
    }

    ts->cursor_y = x >= 80 ? (y + 1) : y;
    ts->cursor_x = x >= 80 ? 0 : x;

    terminal_state_scroll(ts);
}

void terminal_state_print_str(struct TerminalState* ts, char* str) {
    while (*str) {
        terminal_state_putchar(ts, *str);
        ++str;
    }
}

void terminal_state_print_int(struct TerminalState* ts, int n) {
    if (n < 0) {
        terminal_state_putchar(ts, '-');
        terminal_state_print_dec(ts, -n);
    } else {
        terminal_state_print_dec(ts, n);
    }
}

void terminal_state_print_dec(struct TerminalState* ts, unsigned int n) {
    if (n > 0) {
        terminal_state_print_int(ts, n / 10);
        terminal_state_putchar(ts, (n % 10) + '0');
    }
}

void terminal_state_print_hex(struct TerminalState* ts, int n) {
    int i;
    int v;

    terminal_state_putchar(ts, '0');
    terminal_state_putchar(ts, 'x');

    for (i = 0; i < 8; ++i) {
        v = (n >> 28) & 0x0F;

        if (v < 10) {
            terminal_state_putchar(ts, v + '0');
        } else {
            terminal_state_putchar(ts, (v - 10) + 'A');
        }

        n <<= 4;
    }
}

void terminal_state_plot(struct TerminalState* ts) {
    int i;
    u16* video_memory;

    video_memory = (u16*) 0xB8000;

    if (ts->chars != video_memory) {
        for (i = 0; i < 25 * 80; ++i) {
            video_memory[i] = ts->chars[i];
        }

    }

    move_cursor(ts->cursor_x, ts->cursor_y);
}

