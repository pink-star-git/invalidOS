; src/asm/boot.s
; Copyright (C) 2023  Alex Zebra

    use16
    org 7C00h

boot_start:
    cli             ;запрещаем прерывания
    xor ax,ax       ;обнуляем регистр ах
    mov ds,ax       ;настраиваем сегмент данных на нулевой адрес
    mov es,ax       ;настраиваем сегмент es на нулевой адрес
    mov ss,ax       ;настраиваем сегмент стека на нулевой адрес
    mov sp,09400h   ;сегмент sp указывает на текущую вершину стека
    sti             ;разрешаем прерывания

boot_disk:
    mov [disk_id],dl

    push cs
    pop ds
    mov si,paket_setup
    mov ah,42h
    int 13h

    mov si,paket_tmp_main_1
    mov ah,42h
    int 13h

    ; mov si,paket_tmp_main_2
    ; mov ah,42h
    ; int 13h


    jmp 0000:0500h


    jmp $

paket_setup:
    dw 16;const paksize
    dw 2;num sectors
    dw 0500h;offset
    dw 0;segment
    dq 1;start sector

paket_tmp_main_1:
    dw 16;const paksize
    dw 20;num sectors
    dw 0000h;offset
    dw 0A00h;segment
    dq 3;start sector

; paket_tmp_main_2:
;     dw 16;const paksize
;     dw 6;num sectors
;     dw 0C00h;offset
;     dw 1000h;segment
;     dq 9;start sector

times(128-($-07C00h)) db 0

disk_id db 0h,0h

times(512-2-($-07C00h)) db 0

    db 055h,0AAh
;16 байт 1 сегмент