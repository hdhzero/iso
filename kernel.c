#include "monitor.h"
#include "descriptors.h"
#include "x86_asm.h"

unsigned char msg2[] = "Hello, World from .data";

void main() {
    unsigned char msg[] = "Hello, World from stack!";
    
    init_monitor();
    monitor_clear_with_bg(1);


    monitor_print_str("Hello, world! Again muahahaha 123777");
    monitor_print_str("This is a testing screen msg\nOk!\n\nDone...");
    monitor_print_hex(0x123400FF);
    monitor_put('\n');
    monitor_print_int(-7);
    monitor_put('\n');
    monitor_print_dec(7);

    set_initial_idt();
    asm volatile ("int $0xf7");
 //   x86_int();
    
    while (1) { }
}
