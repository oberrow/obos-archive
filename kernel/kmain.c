#include <stdbool.h>

#include "terminal.h"
#include "types.h"

#if defined(__linux__)
#error Must be compiled with a cross compiler.
#endif
#if !defined(__i386__)
#error Must be compiled with a ix86 compiler.
#endif

void outb(UINT16_T port, UINT8_T val)
{
    asm volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) :"memory");
}
void outw(UINT16_T port, UINT16_T val)
{
    asm volatile ( "outw %0, %1" : : "a"(val), "Nd"(port) :"memory");
}
static inline UINT8_T inb(UINT16_T port)
{
    UINT8_T ret;
    asm volatile ( "inb %1, %0"
                   : "=a"(ret)
                   : "Nd"(port)
                   : "memory");
    return ret;
}
static inline void io_wait(void)
{
    outb(0x80, 0);
}

void kmain()
{
	InitializeTeriminal(TERMINALCOLOR_COLOR_WHITE | TERMINALCOLOR_COLOR_BLACK << 4);
	// asm("sti");
	label:
		TerminalOutputString("Hello, world!\r\n");
		for(unsigned i = 0; i < 0x2F00000; i++)
			asm("nop");
	goto label;
	outw(0xB004, 0x2000);
}