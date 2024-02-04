// include/lib/str.h
// Copyright (C) 2023  Vi

#pragma once

#include "../define/integer.h"
#include "mem/mem.h"
#include "mem/mm.h"
#include "math.h"

namespace str {
    template <class T, char8 N>
    inline char8 cpy(const T (&src)[N], T (&dst)[N]) { // only static arrays
        for (int i = 0; i < N; i++) {
            dst[i] = src[i];
        }
        return N;
    }

    inline char8 cpy(const char8 *src, char8 *dst) { // should be \0 ended
        auto cur = src;
        while (*cur) {
            *dst++ = *cur++;
        }
        return cur - src;
    }

    inline char8 len(const char8 *src) { // should be \0 ended
        auto cur = src;
        while (*cur) {
            cur++;
        }
        return cur - src + 1;
    }

    template <class T, char8 N>
    inline char8 cmp(const T (&src)[N], const T (&dst)[N]) { // only static
        for (int i = 0; i < N; i++) {
            if (dst[i] < src[i]) {
                return -1;
            }
            if (dst[i] > src[i]) {
                return 1;
            }
            if (dst[i] == 0 && src[i] == 0)
                break;
        }
        return 0;
    }

    inline char8 cmp(const char8 *src, const char8 *dst) { // should be \0 ended
        while (1) {
            if (*dst < *src)
                return -1;
            if (*dst > *src)
                return 1;
            if (*dst++ == 0 && *src++ == 0)
                break;
        }
        return 0;
    }

    inline void rev(char *src) { // should be \0 ended
        char8 len;
        for (len = 0; len < 255 && src[len] != 0; len++) {}
        for (char8 i = 0; i < len / 2; ++i) {
            sil::swap(src[i], src[len - 1 - i]);
        }
        return 0;
    }
} // namespace str

namespace sil { // standart invalid library
    class string {
    public:
        string() {}

        string(const char8 *src) {
            len = 5;//str::len(src);
            data = sil::mem::malloc(len);
            sil::memcpy(src, data, len);
            // str::cpy(src, data);
        }

        string(const string &str) {
            len = str.size();
            data = sil::mem::malloc(len);
            sil::memcpy(str.c_str(), data, len);
        }

        string(string &&str) {
            len = str.size();
            data = sil::mem::malloc(len);
            sil::memcpy(str.c_str(), data, len);
            str.clear();
        }

        string &operator=(const string &str) {
            len = str.size();
            data = sil::mem::malloc(len);
            sil::memcpy(str.c_str(), data, len);
            return *this;
        }

        string &operator=(string &&str) {
            len = str.size();
            data = sil::mem::malloc(len);
            sil::memcpy(str.c_str(), data, len);
            str.clear();
            return *this;
        }

        char8 operator[](char8 index) const { return data[index]; }
        char8 &operator[](char8 index) { return data[index]; }

        bool operator==(const string &str) const {
            return (len == str.len) && !str::cmp(str.data, data);
        }

        bool operator!=(const string &str) const { return !(*this == str); }

        string &operator+=(const string &str) {
            // if (len + str.len > 255)
                // throw "out string is overflow";
            str::cpy(str.data, (data + len));
            len += str.len;
            return *this;
        }

        string operator+(const string &str) const {
            // if (len + str.len > 255)
            //     throw "out string is overflow";
            string temp(*this);
            temp += str;
            return temp;
        }

        string reverse() const {
            string temp(*this);
            for (char8 i = 0; i < temp.size() / 2; ++i)
                swap(temp[i], temp[temp.size() - i - 1]);
            return temp;
        }

        void clear() {
            len = 0;
            // data = nullptr;
            sil::mem::free(data);
        }
        bool isEmpty() const { return !len; }
        char8 size() const { return len; }
        const char *c_str() const { return (const char *)data; }

    private:
        char8 len = 0; // should be size_t type
        char8 *data;
    };
}; // namespace sil