#include "define/integer.h"

static inline void outb(u_short16 port, u_char8 val)
{
    asm volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
}

static inline void outw(u_short16 port, u_short16 val)
{
    asm volatile ( "outw %0, %1" : : "a"(val), "Nd"(port) );
}

static inline void outl(u_short16 port, u_int32 val)
{
    asm volatile ( "outd %0, %1" : : "a"(val), "Nd"(port) );
}

static inline u_char8 inb(u_short16 port)
{
    u_char8 ret;
    asm volatile ( "inb %1, %0"
                   : "=a"(ret)
                   : "Nd"(port) );
    return ret;
}

void insl(unsigned reg, unsigned int *buffer, int quads)
{
    int index;
    for(index = 0; index < quads; index++)
    {
        buffer[index] = inb(reg);
    }
}