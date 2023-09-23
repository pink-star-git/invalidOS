// include/lib/str.h
// Copyright (C) 2023  Alex Zebra

#pragma once

#include "define/integer.h"
#include "math.h"

// struct format string len8
struct s_f_string8 {
    u_char8 len = 0;
    u_char8 data[255] = { 0 };

    s_f_string8 () {};

    s_f_string8 (u_char8 *v) {
        for (u_short16 i = 0; i < 255; i++){
            if (v[i] == 0) { 
                len = i;
                break;
            }
            data[i] = v[i];
        }
    };

    s_f_string8
    operator= (const s_f_string8& v) {
        if (this == &v)
            return *this;
        len = v.len;
        for (u_short16 i = 0; i < len; i++){
            data[i] = v.data[i];
        }
        return *this;
    };

    // s_f_string8
    // operator+ (u_char8 v) {
    //     s_f_string8 res = *this;
    //     res.data[res.len+1] = v;
    //     res.len++;
    //     return res;
    // };

    s_f_string8
    operator+ (u_char8 *v) {
        s_f_string8 in(v);
        s_f_string8 res = *this;
        for (u_char8 i = 0; i < in.len; i++)
            res.data[res.len+i] = in.data[i];
        res.len += in.len;
        return res;
    };

    s_f_string8
    operator+ (const s_f_string8& v) {
        s_f_string8 res = *this;
        for (u_char8 i = 0; i < v.len; i++)
            res.data[len+i] = v.data[i];
        res.len += v.len;
        return res;
    };
};

s_f_string8
operator+ (u_char8 *lv, s_f_string8 rv) {
    s_f_string8 in = s_f_string8(lv);
    for (u_char8 i = 0; i < rv.len; i++)
        in.data[in.len+i] = rv.data[i];
    in.len += rv.len;
    return in;
};

bool
operator== (s_f_string8 lv, s_f_string8 rv) {
    if (lv.len != rv.len)
        return false;
    for (u_char8 i = 0; i < lv.len; i++)
        if (lv.data[i] != rv.data[i])
            return false;
    return true;
};

static u_char8 new_line[4] = "\n\r\0";



static void
str_clear_f (s_f_string8& str) {
    for (u_char8 i = 0; i < 255; i++)
        str.data[i] = 0;
};

static u_char8
str_get_len (u_char8 *str) {
    for (u_char8 i = 0; i < 255; i++)
        if (str[i] == 0)
            return i;
    return 0;
};

static u_char8
str_calc_len (u_char8 *data) {
    u_char8 len = 0;
    for (u_char8 i = 0; i < 255; i++)
        if (data[i] == 0) { 
            len = i;
            break;
        }
    return len;
};

static u_char8
str_cmp (u_char8 *str1, u_char8 *str2) {
    u_char8 len_str1 = str_get_len(str1);
    u_char8 len_str2 = str_get_len(str2);
    if (len_str1 != len_str2)
        return 1;
    for (u_char8 i = 0; i < len_str1; i++)
        if (str1[i] != str2[i])
            return 1;
    return 0;
};

static bool
f_str_cmp (s_f_string8 str1, s_f_string8 str2) {
    if (str1.len != str2.len)
        return false;
    for (u_char8 i = 0; i < str1.len; i++)
        if (str1.data[i] != str2.data[i])
            return false;
    return true;
};

static void
str_rev (u_char8 *begin, u_char8 *end, u_char8 n) {
    if (n > 1) {
        u_char8 t = *begin;
        *begin = *end;
        *end = t;
        str_rev(begin + 1, end - 1, n - 2);
    }
};

static s_f_string8
hex_2_str (u_int32 num) {
    s_f_string8 conv_str;
    if (num == 0)
        conv_str.data[0] = 0x30;
    else {
        u_short16 i = 0;
        for ( ; i < 255; i++) {
            if (num / 16 == 0 && num % 16 == 0)
                break;
            u_short16 tmp = num % 16;
            num = num / 16;
            if (tmp < 10)
                conv_str.data[i] = tmp + 0x30;
            else
                conv_str.data[i] = tmp - 10 + 0x41;
        }
        str_rev(conv_str.data, conv_str.data + i - 1, i);
        conv_str.data[i] = 0;
    }
    conv_str.len = str_calc_len(conv_str.data);
    return conv_str;
};

static s_f_string8
dec_2_str (u_int32 num) {
    s_f_string8 conv_str;
    if (num == 0)
        conv_str.data[0] = '0';
    else {
        u_short16 i = 0;
        for ( ; i < 255; i++) {
            if (num / 10 == 0 && num % 10 == 0)
                break;
            u_short16 tmp = num % 10;
            num = num / 10;
            conv_str.data[i] = tmp + '0';
        }
        str_rev(conv_str.data, conv_str.data + i - 1, i);
        conv_str.data[i] = 0;
    }
    conv_str.len = str_calc_len(conv_str.data);
    return conv_str;
};

static s_f_string8
bin_2_str (u_int32 num) {
    s_f_string8 conv_str;
    if (num == 0)
        conv_str.data[0] = 0x30;
    else {
        u_short16 i = 0;
        for ( ; i < 255; i++) {
            if (num / 2 == 0 && num % 2 == 0)
                break;
            u_short16 tmp = num % 2;
            num = num / 2;
            conv_str.data[i] = tmp + 0x30;
        }
        str_rev(conv_str.data,  conv_str.data + i - 1, i);
        conv_str.data[i] = 0;
    }
    conv_str.len = str_calc_len(conv_str.data);
    return conv_str;
};

static s_f_string8
float_2_str (float num, u_char8 len = 1) {
    u_int32 h = (u_int32)num;
    u_int32 l = (u_int32)((num - h) * pow(10, len));
    s_f_string8 res = dec_2_str(h) + "." + dec_2_str(l);
    return res;
};

static u_int32
str_2_dec (s_f_string8 str) {
    u_int32 num = 0;
    for (u_short16 i = 0; i < 255; i++) {
        if (str.data[i] == 0)
            break;
        num = (num + (str.data[i] - 0x30)) * 10;
    }
    return num / 10;
};

static u_int32
str_2_hex (s_f_string8 str) {
    u_int32 num = 0;
    for (u_short16 i = 0; i < 255; i++) {
        if (str.data[i] == 0)
            break;
        else if (str.data[i] > 0x40)
            num = (num + (str.data[i] - 0x41)) * 16;
        else
            num = (num + (str.data[i] - 0x30)) * 16;
    }
    return num / 16;
};

static u_int32
str_2_bin (s_f_string8 str) {
    u_int32 num = 0;
    for (u_short16 i = 0; i < 255; i++) {
        if (str.data[i] == 0)
            break;
        num = (num + (str.data[i] - 0x30)) * 2;
    }
    return num / 2;
};