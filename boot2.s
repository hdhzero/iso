    bits 16
    section .text
    extern main

stage2:
    mov ax, 0xB800
    mov es, ax
    mov bx, 0

s2loop:
    mov ax, 0x0137
    mov [es:bx], ax
    inc bx
    inc bx
    cmp bx, 40
    jbe s2loop

set_gdt:
    cli
    pusha
    xor bx, bx
    mov es, bx
    mov bx, 0x8018
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
    dd 0x8000             ; base of GDT

check_data:
    db 0x77
    db 0x88
