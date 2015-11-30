%define WIDTH 1024
%define HEIGHT 768
%define INFO_BASE 0
%define INFO_OFFSET 0x7C00
%define MODE_BASE 0
%define MODE_OFFSET 0x7A00

set_video_mode:
    mov ax, INFO_BASE
    mov es, ax
    mov di, INFO_OFFSET
    mov ax, 0x4F00

    int 0x10
    cmp ax, 0x004F
    jne set_video_mode_failed

    mov ax, 1
    mov [0x0:0x7C00], ax
    ret

set_video_mode_failed:
    mov ax, 0
    mov [0x0:0x7C00], ax
    ret

