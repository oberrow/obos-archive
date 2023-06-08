#include <stdbool.h>

#include "terminal.h"
#include "types.h"
#include "kassert.h"

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
UINT8_T inb(UINT16_T port)
{
    UINT8_T ret;
    asm volatile ( "inb %1, %0"
                   : "=a"(ret)
                   : "Nd"(port)
                   : "memory");
    return ret;
}
UINT16_T inw(UINT16_T port)
{
    UINT16_T ret;
    asm volatile ( "inw %1, %0"
                   : "=a"(ret)
                   : "Nd"(port)
                   : "memory");
    return ret;
}
static inline void io_wait(void)
{
    outb(0x80, 0);
}

_Static_assert (sizeof(INT8_T) == 1, "INT8_T needs to be one byte.");
_Static_assert (sizeof(INT16_T) == 2, "INT16_T needs to be two bytes.");
_Static_assert (sizeof(INT32_T) == 4, "INT32_T needs to be four bytes.");

int acpiEnable(void);
int initAcpi(void);
int acpiEnable(void);
void acpiPowerOff(void);

void kmain()
{
	initAcpi();
	acpiEnable();
	InitializeTeriminal(TERMINALCOLOR_COLOR_WHITE | TERMINALCOLOR_COLOR_BLACK << 4);
	
	// label:
	TerminalOutputString("Hello, world!\r\n");
	for(unsigned i = 0; i < 0x2F00000 * 10; i++)
		asm("nop");
	// goto label;
	acpiPowerOff();
	while(1);
}