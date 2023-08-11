// include/lib/ps2_kb.h
// Copyright (C) 2023  Alex Zebra

#pragma once

#include "define/integer.h"

#include "lib/asm.h"
#include "lib/graphics/text/text.h"

#define PS2_CH_WORK    0x00   // Work
#define PS2_CH_ERR     0x01   // Error

static u_char8 first_channel = 0;
static u_char8 second_channel = 0;
static u_char8 data_byte = 0;

static u_char8 ps2_test (){
    // (I) Disable devices
    outb(0x64,0xAD);
    outb(0x64,0xA7);

    // (II) Clear output buffer
    for (u_char8 i = 0; i < 16; i++)
        inb(0x60);

    // (III) Set the controller configuration byte
    outb(0x64,0x20);
    data_byte = inb(0x60);
    second_channel = testb(data_byte, 5) ? PS2_CH_WORK : PS2_CH_ERR;
    outb(0x60, data_byte & 0xBC);
    outb(0x64, 0x60);
    inb(0x60);

    // (IV) Testing
    outb(0x64, 0xAA);
    if (inb(0x60) != 0x55) return 1;

    outb(0x64, 0xAB);
    if (inb(0x60) != 0x00) first_channel = PS2_CH_ERR;

    if (second_channel == 0) {
        outb(0x64, 0xA9);
        if (inb(0x60) != 0x00) second_channel = PS2_CH_ERR;
    }

    if (first_channel != 0 && second_channel != 0) return 2;

    // (V)
    for (int i = 0; i < 512 || inb(0x64); i++){}

    return 0;
}