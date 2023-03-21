#include "define/integer.h"

// out
static inline void
outb (u_short16 port, u_char8 val)
{
    asm volatile (
                    "outb %0, %1"
                    :
                    : "a"(val), "Nd"(port)
                );
}

static inline void
outw (u_short16 port, u_short16 val)
{
    asm volatile (
                    "outw %0, %1"
                    :
                    : "a"(val), "Nd"(port)
                );
}

static inline void
outl (u_short16 port, u_int32 val)
{
    asm volatile (
                    "outl %0, %1"
                    :
                    : "a"(val), "Nd"(port)
                );
}

// in
static inline u_char8
inb (u_short16 port)
{
    u_char8 ret;
    asm volatile (
                    "inb %1, %0"
                    : "=a"(ret)
                    : "Nd"(port)
                );
    return ret;
}

static inline u_short16
inw (u_short16 port)
{
    u_short16 ret;
    asm volatile (
                    "inw %1, %0"
                    : "=a"(ret)
                    : "Nd"(port)
                );
    return ret;
}

static inline u_int32
inl (u_short16 port)
{
    u_int32 ret;
    asm volatile (
                    "inl %1, %0"
                    : "=a"(ret)
                    : "Nd"(port)
                );
    return ret;
}

// ins
void
insb (u_short16 reg, u_char8 *buffer, u_int32 quads)
{
    u_int32 index;
    for(index = 0; index < quads; index++)
    {
        buffer[index] = inb(reg);
    }
}

void
insw (u_short16 reg, u_short16 *buffer, u_int32 quads)
{
    u_int32 index;
    for(index = 0; index < quads; index++)
    {
        buffer[index] = inw(reg);
    }
}

void
insl (u_short16 reg, u_int32 *buffer, u_int32 quads)
{
    u_int32 index;
    for(index = 0; index < quads; index++)
    {
        buffer[index] = inl(reg);
    }
}