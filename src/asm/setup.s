; src/asm/setup.s
; Copyright (C) 2023  Alex Zebra

use16
org 0500h
jmp start

    include 'lib/ata.s'

.code:
start:
    use16

; Open gate A20
    in al, 92h
    or al, 2
    out 92h, al

; Disable maskable interrupts
    cli

; Disable non-maskable interrupts
    in al, 70h
    or al, 80h
    out 70h, al

    lgdt [gdtr]

; Switch to protected mode
    mov eax, cr0
    or al, 1
    mov cr0, eax

    jmp far dword 0000000000001000b:pm_entry

    times(512-($-0500h)) db 0

    use32

; Protected mode entry point
pm_entry:
    mov ax, 0000000000010000b
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    ; mov eax, 0x3
    ; mov ecx, 0x10
    ; mov edi, 0x8000
    ; call ata_lba_read
    call 0xA000

    mov edi, 0xB8180
    mov esi, msg

    cld
    .loop:
        lodsb
        test al, al
        jz .exit
        stosb
        mov al, [msg_color]
        stosb
        jmp .loop
    cli

    cld
    .exit:
        mov al, 0x2
        mov [msg_color], al
        jmp $
        hlt

    msg_color db 6

    msg db 'Host is down!', 0

    align 8

    times(512-($-0700h)) db 0

.data:


gdt:
    NULL_SEG_DESCRIPTOR db 8 dup(0)

CODE_SEG_DESCRIPTOR:
    dw 0xFFFF          
    db 0x00, 0x00, 0x00
    db 10011010b       
    db 11001111b       
    db 0x00            
    ; dq 0x00            



DATA_SEG_DESCRIPTOR:
    dw 0xFFFF          
    db 0x00, 0x00, 0x00
    db 10010010b       
    db 11001111b       
    db 0x00            
    ; dq 0x00            

gdt_size equ $ - gdt

gdtr:
    dw gdt_size - 1
    dd gdt 

times(512-($-0900h)) db 0