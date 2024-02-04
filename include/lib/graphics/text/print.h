// include/lib/garphics/text/print.h
// Copyright (C) 2023  Alex Zebra

#pragma once

#include "define/integer.h"
#include "define/arg.h"

#include "lib/graphics/graphic/console.h"
#include "lib/str.h"
#include "lib/utils.h"


namespace sil { // standart invalid library

    void clear() {
        for (int i = 0; i < 30 * 100; ++i)
            CONSOLE.set_char(0);
        CONSOLE.set_cursor_x(0);
        CONSOLE.set_cursor_y(0);
    }

    // void printf(const char *format) {
    //     s_int32 d;
    //     double f;
    //     va_list factor;
    //     va_start(factor, format);

    //     for(char8 *c = format; *c; ++c) {
    //         if(*c != '%') {
    //             // TODO print char
    //             CONSOLE.set_char((char8 )(*c));
    //             continue;
    //         }
    //         switch (*++c)
    //         {
    //         case 'd':
    //             d = va_arg(factor, s_int32);
    //             // TODO print int
    //             CONSOLE.send_digit(d);
    //             break;
    //         case 'f':
    //             f = va_arg(factor, float);
    //             // TODO print float
    //             CONSOLE.send_float(f);
    //             break;
    //         default:
    //             CONSOLE.send_char(*c);
    //             // TODO print char
    //         }
    //     }
    //     // va_end(factor);
    // }

    void print (const string& str1, const string& str_end = string("\n")) {
        for(int i = 0; i < str1.size(); ++i) CONSOLE.set_char(str1[i]);
        for(int i = 0; i < str_end.size(); ++i) CONSOLE.set_char(str_end[i]);
    }

    void printc (const string& str, const COLOR color, const string& str_end = string("\n")) {
        COLOR clr_tmp = CONSOLE.get_color_fg();
        CONSOLE.set_color_fg(color);
        print(str, str_end);
        CONSOLE.set_color_fg(clr_tmp);
    }

    namespace debug {
        enum class DEBUG {
            CHR,
            STR,
            DEC,
            HEX,
            BIN,
            BOOL
        };

        void printd (const string& name, auto data, DEBUG data_type) {
            print(name+sil::string(":"), sil::string(""));
            if ((data_type == DEBUG::CHR))
                CONSOLE.set_char(data);
            else if ((data_type == DEBUG::STR))
                print(sil::string(data));
            else if ((data_type == DEBUG::DEC))
                print(utils::dec_2_str(data));
            else if ((data_type == DEBUG::HEX))
                print(utils::hex_2_str(data));
            else if ((data_type == DEBUG::BIN))
                print(utils::bin_2_str(data));
            else if ((data_type == DEBUG::BOOL))
                print(data ? "true" : "false");
        }
    } // namespace debug
} // namespace sil