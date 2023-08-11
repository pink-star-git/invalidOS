// src/kernel/main.cpp
// Copyright (C) 2023  Alex Zebra

#pragma once

#include "define/integer.h"
#include "define/color.h"

#include "lib/graphics/text/text.h"
#include "lib/str.h"
#include "lib/rom/ide.h"
#include "lib/ps2_kb.h"

asm("call main\n\t");


void
main (void)
{
    static u_char8 i = 20;
    static u_char8 *str = "hello";
    static u_char8 *mem = (u_char8 *)0x18000;
    static float t = 3.9f;

    s_f_string8 world = "world";

    s_f_string8 world_wd = world + s_f_string8("_wd");
    print(world_wd.data);

    clear();
    print("str", '\n', C_Green);
    print("aboba");
    print(str);
    // print(dec_2_str((u_char8)t).data, '\n', C_Turquoise);
    print(float_2_str(t).data, '\n', C_Turquoise);
    print(" ");

    // print_char(0x35, 0x2, 8, 8, 80);
    ide_initialize();
    ide_read_sectors(0, 2, 28, 0x10, 0x18000);

    print(hex_2_str(ps2_test()).data);
    print("test");
}