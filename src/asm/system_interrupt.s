; setup
; by aov

use32
org 0700h

; code

MAIN_INTERRUPT_HANDLER:

cmp ah,0;al=видеорежим
je HF_SET_VIDEO

cmp ah,1;ds:di-str,bl-color
je HF_PRINTSF

cmp ah,2;ds:di-str,bl-color
je HF_PRINT

;in - ds:di=buffer cx-len buffer
;out - строка в буфере и на экране
cmp ah,3
je HF_GET_STRING

cmp ah,4;-|-in - dh=string, dl=char
je HF_SET_CURSOR

cmp ah,5
je HF_CHAR_CURSOR_INC

cmp ah,6
je HF_CHAR_CURSOR_DEC

cmp ah,7
je HF_NEW_LINE

;-|-in - ds:di=str, cx=len
; |-out - cx=len
cmp ah,08
je HF_GET_LEN_STR

; |-in - ds:di - str1 ds:si - str2
; |-out - al=|=0{1 srting == 2 string}
;            |=1{1 srting != 2 string}
cmp ah,09h
je HF_CMP_STRING


;-|-in - ds:bx=tableFiles, ds:di=flename
; |-out - cl=sector, ah=numSectors
; |-except - not found - ax=0, cx=0 al=2
cmp ah,10h
je HF_SEACH_FILE

;in: dh - golovka
;	 ch - dorogka
;	 cl - sector
;	 al - num sectors
;	 es:bx - куда грузить
cmp ah,11h
je HF_LOAD_FILE

;-|-in - es=segment, bx=metka
; |-out - (load file table on ram)
cmp ah,12h
je HF_LOAD_TABLE_FILES

cmp ah,13h;al-char
je HF_PRINT_CHAR

cmp ah,14h;out = AL-ASCII AH-scancode
je HF_GET_KEY

cmp ah,15h;out= dh,dl - string,char ch,cl=нач.и кончеч строки курсора
je HF_GET_CURSOR



;in: dh - golovka
;	 ch - dorogka
;	 cl - sector
;	 al - num sectors
;	 es:bx - куда грузить
cmp ah,16h
je HF_LOAD_FILE_AND_START
cmp ah,17h;-|-in - ds:si - daps
je HF_LOAD_DAPS

cmp ah,18h;-|-in - ds:si - daps
je HF_SAVE_DAPS


cmp ah,19h;-|-in - ds:si - daps
je HF_START_LOADED_FILE


cmp ah,20h;-|-in - ds:si - daps
je HCOPY_DATA

cmp ah,21h
je HPOWER_OFF

cmp ah,22h
je HGET_PIXEL_COLOR

cmp ah,23h
je HSAVE_SIZE_RUN_PROGRAMM

cmp ah,24h
je HGET_SIZE_RUN_PROGRAMM

HF_SET_VIDEO:
	mov ah,0
	int 10h
iret

HF_PRINTSF:
    ;call F_PRINTSF
iret

HF_PRINT:
    ;call F_PRINT
iret
HF_GET_STRING:
    ;call F_GET_STRING
iret

HF_CHAR_CURSOR_INC:
    ;call F_CHAR_CURSOR_INC
iret

HF_CHAR_CURSOR_DEC:
    ;call F_CHAR_CURSOR_DEC
iret

HF_NEW_LINE:
    ;call F_NEW_LINE
iret

HF_GET_LEN_STR:
    ;call F_GET_LEN_STR
iret

HF_CMP_STRING:
    ;call F_CMP_STRING
iret

HF_SEACH_FILE:
    ;call F_SEACH_FILE
iret

HF_LOAD_FILE:
    ;call F_LOAD_FILE
iret

HF_LOAD_TABLE_FILES:
    ;call F_LOAD_TABLE_FILES
iret

HF_PRINT_CHAR:
	push ax
	mov ah,0fh
    int 10h
	pop ax
	mov ah,0ah
	mov cx,1
	int 10h
iret

HF_SET_CURSOR:
	push dx
	mov ah,0fh
    int 10h
	pop dx
	mov ah,02h
	int 10h
iret

HF_GET_CURSOR:
	mov ah,0fh
    int 10h
    mov ah,03h
    int 10h
iret

HF_GET_KEY:
    xor ax,ax
    int 16h
iret

HF_LOAD_DAPS:
	mov dl,[7C80h]
	mov ah,42h
	int 13h
iret

HF_SAVE_DAPS:
	mov dl,[7C80h]
	mov ah,43h
	int 13h
iret

HCOPY_DATA:;bx - от куда di - куда cx - len
    ;mov bx, TMP
    HCOPY_DATA_start:
        mov al, [bx]
        mov [di],al
        inc di
        inc bx
    loop HCOPY_DATA_start
iret

