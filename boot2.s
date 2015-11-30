    bits 16
    section .text
    extern main
    global stage2

stage2:
    call enable_a20
    call set_video_mode
    mov ax, 0x4f02
    mov bx, 0x0123
    int 0x10
;    jmp set_gdt

set_gdt:
    cli
    pusha
    xor bx, bx
    mov es, bx
;    mov bx, 0x8018
    mov bx, toc
    lgdt [es:bx]
    popa

go_into_pm:
    cli
    mov eax, cr0
    or eax, 1
    mov cr0, eax
    mov eax, 0x10
    mov ds, eax
    mov ss, eax
    mov es, eax
    mov fs, eax
    mov gs, eax
    mov esp, 0x70000
;    call 0x08:0x8200 ; call main
    call 0x08:main ; call main

; Offset 0 in GDT: Descriptor code=0

enable_a20:
    mov ax, 0x2401
    int 0x15
    mov ax, 0x2402
    cmp al, 0
    je enable_a20
    ret
    
%define WIDTH 1024
%define HEIGHT 768
%define BITS_PER_PIXEL 32
%define INFO_BASE 0
%define INFO_OFFSET 0x7C00
%define MODE_BASE 0
%define MODE_OFFSET 0x7A00

set_video_mode:
    mov ax, INFO_BASE
    mov es, ax
    mov di, INFO_OFFSET
    mov al, 'V'
    mov [es:di + 0], al
    mov al, 'B'
    mov [es:di + 1], al
    mov al, 'E'
    mov [es:di + 2], al
    mov al, '2'
    mov [es:di + 3], al
    mov ax, 0x4F00

    int 0x10
    cmp ax, 0x004F
    jne set_video_mode_failed

    xor ax, ax
    push ax

check_video_modes:
    mov ax, INFO_BASE
    mov es, ax
    mov di, INFO_OFFSET

    mov bx, [es:di + 0xE]
    mov cx, [es:di + 0xE + 2]
    mov di, bx
    mov es, cx
    pop ax
    add di, ax
    add ax, 2
    push ax

    mov cx, [es:di]
    cmp cx, 0xFFFF 
    je set_video_mode_failed

    mov ax, MODE_BASE
    mov es, ax
    mov di, MODE_OFFSET
    mov ax, 0x4F01
    int 0x10

    cmp ax, 0x004F
    jne check_video_modes

    call is_requested_video_mode
    cmp ax, 0
    je check_video_modes
    push ax
    mov ax, INFO_BASE
    mov es, ax
    mov di, INFO_OFFSET
    pop ax
    mov [es:di], ax
    mov bx, ax
    mov ax, 0x4F02
    int 0x10
    pop ax
    ret

set_video_mode_failed:
    mov ax, INFO_BASE
    mov es, ax
    mov di, INFO_OFFSET
    mov ax, 0
    mov [es:di], ax
    pop ax
    ret

is_requested_video_mode:
    mov ax, MODE_BASE
    mov es, ax
    mov di, MODE_OFFSET

    mov ax, [es:di + 0x12]
    cmp ax, WIDTH
    jne is_not_mode

    mov ax, [es:di + 0x14]
    cmp ax, HEIGHT
    jne is_not_mode

    mov al, [es:di + 0x19]
    cmp al, BITS_PER_PIXEL
    jne is_not_mode

    mov ax, cx
    ret

is_not_mode:
    xor ax, ax
    ret

    section .data 
gdt_data: 
    dd 0                ; null descriptor
    dd 0 
 
; Offset 0x8 bytes from start of GDT: Descriptor code therfore is 8
 
; gdt code:             ; code descriptor
    dw 0xFFFF           ; limit low
    dw 0                ; base low
    db 0                ; base middle
    db 10011010b            ; access
    db 11001111b            ; granularity
    db 0                ; base high
 
; Offset 16 bytes (0x10) from start of GDT. Descriptor code therfore is 0x10.
 
; gdt data:             ; data descriptor
    dw 0xFFFF           ; limit low (Same as code)
    dw 0                ; base low
    db 0                ; base middle
    db 10010010b            ; access
    db 11001111b            ; granularity
    db 0                ; base high
 
;...Other descriptors begin at offset 0x18. Remember that each descriptor is 8 bytes in size?
; Add other descriptors for Ring 3 applications, stack, whatever here...
 
end_of_gdt:
toc: 
    dw 23 
;    dd 0x8000             ; base of GDT
    dd gdt_data

check_data:
    db 0x77
    db 0x88
