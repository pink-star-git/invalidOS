// include/lib/str.h
// Copyright (C) 2023  Vi

#pragma once

#include "define/integer.h"

namespace sil  // standart invalid library
{
    inline u_int32 memcpy(const char8 *src, char8 *dst, u_int32 size) {
        auto cur = src;
        while (src - cur <= size) { // TODO: test `size`
            *dst++ = *src++;
            // *dst++ = 'x';
            // src++;
        }
        return src - cur;
    }

    inline u_int32 memset(char8 *dst, u_int32 size, char8 symbol = 0) {
        auto cur = dst;
        while (dst - cur <= size) { // TODO: test `size`
            *dst++ = symbol;
        }
        return dst - cur;
    }

    template <typename T>
    inline void swap(T &src, T &dst) {
        T tmp = src;
        src = dst;
        dst = tmp;
    }
} // namespace sil
