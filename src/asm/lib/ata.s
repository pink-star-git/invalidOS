use32
;=============================================================================
; ATA read sectors (LBA mode) 
;
; @param EAX Logical Block Address of sector
; @param CL  Number of sectors to read
; @param EDI The address of buffer to put data obtained from disk
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

    mov ebx, eax         ; Save LBA in EBX

    mov edx, 0x01F6      ; Port to send drive and bit 24 - 27 of LBA
    shr eax, 24          ; Get bit 24 - 27 in al
    or al, 11100000b     ; Set bit 6 in al for LBA mode
    out dx, al

    mov edx, 0x01F2      ; Port to send number of sectors
    mov al, cl           ; Get number of sectors from CL
    out dx, al

    mov edx, 0x01F3       ; Port to send bit 0 - 7 of LBA
    mov eax, ebx         ; Get LBA from EBX
    out dx, al

    mov edx, 0x01F4       ; Port to send bit 8 - 15 of LBA
    mov eax, ebx         ; Get LBA from EBX
    shr eax, 8           ; Get bit 8 - 15 in AL
    out dx, al

    mov edx, 0x01F5       ; Port to send bit 16 - 23 of LBA
    mov eax, ebx         ; Get LBA from EBX
    shr eax, 16          ; Get bit 16 - 23 in AL
    out dx, al

    mov edx, 0x01F7       ; Command port
    mov al, 0x20         ; Read with retry.
    out dx, al

    cld
    .read_still_going:
        in al, dx
        test al, 8           ; the sector buffer requires servicing.
        jz .read_still_going      ; until the sector buffer is ready.
    cli

    mov eax, 256         ; to read 256 words = 1 sector
    xor bx, bx
    mov bl, cl           ; read CL sectors
    mul bx
    mov ecx, eax         ; ECX is counter for INSW
    mov edx, 0x01F0       ; Data port, in and out
    rep insw             ; in to [EDI]

    pop edi
    pop edx
    pop ecx
    pop ebx
    pop eax
    popfd
    ret

;=============================================================================
; ATA write sectors (LBA mode) 
;
; @param EAX Logical Block Address of sector
; @param CL  Number of sectors to write
; @param RDI The address of data to write to the disk
;
; @return None
;=============================================================================
 
ata_lba_write:
    pushfd
    and eax, 0x0FFFFFFF
    push eax
    push ebx
    push ecx
    push edx
    push edi
 
    mov ebx, eax         ; Save LBA in EBX
 
    mov edx, 0x01F6      ; Port to send drive and bit 24 - 27 of LBA
    shr eax, 24          ; Get bit 24 - 27 in al
    or al, 11100000b     ; Set bit 6 in al for LBA mode
    out dx, al
 
    mov edx, 0x01F2      ; Port to send number of sectors
    mov al, cl           ; Get number of sectors from CL
    out dx, al
 
    mov edx, 0x01F3       ; Port to send bit 0 - 7 of LBA
    mov eax, ebx         ; Get LBA from EBX
    out dx, al
 
    mov edx, 0x01F4       ; Port to send bit 8 - 15 of LBA
    mov eax, ebx         ; Get LBA from EBX
    shr eax, 8           ; Get bit 8 - 15 in AL
    out dx, al
 
 
    mov edx, 0x01F5       ; Port to send bit 16 - 23 of LBA
    mov eax, ebx         ; Get LBA from EBX
    shr eax, 16          ; Get bit 16 - 23 in AL
    out dx, al
 
    mov edx, 0x01F7       ; Command port
    mov al, 0x30         ; Write with retry.
    out dx, al
 
    cld
    .write_still_going:
        in al, dx
        test al, 8           ; the sector buffer requires servicing.
        jz .write_still_going      ; until the sector buffer is ready.
    cli
 
    mov eax, 256         ; to read 256 words = 1 sector
    xor bx, bx
    mov bl, cl           ; write CL sectors
    mul bx
    mov ecx, eax         ; ECX is counter for OUTSW
    mov edx, 0x01F0       ; Data port, in and out
    mov esi, edi
    rep outsw            ; out
 
    pop edi
    pop edx
    pop ecx
    pop ebx
    pop eax
    popfd
    ret