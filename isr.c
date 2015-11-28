#include "isr.h"
#include "monitor.h"

void isr0(u32* regs) {
    int i;

    char* regs_str[] = {
        "EDI: ", "ESI: ", "EBP: ", "ESP: ", 
	"EBX: ", "EDX: ", "ECX: ", "EAX: "
    };

    monitor_print_str("Receiveid interrupt: \n");

    for (i = 0; i < 8; i += 2) {
        monitor_print_str(regs_str[i]);
        monitor_print_hex(*(regs + i));
        monitor_put(' ');
        monitor_print_str(regs_str[i + 1]);
        monitor_print_hex(*(regs + i + 1));
        monitor_put('\n');
    }
}
