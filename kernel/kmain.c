#include "stdint.h"

#if defined(__linux__)
#error Must be compiled with a cross compiler.
#endif
#if !defined(__i386__)
#error Must be compiled with a ix86 compiler.
#endif

void kmain()
{
    while(1);
}