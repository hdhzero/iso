#include "common.h"

void kmemset(u8* ptr, u8 value, int n) {
    u8* end = ptr + n;

    while (ptr != end) {
        *ptr = value;
        ++ptr;
    }
}
