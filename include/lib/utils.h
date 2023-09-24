// include/lib/garphics/text/text.h
// Copyright (C) 2023  Alex Zebra

#pragma once

#include "str.h"

namespace utils {
    static sil::string
    hex_2_str (u_int32 num) {
        u_char8 data[255] = {};
        if (num == 0)
            data[0] = 0x30;
        else {
            u_short16 i = 0;
            for ( ; i < 255; i++) {
                if (num / 16 == 0 && num % 16 == 0)
                    break;
                u_short16 tmp = num % 16;
                num = num / 16;
                if (tmp < 10)
                    data[i] = tmp + 0x30;
                else
                    data[i] = tmp - 10 + 0x41;
            }
            str::rev(data);
            // conv_str.data[i] = 0;
        }
        sil::string conv_str = sil::string(data);
        return conv_str;
    };

    static sil::string
    dec_2_str (u_int32 num) {
        u_char8 data[255] = {};
        if (num == 0)
            data[0] = '0';
        else {
            u_short16 i = 0;
            for ( ; i < 255; i++) {
                if (num / 10 == 0 && num % 10 == 0)
                    break;
                u_short16 tmp = num % 10;
                num = num / 10;
                data[i] = tmp + '0';
            }
            str::rev(data);
            // conv_str.data[i] = 0;
        }
        sil::string conv_str = sil::string(data);
        return conv_str;
    };

    static sil::string
    bin_2_str (u_int32 num) {
        u_char8 data[255] = {};
        if (num == 0)
            data[0] = 0x30;
        else {
            u_short16 i = 0;
            for ( ; i < 255; i++) {
                if (num / 2 == 0 && num % 2 == 0)
                    break;
                u_short16 tmp = num % 2;
                num = num / 2;
                data[i] = tmp + 0x30;
            }
            str::rev(data);
            data[i] = 0;
        }
        sil::string conv_str = sil::string(data);
        return conv_str;
    };
} // namespace utils