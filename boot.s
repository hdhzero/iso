    global mystart
    extern stage2

    bits 16
    section .text

mystart:
    mov ax, 0x07C0
    mov ds, ax
    xor ax, ax
    mov es, ax

; ah = 2
; al = qtd sectors
; ch = track (0 to ...)
; cl = sector (1 to 18)
; dh = head (0 or 1)
; dl = drive 0 (always 0?)
; es:bx = buffer

; add 576 to es
track0:
    mov ah, 0
    int 0x13
    jc track0
    mov ax, 0x0201
    mov cx, 0x0002
    mov dx, 0x0000
    mov bx, 0x7E00
    int 0x13
    jc track0

track1:
    mov ah, 0
    int 0x13
    jc track1
    mov ax, 0x0201
    mov cx, 0x0003
    mov dx, 0x0000
    mov bx, 0x8000
    int 0x13
    jc track1

track2:
    mov ah, 0
    int 0x13
    jc track2
    mov ax, 0x0201
    mov cx, 0x0004
    mov dx, 0x0000
    mov bx, 0x8200
    int 0x13
    jc track2

track3:
    mov ah, 0
    int 0x13
    jc track3
    mov ax, 0x0201
    mov cx, 0x0005
    mov dx, 0x0000
    mov bx, 0x8400
    int 0x13
    jc track3

track4:
    mov ah, 0
    int 0x13
    jc track4
    mov ax, 0x0201
    mov cx, 0x0006
    mov dx, 0x0000
    mov bx, 0x8600
    int 0x13
    jc track4

track5:
    mov ah, 0
    int 0x13
    jc track5
    mov ax, 0x0201
    mov cx, 0x0007
    mov dx, 0x0000
    mov bx, 0x8800
    int 0x13
    jc track5

track6:
    mov ah, 0
    int 0x13
    jc track6
    mov ax, 0x0201
    mov cx, 0x0008
    mov dx, 0x0000
    mov bx, 0x8A00
    int 0x13
    jc track6

track7:
    mov ah, 0
    int 0x13
    jc track7
    mov ax, 0x0201
    mov cx, 0x0009
    mov dx, 0x0000
    mov bx, 0x8C00
    int 0x13
    jc track7

go_to_stage2:
;    jmp 0x0000:0x7E00
    jmp 0x0000:stage2


