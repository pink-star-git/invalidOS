// include/lib/garphics/garphic/garphic.h
// Copyright (C) 2023  Alex Zebra

#pragma once

#include "define/integer.h"

#define DISPLAY sil::display::instance()

namespace sil { // standart invalid library
    enum class COLOR {
        Black,
        Blue,
        Green,
        Turquoise,
        Red,
        Purple,
        Brown,
        Light_gray,
        Gray,
        Light_blue,
        Light_green,
        Light_turquoise,
        Pink,
        Light_purple,
        Yellow,
        White
    };

    class display
    {
      private:
        u_short16 x_pos = 0;
        u_short16 y_pos = 1;
        u_short16 width = 80;
        COLOR color_fg = COLOR::White;
        COLOR color_bg = COLOR::Black;
        display() {};
      public:
        static display& instance()
        {
            static display dis = display();
            return dis;
        }

        void set_char(u_char8 ch) {
                asm volatile(
                    "mov %%cl, %%ah\n\t"
                    "stosw\n\t"
                    :
                    : "a"(ch), "c"(color_fg),"D"(0xB8000 + (y_pos * width + x_pos++) * 2)
                    :
                );
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
    };
} // namespace sil