// include/lib/ps2_kb.h
// Copyright (C) 2023  Alex Zebra

#pragma once

#include "define/integer.h"

#include "lib/asm.h"
#include "lib/graphics/text/print.h"

#define PS2_CH_WORK    0x00  // Work
#define PS2_CH_ERR     0x01  // Error

#define PS2_STATUS_OUTBUF     0x01
#define PS2_STATUS_INBUF      0x02
#define PS2_STATUS_SYSFLAG    0x04
#define PS2_STATUS_COMDATA    0x08
#define PS2_STATUS_TIMEERR    0x40
#define PS2_STATUS_PARITYERR  0x80

#define PS2_COMMAND_READNRAM       0x20
#define PS2_COMMAND_WRITENRAM      0x60
#define PS2_COMMAND_DISABLE2PORT   0xA7
#define PS2_COMMAND_ENABLE2PORT    0xA8
#define PS2_COMMAND_TEST2PORT      0xA9  // 0x00 test passed 
#define PS2_COMMAND_TESTPS2CONTR   0xAA  // 0x55 test passed 
#define PS2_COMMAND_TEST1PORT      0xAB  // 0x00 test passed 
#define PS2_COMMAND_READALLRAM     0xAC
#define PS2_COMMAND_DISABLE1PORT   0xAD
#define PS2_COMMAND_ENABLE1PORT    0xAE
#define PS2_COMMAND_READINPORT     0xCO
#define PS2_COMMAND_COPYLOWIN      0xC1
#define PS2_COMMAND_COPYHIGHIN     0xC2
#define PS2_COMMAND_READOUTPORT    0xDO
#define PS2_COMMAND_WRITEOUTPORT   0xD1
#define PS2_COMMAND_WRITE1PORTOUT  0xD2
#define PS2_COMMAND_WRITE2PORTOUT  0xD3
#define PS2_COMMAND_WRITE2PORTIN   0xD4

static u_char8 first_channel = 0;
static u_char8 second_channel = 0;
static u_char8 data_byte = 0;

static u_char8 ps2_test (){
    // (I) Disable devices
    asm_::outb(0x64,0xAD);
    asm_::outb(0x64,0xA7);

    // (II) Clear output buffer
    for (u_char8 i = 0; i < 16; i++)
        asm_::inb(0x60);

    // (III) Set the controller configuration byte
    asm_::outb(0x64,0x20);
    data_byte = asm_::inb(0x60);
    second_channel = asm_::testb(data_byte, 5) ? PS2_CH_WORK : PS2_CH_ERR;
    asm_::outb(0x60, data_byte & 0xBC);
    asm_::outb(0x64, 0x60);
    asm_::inb(0x60);

    // (IV) Testing
    asm_::outb(0x64, 0xAA);
    if (asm_::inb(0x60) != 0x55)
        return 1;
    else
        sil::debug::print("Status", "OK", DEBUG_STR);

    asm_::outb(0x64, 0xAB);
    if (asm_::inb(0x60) != 0x00) first_channel = PS2_CH_ERR;

    if (second_channel == 0) {
        asm_::outb(0x64, 0xA9);
        if (asm_::inb(0x60) != 0x00) second_channel = PS2_CH_ERR;
    }

    if (first_channel != 0 && second_channel != 0) return 2;

    // (V)
    for (int i = 0; i < 512 || asm_::inb(0x64); i++){}

    return 0;
}