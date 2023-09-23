// src/kernel/main.cpp
// Copyright (C) 2023  Alex Zebra

#include "define/integer.h"
#include "define/color.h"

#include "lib/math.h"
#include "lib/graphics/text/text.h"
#include "lib/str.h"
#include "lib/rom/ide.h"
// #include "lib/ps2_kb.h"
#include "lib/avl_tree.h"

asm("call main\n\t");


void
main (void)
{
    static u_char8 *str = "hello";
    static u_char8 *mem = (u_char8 *)0x18000;
    static u_char8 *str1 = "10";
    static u_char8 *str2 = "100";
    static u_char8 *str3 = "30";
    static u_char8 *str4 = "60";
    static u_char8 *str5 = "140";


    avl_tree tree;
    // tree.insert(10, str1);
    // tree.insert(140, str5);
    // tree.insert(100, str2);
    // tree.insert(30, str3);
    // tree.insert(60, str4);

    clear();

    print(str);
    u_int32 test140;
    test140 = tree.find_node(140);
    u_int32 t140 = 140;
    print(test140 == NULL ? "NULL" : "1");
    // print((u_char8 *)t140);

    // print(tree.find(10));
    // print(tree.find(60));

    // ide_initialize();
    // ide_read_sectors(0, 2, 28, 0x10, 0x18000);

    // print(hex_2_str(ps2_test()).data);
    print("test");
    // while (true)
    // {
    //     /* code */
    // }
    
}