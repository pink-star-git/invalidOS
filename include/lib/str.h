// include/lib/str.h
// Copyright (C) 2023  Alex Zebra

#pragma once

#include "../define/integer.h"
#include "math.h"

namespace detail {

template <class T, u_char8 N>
inline u_char8 strcpy(const T (&src)[N], T (&dst)[N]) { // only static arrays
    for (int i = 0; i < N; i++) {
        dst[i] = src[i];
    }
    return N;
}

inline u_char8 strcpy(const char *src, char *dst) { // should be \0 ended
    auto cur = src;
    while (*cur) {
        *dst++ = *cur++;
    }
    return cur - src;
}

template <class T, u_char8 N>
inline u_char8 strcmp(const T (&src)[N], const T (&dst)[N]) { // only static
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

inline u_char8 strcmp(const char *src, const char *dst) { // should be \0 ended
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

inline u_char8 memcpy(const char *src, char *dst, u_char8 size) {
    auto cur = src;
    while (src - cur <= size) { // TODO: test `size`
        *dst++ = *src++;
    }
    return src - cur;
}

inline u_char8 memset(char *dst, u_char8 size, char symbol = 0) {
    auto cur = dst;
    while (dst - cur <= size) { // TODO: test `size`
        *dst++ = symbol;
    }
    return dst - cur;
}

template <typename T> inline void swap(T &src, T &dst) {
    T tmp = src;
    src = dst;
    dst = tmp;
}

} // namespace detail

namespace sil { // standart invalid library
class string {
  public:
    string() {}
    string(const char *src) { len = detail::strcpy(src, (char *)data); }

    string(const string &str) {
        len = str.len;
        detail::strcpy(str.data, data);
    }

    string(string &&str) {
        len = str.len;
        detail::strcpy(str.data, data);
        str.clear();
    }

    string &operator=(const string &str) {
        len = str.len;
        detail::strcpy(str.data, data);
        return *this;
    }

    string &operator=(string &&str) {
        detail::swap(*this, str);
        str.clear();
        return *this;
    }

    u_char8 operator[](u_char8 index) const { return data[index]; }
    u_char8 &operator[](u_char8 index) { return data[index]; }

    bool operator==(const string &str) const {
        return (len == str.len) && !detail::strcmp(str.data, data);
    }

    bool operator!=(const string &str) const { return !(*this == str); }

    string &operator+=(const string &str) {
        if (len + str.len > 255)
            throw "out string is overflow";
        detail::strcpy((const char *)str.data, (char *)data + len);
        len += str.len;
        return *this;
    }

    string operator+(const string &str) const {
        if (len + str.len > 255)
            throw "out string is overflow";
        string temp(*this);
        temp += str;
        return temp;
    }

    string reverse() const {
        string temp(*this);
        for (u_char8 i = 0; i < temp.size() / 2; ++i)
            detail::swap(temp[i], temp[temp.size() - i - 1]);
        return temp;
    }

    void clear() { len = 0; }
    bool isEmpty() const { return !len; }
    u_char8 size() const { return len; }
    const char *c_str() const { return (const char *)data; }

  private:
    u_char8 len = 0; // should be size_t type
    u_char8 data[255] = {};
};
}; // namespace sil