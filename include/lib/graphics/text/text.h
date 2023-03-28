#include "define/integer.h"

u_char8
print_char (u_char8 symbol, u_char8 color, u_char8 x, u_char8 y, u_short16 width)
{
    asm volatile(
        "mov %%cl, %%ah\n\t"
        // "stosw\n\t"
        : "=a"(symbol)
        : "a"(symbol), "c"(color),"D"(0xB8000 + (y * width + x) * 2)
        :
    );
    return symbol;
}

void
print (u_char8 *str, u_char8 color, u_char8 x, u_char8 y){
    while( *str != 0 )
    {
        print_char(*str,color,x,y,80);
        str++;
    }

}