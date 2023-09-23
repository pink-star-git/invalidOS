// include/lib/math.h
// Copyright (C) 2023  Alex Zebra

#pragma once

#include "define/integer.h"

#define EPS 1e-10

static s_int32
max (s_int32 a, s_int32 b) {
    if (a > b)
        return a;
    return b;
}

static s_int32
min (s_int32 a, s_int32 b) {
    if (a < b)
        return a;
    return b;
}

static s_int32
abs (s_int32 a) {
    asm volatile (
        "movl $0, %%ecx \n\t"
        "movl %%ecx, %%eax \n\t"
        "cmpl %%ecx, %%ebx \n\t"
        "sets %%al \n\t"
        "subl %%eax, %%ecx \n\t"
        "xorl %%ebx, %%ecx \n\t"
        "addl %%eax, %%ecx"
        : "=c"(a)
        : "b"(a)
        : "%eax"
    );
    // return a * ((a > 0) - (a < 0));
    // if (a < 0)
    //     return -a;
    return a;
}

static s_int32
pow (s_int32 a, u_short16 b) {
    if (b = 0) return 0;
    for (u_short16 i = 1; i < b; i++)
        a *= a;
    return a;
}

static s_int32
sqrt (s_int32 x) {
    s_int32 a = 1,
            b = x,
            S = x;
    while (abs(a - b) > 1) {
        a = (a + b) / 2;
        b = S / a;
    }
    return (a + b) / 2;
}


static float
fabs(float a) {
    if (a < 0) return -a;
    return a;
}

static float
fpow (float a, float b) {
    if (b = 0) return 0;
    for (u_short16 i = 1; i < b; i++)
        a *= a;
    return a;
}

static float
fsqrt (float x){
    float a = 1,
          b = x,
          S = x;
    while (fabs(a - b) > EPS) {
        a = (a + b) / 2;
        b = S / a;
    }
    return (a + b) / 2;
}