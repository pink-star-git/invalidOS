// src/kernel/main.cpp
// Copyright (C) 2023  Alex Zebra

#include "define/integer.h"

#include "lib/gcc_fix.h"

#include "lib/mem/mm.h"
#include "lib/math.h"
#include "lib/graphics/text/print.h"
// #include "lib/graphics/GraphicsIO.h"
#include "lib/str.h"
#include "lib/utils.h"
// #include "lib/rom/ide.h"
// #include  "lib/ps2_kb.h"
// #include "lib/driver/VGA.h"


asm("call main\n\t");


void
main (void)
{
    sil::mem::init();

    sil::clear();

//|B B B|B B B B B B|B B B B B B B B B B B
//       E S     * B


    sil::string test = "test";
    sil::print(test);
    // sil::print("test2");


    // sil::print("start", " ");
    // sil::printc("invalid-OS", COLOR::Red);

    // sil::print(utils::dec_2_str(sil::count_memory()) + sil::string(" MB"));

    // sil::print("");

    // sil::print("block size:", " ");
    // sil::print(utils::dec_2_str(memory::block_size));

    // gio.printf_c("test printf_c");

    // VGA_test();

    // ps2_init();


    // sil::vprint(sil::string("test"), sil::string("test2"), sil::string(""));

    // ide_initialize();

    // sil::print("delay");
    // ps2_init();
    // asm_::time::sleep(100);
    // kbd_test();

    // i8042_init();

    // static u_char8 *str = "hello";
    // static u_char8 *mem = (u_char8 *)0x18000;
    // static u_char8 *str1 = "10";
    // static u_char8 *str2 = "100";
    // static u_char8 *str3 = "30";
    // static u_char8 *str4 = "60";
    // static u_char8 *str5 = "140";

    // sil::print("blk addr:", " ");
    // sil::print(utils::hex_2_str((memory::alloc - memory::block_size)));
    // sil::print("blk sign:", " ");
    // sil::print(utils::hex_2_str(((memory::block_header *)(memory::alloc - memory::block_size))->sign_));

    // sil::print("malloc test:", " ");
    // sil::print(utils::hex_2_str(sil::mem::malloc(32)));

    // avl_tree tree;
    // tree.insert(10, str1);
    // tree.insert(140, str5);
    // tree.insert(100, str2);
    // tree.insert(30, str3);
    // tree.insert(60, str4);

    // sil::clear();

    // u_int32 test140;
    // test140 = tree.find_node(140);
    // u_int32 t140 = 140;
    // print(test140 == NULL ? "NULL" : "1");
    // print((u_char8 *)t140);

    // print(tree.find(10));
    // print(tree.find(60));

    // ide_read_sectors(0, 2, 28, 0x10, 0x18000);

    // print(hex_2_str(ps2_test()).data);
    
    // while (true)
    // {
    //     /* code */
    // }
    
}