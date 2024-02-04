// include/lib/ps2_kb.h
// Copyright (C) 2023  Alex Zebra

#pragma once

#include "define/integer.h"

#include "lib/asm.h"
#include "lib/graphics/text/print.h"

#define PS2_PORT_WORK    0x00  // Work
#define PS2_PORT_ERR     0x01  // Error

#define PS2_PORT_DATA     0x60
#define PS2_PORT_COMMAND  0x64
#define PS2_PORT_STATUS   0x64

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

#define PS2_CONFIG_INT1PORT     0x01
#define PS2_CONFIG_INT2PORT     0x02
#define PS2_CONFIG_SYSFLAG      0x04
// #define PS2_CONFIG_ZERO      0x08
#define PS2_CONFIG_CLOCK1PORT   0x10
#define PS2_CONFIG_CLOCK2PORT   0x20
#define PS2_CONFIG_TRANS1PORT   0x40
// #define PS2_CONFIG_ZERO      0x80


static u_char8 first_channel = 0;
static u_char8 second_channel = 0;
static u_char8 data_byte = 0;
static u_char8 config_byte = 0;


static void
ps2_clear_buffer() {
    for (u_char8 i = 0; i < 16 && asm_::inb(PS2_PORT_STATUS) & PS2_STATUS_OUTBUF; i++) {
        asm_::inb(PS2_PORT_DATA);
        asm_::time::sleep(10);
    }
}

static u_char8
ps2_init () {
    // (I) Disable devices (3)
    asm_::outb(PS2_PORT_COMMAND,PS2_COMMAND_DISABLE1PORT);
    asm_::outb(PS2_PORT_COMMAND,PS2_COMMAND_DISABLE2PORT);


    // (II) Clear output buffer (4)
    ps2_clear_buffer();



    // (III) Set the controller configuration byte (5)
    asm_::outb(PS2_PORT_COMMAND,PS2_COMMAND_READNRAM);
    config_byte = asm_::inb(PS2_PORT_DATA);
    second_channel = (config_byte & PS2_CONFIG_CLOCK2PORT) ? PS2_PORT_WORK : PS2_PORT_ERR;
    config_byte = config_byte & ~PS2_CONFIG_INT1PORT
                              & ~PS2_CONFIG_INT2PORT
                              & ~PS2_CONFIG_TRANS1PORT;
    asm_::outb(PS2_PORT_DATA, config_byte);
    asm_::outb(PS2_PORT_COMMAND, PS2_COMMAND_WRITENRAM);
    sil::print("Second port", second_channel ? "Unsupport" : "Support");
    asm_::inb(PS2_PORT_DATA);


    // (IV) Testing (6)
    asm_::outb(PS2_PORT_COMMAND, PS2_COMMAND_TESTPS2CONTR);
    if (asm_::inb(PS2_PORT_DATA) != 0x55) {
        sil::print("Controller PS2", "ERORR");
        return 1;
    }
    else
        sil::print("Controller PS2", "OK");

    asm_::outb(PS2_PORT_DATA, config_byte);
    asm_::outb(PS2_PORT_COMMAND, PS2_COMMAND_WRITENRAM);

    if (first_channel == 0) {
        asm_::outb(PS2_PORT_COMMAND, PS2_COMMAND_TEST1PORT);
        if (asm_::inb(PS2_PORT_DATA) != 0x00) {
            first_channel = PS2_PORT_ERR;
            sil::print("First channel", "ERORR");
        }
        sil::print("First channel", "OK");
    }
    if (second_channel == 0) {
        asm_::outb(PS2_PORT_COMMAND, PS2_COMMAND_TEST2PORT);
        if (asm_::inb(PS2_PORT_DATA) != 0x00) {
            second_channel = PS2_PORT_ERR;
            sil::print("Second channel", "ERORR");
        }
        sil::print("Second channel", "OK");
    }
    if (first_channel != 0 && second_channel != 0) return 2;

    // (V)
    asm_::outb(PS2_PORT_COMMAND, PS2_COMMAND_ENABLE1PORT);

    sil::print("PS2 init", "DONE");
    return 0;
}

void kbd_test () {
    sil::print("KBD test", "start");
    u_short16 device_status;

    ps2_clear_buffer();
    asm_::outb(PS2_PORT_DATA, 0xFF);
    while (~asm_::inb(PS2_PORT_STATUS) & PS2_STATUS_OUTBUF);
    device_status = asm_::inb(PS2_PORT_DATA);
    sil::print("Device status", utils::hex_2_str(device_status));
    // printd("Device status", device_status == 0xFA ? "Work" : "Erorr", DEBUG::STR);
}