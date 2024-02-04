// include/lib/garphics/text/print.h
// Copyright (C) 2024  Vi


//! THIS FILE DON`T USE

#pragma once 

#include "define/integer.h"
#include "define/arg.h"
#include "define/color.h"

#define gio sil::GraphicsIO::instance()
#define cio sil::ConsoleIO::instance()

namespace sil {
struct Cursor {
    u_short16 x = 0,
              y = 0;
    COLOR bg = COLOR::White,
          fg = COLOR::White;
};

class ConsoleIO {
    public:
        static ConsoleIO &instance() {
            static ConsoleIO console = ConsoleIO();
            return console;
        }

        void set_console_size(u_short16 _w) { w = _w; }

        void next_symbol() {
            if(++m_cur.x >= w) {
                m_cur.x = 0;
                ++m_cur.y;
            }
        }

        void move_cursor(u_int32 s) {
            while(s--)
                next_symbol();
        }

        void new_line() { m_cur.x = 0;
            m_cur.y++;
        }

        void set_cursor(const Cursor &cursor) { m_cur = cursor; }
        Cursor get_cursor() { return m_cur; }

        void send_s(u_char8 c) {
            switch (c)
            {
            case '\t':
                move_cursor(4);
                break;
            case '\n':
                new_line();
                break;
            default:
                send_char(c);
            }
        }

        void send_d(s_int32 d) {
            if(d < 0) {
                send_s('-');
                d = -d;
            }
            while(d) {
                s_int32 tmp = d % 10;
                d /= 10;
                send_char((char)tmp + '0');
            }
        }

        void send_f(double f) {
            if(f < 0) {
                send_s('-');
                f = -f;
            }
            s_int32 d = (s_int32)f;
            f -= d;
            send_d(d);
            send_s('.');
            double  rem = f;
            for (u_long64 i = 10; i < (u_long64)10e20; i*=10) {
                f *= 10;
                d = (s_int32)f;
                f -= d;
                rem -= f * i;
                send_d(d);
                if (rem < 0.00000000000000001) break;

            // while (f) {
                // f *= 10;
                // d = (u_int32)f;
                // f -= d;
            }
        }

    private:
        u_short16 w = 80;
        Cursor m_cur;

        ConsoleIO() = default;

        void send_char(const u_char8 &c) {
            asm volatile(
                "mov %%cl, %%ah\n\t"
                "stosw\n\t"
                :
                : "a"(w), "c"(m_cur.fg), "D"(0xB8000 + (m_cur.y * w + m_cur.x) * 2)
                :
            );
        }
};

class GraphicsIO {
  public:
    static GraphicsIO &instance() { 
        static GraphicsIO graphics = GraphicsIO();
        return graphics;
    }

    void printf_c(const char *format) {
        s_int32 d;
        double f;
        // va_list factor;
        // va_start(factor, format);

        for(char8 *c = format; *c; ++c) {
            if(*c != '%') {
                // TODO print char
                cio.send_s((char8 )(*c));
                continue;
            }
            switch (*++c)
            {
            case 'd':
                d = va_arg(factor, s_int32);
                // TODO print int
                cio.send_d(d);
                break;
            case 'f':
                f = va_arg(factor, double);
                // TODO print double
                cio.send_f(f);
                break;
            default:
                cio.send_s(*c);
                // TODO print char
            }
        }
        // va_end(factor);
    }

  private:
    GraphicsIO() = default;
};
} // namespace sil