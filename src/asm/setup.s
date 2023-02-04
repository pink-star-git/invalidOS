; setup
; by zebra

use16
org 0500h

; code

xor ax,ax
push ax
pop es
mov al,90h
mov bl,4h
mul bl
mov bx,ax
mov si,0700h
mov [es:bx],si
add bx,2
push cs
pop ax
mov [es:bx], ax

push cs
pop ds
mov si,paket_sysint
mov ah,42h
int 13h

aboba:
mov ah,14h
int 90h
mov ah,13h
int 90h
jmp aboba

push cs
pop ds
mov si,paket_kernel
mov ah,42h
int 13h
jmp 0000:8000h

jmp $

times(512-($-0500h)) db 0

;data
paket_kernel:
    dw 16;const paksize
    dw 8;num sectors
    dw 8000h;offset
    dw 0;segment
    dq 5;start sector

paket_sysint:
    dw 16;const paksize
    dw 2;num sectors
    dw 700h;offset
    dw 0;segment
    dq 3;start sector

times(512-($-0700h)) db 0