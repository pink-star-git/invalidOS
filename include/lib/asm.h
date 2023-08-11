// include/lib/asm.h
// Copyright (C) 2023  Alex Zebra

#pragma once

#include "define/integer.h"

// out
static inline void
outb (u_short16 port, u_char8 val) {
    asm volatile (
                    "outb %0, %1"
                    :
                    : "a"(val), "Nd"(port)
                );
}

static inline void
outw (u_short16 port, u_short16 val) {
    asm volatile (
                    "outw %0, %1"
                    :
                    : "a"(val), "Nd"(port)
                );
}

static inline void
outl (u_short16 port, u_int32 val) {
    asm volatile (
                    "outl %0, %1"
                    :
                    : "a"(val), "Nd"(port)
                );
}

// in
static inline u_char8
inb (u_short16 port) {
    u_char8 ret;
    asm volatile (
                    "inb %1, %0"
                    : "=a"(ret)
                    : "Nd"(port)
                );
    return ret;
}

static inline u_short16
inw (u_short16 port) {
    u_short16 ret;
    asm volatile (
                    "inw %1, %0"
                    : "=a"(ret)
                    : "Nd"(port)
                );
    return ret;
}

static inline u_int32
inl (u_short16 port) {
    u_int32 ret;
    asm volatile (
                    "inl %1, %0"
                    : "=a"(ret)
                    : "Nd"(port)
                );
    return ret;
}

// ins
static void
insb (u_short16 reg, u_char8 *buffer, u_int32 quads) {
    u_int32 index;
    for(index = 0; index < quads; index++) {
        buffer[index] = inb(reg);
    }
}

static void
insw (u_short16 reg, u_short16 *buffer, u_int32 quads) {
    u_int32 index;
    for(index = 0; index < quads; index++) {
        buffer[index] = inw(reg);
    }
}

static void
insl (u_short16 reg, u_int32 *buffer, u_int32 quads) {
    u_int32 index;
    for(index = 0; index < quads; index++) {
        buffer[index] = inl(reg);
    }
}

static bool
testb (u_char8 byte, u_char8 index) {
    return (byte >> index) & 1;
}

static void
sleep (u_int32 millis) {
    if (!millis)
        return;
    while (millis*10 > 0) {
        asm volatile (
            "nop"
        );
        millis--;
    }
}