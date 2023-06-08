#include "terminal.h"
#include "types.h"

#if defined(__linux__)
#error Must be compiled with a cross compiler.
#endif
#if !defined(__i386__)
#error Must be compiled with a ix86 compiler.
#endif

extern void outb(UINT16_T port, UINT8_T val);

static const SIZE_T VGA_WIDTH = 80;
static const SIZE_T VGA_HEIGHT = 25;
 
SIZE_T terminal_row;
SIZE_T terminal_column;
UINT8_T terminal_color;
UINT16_T* terminal_buffer;
 
static inline UINT16_T vga_entry(unsigned char uc, UINT8_T color) 
{
	return (UINT16_T) uc | (UINT16_T) color << 8;
}

void SetTerminalCursorPosition(CONSOLEPOINT point)
{
	UINT16_T pos = point.y * VGA_WIDTH + point.x;
 
	outb(0x3D4, 0x0F);
	outb(0x3D5, (UINT8_T) (pos & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (UINT8_T) ((pos >> 8) & 0xFF));

    terminal_column = point.x;
    terminal_row = point.y;
}
CONSOLEPOINT GetTerminalCursorPosition()
{
    CONSOLEPOINT ret;
    ret.x = terminal_column;
    ret.y = terminal_row;
    return ret;
}

void InitializeTeriminal(UINT8_T color) 
{
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = color;
	terminal_buffer = (UINT16_T*) 0xB8000;
	for (SIZE_T y = 0; y < VGA_HEIGHT; y++) {
		for (SIZE_T x = 0; x < VGA_WIDTH; x++)
		{
			const SIZE_T index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
	}
}
 
void TerminalSetColor(UINT8_T color) 
{
	terminal_color = color;
}
 
void terminal_putentryat(char c, UINT8_T color, SIZE_T x, SIZE_T y) 
{
	const SIZE_T index = y * VGA_WIDTH + x;
	terminal_buffer[index] = vga_entry(c, color);
}

static int g_reachedEndTerminal = 0;

void TerminalOutputCharacter(CHAR c)
{
	if(c == '\n')
	{
		if(++terminal_row >= VGA_HEIGHT)
		{ terminal_row = 0; g_reachedEndTerminal = 1; }
		CONSOLEPOINT point;
		point.x = terminal_column;
		if(!g_reachedEndTerminal)
			point.y = terminal_row;
		SetTerminalCursorPosition(point);
		return;
	}
	else if (c == '\r')
	{
		terminal_column = -1;
		c = 0;
	}
	else if (c == '\t')
	{
		for(int i = 0; i < 5 && terminal_column < VGA_WIDTH; i++)
			terminal_putentryat(' ', terminal_color, ++terminal_column, terminal_row);
		return;
	}
	terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
	if (++terminal_column == VGA_WIDTH)
	{
		terminal_column = 0;
		if (++terminal_row == VGA_HEIGHT)
			{terminal_row = 0; g_reachedEndTerminal = 1;}
	}
	CONSOLEPOINT point;
	point.x = terminal_column;
	if(!g_reachedEndTerminal)
    	point.y = terminal_row;
    SetTerminalCursorPosition(point);
}
void TerminalOutputCharacterAt(CONSOLEPOINT point, CHAR c)
{
	terminal_putentryat(c, terminal_color, point.x, point.y);
}
void TerminalOutput(CSTRING data, SIZE_T size)
{
	for (SIZE_T i = 0; i < size; i++)
		TerminalOutputCharacter(data[i]);
}
void TerminalOutputString(CSTRING data)
{
	SIZE_T i = 0;
	for(; data[i]; i++);
	TerminalOutput(data, i);
}