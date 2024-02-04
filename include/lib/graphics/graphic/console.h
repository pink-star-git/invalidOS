// include/lib/garphics/garphic/garphic.h
// Copyright (C) 2023  Alex Zebra

#pragma once

#include "define/integer.h"
#include "define/color.h"

#define CONSOLE sil::Console::instance()

namespace sil { // standart invalid library

    // namespace cons
    // {
    //     send_char(const char8 &ch) {
    //         CONSOLE::set_char(ch);
    //     }

    //     send_digit(s_int32 num) {
    //         if(d < 0) {
    //             send_s('-');
    //             num = -num;
    //         }
    //         while(num) {
    //             s_int32 tmp = num % 10;
    //             num /= 10;
    //             ::CONSOLE::set_char('0' + (char)tmp);
    //         }
    //     }
    // } // namespace cons


    class Console
    {
      private:
        u_short16 x_pos = 0;
        u_short16 y_pos = 0;
        u_short16 width = 80;
        u_short16 height = 24;
        COLOR color_fg = COLOR::White;
        COLOR color_bg = COLOR::Black;
        Console() {};
      public:
        static Console &instance() {
            static Console dis = Console();
            return dis;
        }

        void set_char (u_char8 ch) {
            switch (ch)
            {
            case '\n':
                x_pos = 0;
                y_pos++;
                break;

            case '\t':
                x_pos+=4;
                break;

            default:
                asm volatile(
                    "mov %%cl, %%ah\n\t"
                    "stosw\n\t"
                    :
                    : "a"(ch), "c"(color_fg),"D"(0xB8000 + (y_pos * width + x_pos++) * 2)
                    :
                );
            }
            if(x_pos >= width) {x_pos = 0; y_pos++;}
        }


        void set_cursor_x(u_short16 x) {
            x_pos = x;
        }
        void set_cursor_y(u_short16 y) {
            y_pos = y;
        }

        u_short16 get_cursor_x() {
            return x_pos;
        }
        u_short16 get_cursor_y() {
            return y_pos;
        }


        void set_color_fg(COLOR color) {
            color_fg = color;
        }
        void set_color_bg(COLOR color) {
            color_bg = color;
        }

        COLOR get_color_fg() {
            return color_fg;
        }
        COLOR get_color_bg() {
            return color_bg;
        }
    };
} // namespace sil