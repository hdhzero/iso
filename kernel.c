#include "monitor.h"
#include "descriptors.h"
#include "x86_asm.h"

unsigned char msg2[] = "Hello, World from .data";

void print_mode_info();
void print_modes_available();
void print_hexdump(u8* ptr, int n);
int is_video_mode_on();
u32* get_linear_framebuffer_address();
u32 get_bytes_per_line();
void put_pixel(int x, int y, u32 pixel);

u32* vram;
u32 bytes_per_line;

void main() {
    int i;
    u32* lfb;
    set_initial_idt();

    if (is_video_mode_on()) {
        vram = get_linear_framebuffer_address();
        bytes_per_line = get_bytes_per_line();
        lfb = vram;

        for (i = 0; i < 1024 * 20; ++i) {
            *lfb = 0x0000FF00;
            ++lfb;
        }

        for (i = 0; i < 1024 * 20; ++i) {
            *lfb = 0x000000FF;
            ++lfb;
        }

        put_pixel(1024 / 2, 768 / 2, 0x00FFFFFF);
    } else {
        init_monitor();
        monitor_clear_with_bg(1);
        monitor_print_str("Starting on text mode...\n");
        print_modes_available();
        print_mode_info();
    }

//    asm volatile ("int $0xf7");
 //   x86_int();
    
    while (1) { }
}

void put_pixel(int x, int y, u32 rgb) {
    u8* lfb = (u8*) vram;

    lfb += x * 4 + y * bytes_per_line;
    *((u32*) lfb) = rgb;
}

int is_video_mode_on() {
    u16* ptr = (u16*) 0x7C00;

    return *ptr != 0;
}

u32* get_linear_framebuffer_address() {
    u32* ptr = (u32*) (0x7A00 + 0x28);

    return (u32*) *ptr;
}

u32 get_bytes_per_line() {
    u16* ptr = (u16*) (0x7A00 + 0x10);

    return (u32) *ptr;
}

void print_modes_available() {
    u16 base   = *((u16*) (0x7C00 + 0xE + 2));
    u16 offset = *((u16*) (0x7C00 + 0xE + 0));
    u16* modes = (u16*) (base * 16 + offset);
    int i = 0;

    while (*modes != 0xFFFF) {
        monitor_print_hex(*modes);
        monitor_put(' ');
        ++i;
        ++modes;

        if (i % 4 == 0) {
            monitor_put('\n');
        }
    }
    monitor_put('\n');
}

void print_hexdump(u8* ptr, int n) {
    int i;
    u8 h, b;

    for (i = 0; i < n; ++i) {
        h = ((*(u8*) (ptr + i)) >> 4) & 0x0F;
        b = (*(u8*) (ptr + i)) & 0xF;

        if (i != 0 && i % 16 == 0) {
            monitor_put('\n');
        }

        if (h > 9) {
            monitor_put('A' + (h - 10));
        } else {
            monitor_put('0' + h);
        }

        if (b > 9) {
            monitor_put('A' + (b - 10));
        } else {
            monitor_put('0' + b);
        }

        monitor_put(' ');
    }
    monitor_put('\n');
}

void print_mode_info() {
    u8* ptr = (u8*) 0x7A00;

    monitor_print_str("Mode attr: ");
    monitor_print_hex(*((u16*) (ptr + 0)));
    monitor_put('\n');

    monitor_print_str("Width: ");
    monitor_print_dec(*((u16*) (ptr + 0x12)));
    monitor_put('\n');
    
    monitor_print_str("Height: ");
    monitor_print_dec(*((u16*) (ptr + 0x14)));
    monitor_put('\n');

    monitor_print_str("Bits per pixel: ");
    monitor_print_dec(*((u8*) (ptr + 0x19)));
    monitor_put('\t');
    monitor_print_str("Mem model: ");
    monitor_print_dec(*((u8*) (ptr + 0x1B)));
    monitor_put('\t');
    monitor_print_str("LFB: ");
    monitor_print_hex(*((u32*) (ptr + 0x28)));

    monitor_put('\n');
    monitor_print_hex(*((u8*) (ptr + 0x1F)));
    monitor_put(' ');
    monitor_print_hex(*((u8*) (ptr + 0x20)));
    monitor_put(' ');

    monitor_put('\n');
    monitor_print_hex(*((u8*) (ptr + 0x21)));
    monitor_put(' ');
    monitor_print_hex(*((u8*) (ptr + 0x22)));

    monitor_put('\n');
    monitor_print_hex(*((u8*) (ptr + 0x23)));
    monitor_put(' ');
    monitor_print_hex(*((u8*) (ptr + 0x24)));
}