HPOWER_OFF:
    mov ax,5301h
    xor bx,bx
    int 15h
    mov ax,5308h
    mov bx,0FFFFh
    mov cx,0001h
    int 15h
    mov ax,5307h
    mov bx,0001h
    mov cx,0003h
    int 15h
iret
HF_START_LOADED_FILE:
    ; pusha
    ; push cs
    ; pop ds
    ; mov di, TMP
    ; mov cx, 254
    ; call F_CLEAR_BUFFER
    ; popa
    ; pusha
    ; mov cx,254
    ; mov di,TMP
    ; HF_START_LOADED_FILE_TMP:
    ;     mov al,[bx] 
    ;     mov [di], al
    ;     inc bx
    ;     inc di
    ; loop HF_START_LOADED_FILE_TMP
    ; popa
    ; push TMP
    ; call F_LOAD_DAPS
    ; jmp 0000:07E00h
HGET_PIXEL_COLOR:;si-image_data ;out al-color
;    xor   al,al                ; здесь будет результат  
;    mov   ah,byte[esi]     ; берём цвет(B). читаем сколько синего
;    shr   ah,7                 ; сдвигаем вправо на 7 разрядов
;    and   ah,0001b             ; бит(0) маски отвечает за синий цвет
;    or    al,ah                ; если имеется, то выставляем в маске синий
;    inc   esi                   ; берём цвет(G) (в BMP цвета идут BGR)
;    mov   ah,byte[esi]     ; тоже/самое, только со сдвигом на 1 левее
;    shr   ah,6                 ; сдвигаем вправо на 6 разрядов
;    and   ah,0010b             ; бит(1) маски - зелёный цвет
;    or    al,ah                ; выставляем зеленый
;    inc   esi                   ; берём цвет(R). читаем кол-во красного
;    mov   ah,byte[esi]     ; всё/тоже,
;    shr   ah,5                 ;   ..только со-сдвигом вправо на 5
;    and   ah,0100b             ; бит(2) маски - красный
;    or    al,ah                ; имеется? выставляем красный
;    inc   esi
;    or    al,1000b 
iret
HF_LOAD_FILE_AND_START:
    ; call F_LOAD_FILE
    ; pop ax
    ; pop bx
    ; pop cx
    ; xor ax,ax
    ; xor bx,bx
    ; xor cx,cx
    ; jmp 0000:07E00h
iret

HSAVE_SIZE_RUN_PROGRAMM:
    ; mov di,SIZE_RUN_PROGRAMM
    ; mov [di],bl
iret
HGET_SIZE_RUN_PROGRAMM:
    ; mov di,SIZE_RUN_PROGRAMM
    ; mov bl,[di]
iret


;=============================================================================
; ATA read sectors (LBA mode) 
;
; @param EAX Logical Block Address of sector
; @param CL  Number of sectors to read
; @param RDI The address of buffer to put data obtained from disk
;
; @return None
;=============================================================================
ata_lba_read:
               pushfd
               and eax, 0x0FFFFFFF
               push eax
               push ebx
               push ecx
               push edx
               push edi
 
               mov ebx, eax         ; Save LBA in RBX
 
               mov edx, 0x01F6      ; Port to send drive and bit 24 - 27 of LBA
               shr eax, 24          ; Get bit 24 - 27 in al
               or al, 11100000b     ; Set bit 6 in al for LBA mode
               out dx, al
 
               mov edx, 0x01F2      ; Port to send number of sectors
               mov al, cl           ; Get number of sectors from CL
               out dx, al
 
               mov edx, 0x1F3       ; Port to send bit 0 - 7 of LBA
               mov eax, ebx         ; Get LBA from EBX
               out dx, al
 
               mov edx, 0x1F4       ; Port to send bit 8 - 15 of LBA
               mov eax, ebx         ; Get LBA from EBX
               shr eax, 8           ; Get bit 8 - 15 in AL
               out dx, al
 
 
               mov edx, 0x1F5       ; Port to send bit 16 - 23 of LBA
               mov eax, ebx         ; Get LBA from EBX
               shr eax, 16          ; Get bit 16 - 23 in AL
               out dx, al
 
               mov edx, 0x1F7       ; Command port
               mov al, 0x20         ; Read with retry.
               out dx, al
 
.still_going:  in al, dx
               test al, 8           ; the sector buffer requires servicing.
               jz .still_going      ; until the sector buffer is ready.
 
               mov eax, 256         ; to read 256 words = 1 sector
               xor bx, bx
               mov bl, cl           ; read CL sectors
               mul bx
               mov ecx, eax         ; RCX is counter for INSW
               mov edx, 0x1F0       ; Data port, in and out
               rep insw             ; in to [RDI]
 
               pop edi
               pop edx
               pop ecx
               pop ebx
               pop eax
               popfd
               ret

times(1024-($-0700h)) db 0