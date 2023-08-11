; src/asm/test.s
; Copyright (C) 2023  Alex Zebra

use32
org 0900h
.code:
    mov edi, 0xB8000            
    mov esi, message            

.loop:                          
    lodsb                       
    test al, al                 
    jz .exit                    
    stosb                       
    mov al, 7                   
    stosb                       
    jmp .loop

.exit:
    ret                         

message db 'Hello Test Test!', 0
times(512-($-0900h)) db 0