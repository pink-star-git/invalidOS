use16               ; generate 16-bit code
org 7C00h           ; the code starts at 0x7C00 memory address

start:
    jmp far dword 0x0000:entr ; makes CS=0, IP=0x7c00
entr:
    xor  ax, ax     ; ax = 0
    mov  ds, ax     ; setup data segment ds=ax=0
    cli             ; when we set up stack we need disable interrupts because stack is involved in interrupts handling
    mov  ss, ax     ; setup stack segment ss=ax=0
    mov  sp, 0x7C00 ; stack will grow starting from 0x7C00 memory address
    sti             ; enable interrupts
   
    ; Clear screen
    mov ax, 0x0003  ; ah=0 means clear screen and setup video mode, al=3 means text mode, 80x25 screen, CGA/EGA adapter, etc.
    int 0x10        ; call BIOS standard video service function
   
    ; Open gate A20 through the System Control Port A
    in  al, 0x92    ; read the content of port 0x92 (System Control Port A) into al
    or  al, 0x02    ; set 1st bit in al to 1
    out 0x92, al    ; write al into port 0x92
   
    ; Disable maskable interrupts
    cli ; clear flag IF in EFLAGS register
   
    ; Disable non-maskable interrupts
    in  al, 0x70
    or  al, 0x80
    out 0x70, al
   
    lgdt [gdtr]     ; load the address and size of Global Descriptor Table (GDT) into GDTR register
   
    ; Switch to protected mode
    mov  eax, cr0   ; read the content of register cr0 (Machine Status Word - MSW) into eax
    or   al,  0x01  ; set 0th bit to 1 (0th bit of cr0 is called PE - Protection Enable)
    mov  cr0, eax   ; write eax to cr0

    ; Load protected mode entry point into CS:EIP
    jmp far dword 0000000000001000b:pm_entry ; 0000000000001000b is a segment selector which is loaded into CS register
    ; Segment selector's format:
    ;  [0:1]  RPL              = 00b            - requested privilege level = 0 (most privileged)
    ;      2  TI               = 0              - chooses descriptor table; 0 means Global Descriptor Table
    ; [3:15]  Descriptor Index = 0000000000001b - index of descriptor inside the descriptor table = 1
; ========================================== PROTECTED MODE ====================================================
use32               ; generate 32-bit code

; Protected mode entry point
pm_entry:
    ; Initialize segment registers (except CS which is already litialized)
    mov ax, 0000000000010000b ; segment selector: RPL = 0, TI = 0, Descriptor Index = 2
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    mov edi, 0xB8000            ; 0xB8000 is the beginning of video-memory in 0x3 video-mode
    mov esi, message            ; the message which is going to be printed on the screen
    cld                         ; clear direction flag (DF is a flag used for string operations)

; Message-printing loop
.loop:                          
    lodsb                       ; load to al the next charachter located at [esi] address in memory (si is incremented automatically because the direction flag DF = 0)
    test al, al                 ; test al against zero
    jz .exit                    ; exit if al is zero
    stosb                       ; otherwise move al to memory at address [edi]
    mov al, 7                   ; 7 is the so-called attribute of the symbol
    stosb                       ; move al to memory at address [edi]
    jmp .loop

.exit:
    cli                         ; disable interrupts before halting the processor
    hlt                         ; halt the processor

message db 'Hello World!', 0

align 8

gdt:
    ; Segment descriptor format
    ; BITS  | SIZE | FIELD
    ; ------+------+------
    ; 0-15  |  16  | Limit[0:15]
    ; 16-39 |  24  | Base[0:23]
    ; 40-47 |   8  | P DPL[0:1] S Type[0:3]
    ; 48-55 |   8  | G D/B L AVL Limit[16:19]
    ; 56-63 |   8  | Base[24:31]

NULL_SEG_DESCRIPTOR db 8 dup(0)
   
CODE_SEG_DESCRIPTOR:
    dw 0xFFFF           ; Limit[0:15]
    db 0x00, 0x00, 0x00 ; Base[0:23]
    db 10011010b        ; P DPL[0:1] S Type[0:3]
    db 11001111b        ; G D/B L AVL Limit[16:19]
    db 0x00             ; Base[24:31]

    ; Detailed description of the segment descriptor:
    ; Base  = 0x00000000 - segment base address = 0
    ; Limit = 0xFFFFF    - segment size = 2^20
    ; P     = 1          - presence: segment is present in physical memory
    ; DPL   = 00b        - descriptor privilege level = 0 (most privileged)
    ; S     = 1          - system (TSS segment): segment is not a system segment
    ; Type  = 1010b      - code segment (1), C=0 R=1 A=0 execution and reading allowed
    ; G     = 1          - granularity: the size of the segment is measured in 4 kilobyte pages, i. e. it's equal to 2^20*4 KiB = 4 GiB
    ; D/B   = 1          - default size: operands and addresses are 32-bit wide
    ; L     = 0          - 64-bit code segment: in protected mode this bit is always zero
    ; AVL   = 0          - available: it's up to the programmer how to use this bit

DATA_SEG_DESCRIPTOR:
    dw 0xFFFF           ; Limit[0:15]
    db 0x00, 0x00, 0x00 ; Base[0:23]
    db 10010010b        ; P DPL[0:1] S Type[0:3]
    db 11001111b        ; G D/B L AVL Limit[16:19]
    db 0x00             ; Base[24:31]

    ; Detailed description of the segment descriptor:
    ; Base  = 0x00000000 - segment base address = 0
    ; Limit = 0xFFFFF    - segment size = 2^20
    ; P     = 1          - presence: segment is present in physical memory
    ; DPL   = 00b        - descriptor privilege level = 0 (most privileged)
    ; S     = 1          - system (TSS segment): segment is not a system segment
    ; Type  = 0010b      - data segment (0), E=0 W=1 A=0 reading and writing are allowed, expand-up data segment (offset ranges from 0 to Limit)
    ; G     = 1          - granularity: the size of the segment is measured in 4 kilobyte pages, i. e. it's equal to 2^20*4 KiB = 4 GiB
    ; D/B   = 1          - default size: stack pointer is 32-bit wide (concerns stack segment) and the upper bound of the segment is 4 GiB (concerns data segment)
    ; L     = 0          - 64-bit code segment: in protected mode this bit is always zero
    ; AVL   = 0          - available: it's up to the programmer how to use this bit

gdt_size equ $ - gdt

; data to be loaded to GDTR register
gdtr:
    dw gdt_size - 1         ; 16-bit limit of the global descriptor table
    dd gdt                  ; 32-bit base address of the global descriptor table

; ======================================== BOOT SECTOR SIGNATURE ===============================================
finish:
    ; The size of a disk sector is 512 bytes. Boot sector signature occupies the two last bytes.
    ; The gap between the end of the source code and the boot sector signature is filled with zeroes.
    times 510-finish+start db 0
    db 55h, 0AAh    ; boot sector signature