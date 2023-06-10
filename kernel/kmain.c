#include <stdbool.h>

#include "terminal.h"
#include "types.h"
#include "kassert.h"
#include "kalloc.h"

#include "multiboot/mutliboot.h"

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

multiboot_info_t* g_multibootInfo = (multiboot_info_t*)0;

void __attribute__((stdcall)) invoke_kmain(multiboot_info_t* mbd, UINT32_T magic)
{
void kmain(multiboot_info_t* _mbd, UINT32_T _magic);
    kmain(mbd, magic);
}

static inline char* strcpy(char* destination, const char* source, int bytesToCopy)
{
	for(int i = 0; i < bytesToCopy; i++)
		destination[i] = source[i];
	return destination;
}

int pow(int a, int b)
{
	int res = a;
	for (int i = 0; i < b; i++) res *= a;
	res /= a;
	return res;
}

void kmain(multiboot_info_t* mbd, UINT32_T magic)
{
	InitializeTeriminal(TERMINALCOLOR_COLOR_WHITE | TERMINALCOLOR_COLOR_BLACK << 4);
	initAcpi();
	acpiEnable();
	
	kassert(magic == MULTIBOOT_BOOTLOADER_MAGIC, "Invalid magic number for multiboot.\r\n", 37);
	kassert(mbd->flags & (pow(2, 6) >> 1), "No memory map provided from GRUB.\r\n", 35);
	
	g_multibootInfo = mbd;
	kmeminit();

	SIZE_T real_size = 0;
	char* memory = (char*)kalloc(16, &real_size);

	if(memory)
	{
		strcpy(memory, "Hello, world!\r\n", 16);
		TerminalOutput(memory, 15);
	}

	while(1);

	acpiPowerOff();
}