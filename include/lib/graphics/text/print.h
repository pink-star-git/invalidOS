// include/lib/garphics/text/text.h
// Copyright (C) 2023  Alex Zebra

#pragma once

#include "define/integer.h"
#include "lib/graphics/graphic/display.h"
#include "lib/str.h"
#include "lib/utils.h"


namespace sil { // standart invalid library

    void clear() {
        for(int i = 0; i < 30*100; ++i) DISPLAY.set_char(0);
        DISPLAY.set_cursor_x(0);
        DISPLAY.set_cursor_y(0);
    }

    void print(const string& str, bool new_line = true) {
        for(int i = 0; i < str.size(); ++i) DISPLAY.set_char(str[i]);
        if (new_line) {
            DISPLAY.set_cursor_x(0);
            DISPLAY.set_cursor_y(DISPLAY.get_cursor_y() + 1);
        }
        else
            DISPLAY.set_cursor_x(DISPLAY.get_cursor_x() + 1);
    }

    namespace debug {
        #define DEBUG_CHR 0x00
        #define DEBUG_STR 0x01
        #define DEBUG_DEC 0x02
        #define DEBUG_HEX 0x03
        #define DEBUG_BIN 0x04

        void print(const string& name, auto data, u_char8 data_type) {
            print(name+sil::string(":"), false);
            if (!(data_type ^ DEBUG_CHR))
                DISPLAY.set_char(data);
            else if (!(data_type ^ DEBUG_STR))
                print(sil::string(data));
            else if (!(data_type ^ DEBUG_DEC))
                print(utils::dec_2_str(data));
            else if (!(data_type ^ DEBUG_HEX))
                print(utils::hex_2_str(data));
            else if (!(data_type ^ DEBUG_BIN))
                print(utils::bin_2_str(data));
        }
    } // namespace debug
    


    // class Print
    // {
    //   public:
    //     Print();

    //     Print(const print&) = delete;
    //     Print(print&&) = delete;

    //     void print(const string& str)
    //     {
    //         for(int i = 0; i < str.size(); ++i) DISPLAY.set_char(str[i]);
    //     }

    //     ~Print();
    //   private:
    // };

} // namespace sil















// static void
// print_char (u_char8 symbol, u_char8 color, u_char8 x, u_char8 y) {
//     asm volatile(
//         "mov %%cl, %%ah\n\t"
//         "stosw\n\t"
//         : "=a"(symbol)
//         : "a"(symbol), "c"(color),"D"(0xB8000 + (y * width + x) * 2)
//         :
//     );
//     // return symbol;
// }

// static void
// print_point (u_char8 *str, u_char8 color, u_char8 x, u_char8 y) {
//     while( *str != 0 ) {
//         print_char(*str,color,x,y);
//         str++;
//         x++;
//     }
//     x_pos = x;
// }

// static void
// print (u_char8 *str, u_char8 *end_str = "\n", u_char8 color = C_White) {
//     print_point(str, color, x_pos, y_pos);
//     if (end_str == "\n") {
//         y_pos++;
//         x_pos = 0;
//     }
//     else {
//         print_point(end_str, color, x_pos, y_pos);
//     }
// }

// static void
// clear () {
//     for (u_short16 i = 0; i < width*256; i++) {
//         asm volatile(
//             "stosb\n\t"
//             :
//             : "a"(0), "D"(0xB8000 + i)
//             :
//         );
//     }
    
// }

// static void
// f_print_point (s_f_string8 str, u_char8 color, u_char8 x, u_char8 y) {
//     for (u_char8 i = 0; i < str.len; i++) {
//         print_char(str.data[i],color,x,y);
//         x++;
//     }
//     x_pos = x;
// }

// static void
// f_print (s_f_string8 str, s_f_string8 end_str = "\n", u_char8 color = C_White) {
//     f_print_point(str, color, x_pos, y_pos);
//     if (end_str == "\n") {
//         y_pos++;
//         x_pos = 0;
//     }
//     else {
//         f_print_point(end_str, color, x_pos, y_pos);
//     }
// }