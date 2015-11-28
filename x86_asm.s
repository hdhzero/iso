    section .text
    global x86_outb
    global x86_inb
    global x86_inw
    global x86_idt_flush
    global x86_int
    global asm_isr0

    extern isr0

x86_outb:
    mov eax, [esp + 8]
    mov edx, [esp + 4]
    out dx, al
    ret

x86_inb:
    xor eax, eax
    mov edx, [esp + 4]
    in al, dx
    ret

x86_inw:
    xor eax, eax
    mov edx, [esp + 4]
    in ax, dx
    ret

x86_idt_flush:
    mov eax, [esp + 4]
    lidt [eax]
    ret

x86_int:
    int 0x0
    ret

asm_isr0:
    push eax
    mov eax, 0x1234ABCD
    ; save registers edi,esi,ebp,esp,ebx,edx,ecx,eax
    pushad

    ; at this moment, this is our current stack
    ; 28: eax
    ; 24: ecx
    ; 20: edx
    ; 16: ebx
    ; 12: esp
    ;  8: ebp
    ;  4: esi
    ;  0: edi

    ; now we pass a pointer to our registers that are in the stack
    push esp

    ; and finally call our high level interrupt routine
    call isr0

    ; take out our pointer (esp) from the stack
    pop eax

    ; take out the registers pushed by pushad
    popad 
    pop eax

    ; return from interruption
    iret
