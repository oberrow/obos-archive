#include <stdbool.h>

#include "terminal.h"
#include "types.h"
#include "kassert.h"
#include "kalloc.h"
#include "inline-asm.h"

#include "multiboot/mutliboot.h"

#if defined(__linux__)
#error Must be compiled with a cross compiler.
#endif
#if !defined(__i686__)
#error Must be compiled with a i686 compiler.
#endif

_Static_assert (sizeof(INT8_T) == 1, "INT8_T needs to be one byte.");
_Static_assert (sizeof(INT16_T) == 2, "INT16_T needs to be two bytes.");
_Static_assert (sizeof(INT32_T) == 4, "INT32_T needs to be four bytes.");
_Static_assert (sizeof(INT64_T) == 8, "INT64_T needs to be eight bytes.");

extern int acpiEnable(void);
extern int initAcpi(void);
extern int acpiEnable(void);
extern void acpiPowerOff(void);

multiboot_info_t* g_multibootInfo = (multiboot_info_t*)0;

static inline char* strcpy(char* destination, const char* source, int bytesToCopy)
{
	for(int i = 0; i < bytesToCopy; i++)
		destination[i] = source[i];
	return destination;
}

// int pow(int a, int b)
// {
// 	int res = a;
// 	for (int i = 0; i < b; i++) res *= a;
// 	res /= a;
// 	return res;
// }

static void onKernelPanic()
{
	TerminalSetColor(TERMINALCOLOR_COLOR_WHITE | TERMINALCOLOR_COLOR_BLACK >> 4);
	TerminalOutputString("Shuting down the computer.\r\n");
	for(int i = 0; i < 0x10000000; i++)
		nop();
	acpiPowerOff();
}

void kmain(multiboot_info_t* mbd, UINT32_T magic)
{
	InitializeTeriminal(TERMINALCOLOR_COLOR_WHITE | TERMINALCOLOR_COLOR_BLACK << 4);
	initAcpi();
	acpiEnable();
	
	setOnKernelPanic(onKernelPanic);

	kassert(magic == MULTIBOOT_BOOTLOADER_MAGIC, "Invalid magic number for multiboot.\r\n", 37);
	kassert(mbd->flags >> 6 & 0x1, "No memory map provided from GRUB.\r\n", 35);
	
	g_multibootInfo = mbd;
	kmeminit();

	SIZE_T real_size = 0;
	char* memory = (char*)kfindmemblock(32, &real_size);

	kassert(memory != (char*)0xFFFFFFFF, "Could not find a block big enough for the string.\r\n", 51);

	strcpy(memory, "Hello, world!\r\n", 16);
	strcpy(memory + 16, "Hello, world!\r\n", 16);
	TerminalOutput(memory, 15);
	TerminalOutput(memory + 16, 15);

	while(1);

	acpiPowerOff();
}