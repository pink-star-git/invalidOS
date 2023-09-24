// include/lib/garphics/text/text.h
// Copyright (C) 2023  Alex Zebra

#pragma once

#include "define/integer.h"
#include "lib/graphics/graphic/display.h"
#include "lib/str.h"


namespace sil { // standart invalid library

    void print(const string& str)
    {
        for(int i = 0; i < str.size(); ++i) DISPLAY.set_char(str[i]);
        // DISPLAY.get_cursor_x(0);
        // DISPLAY.get_cursor_y(DISPLAY.get_cursor_y());
    }

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