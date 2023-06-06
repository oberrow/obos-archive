#ifndef __OBOS_TERMINAL_H
#define __OBOS_TERMINAL_H

#include "types.h"

typedef enum {
	TERMINALCOLOR_COLOR_BLACK = 0,
	TERMINALCOLOR_COLOR_BLUE = 1,
	TERMINALCOLOR_COLOR_GREEN = 2,
	TERMINALCOLOR_COLOR_CYAN = 3,
	TERMINALCOLOR_COLOR_RED = 4,
	TERMINALCOLOR_COLOR_MAGENTA = 5,
	TERMINALCOLOR_COLOR_BROWN = 6,
	TERMINALCOLOR_COLOR_LIGHT_GREY = 7,
	TERMINALCOLOR_COLOR_DARK_GREY = 8,
	TERMINALCOLOR_COLOR_LIGHT_BLUE = 9,
	TERMINALCOLOR_COLOR_LIGHT_GREEN = 10,
	TERMINALCOLOR_COLOR_LIGHT_CYAN = 11,
	TERMINALCOLOR_COLOR_LIGHT_RED = 12,
	TERMINALCOLOR_COLOR_LIGHT_MAGENTA = 13,
	TERMINALCOLOR_COLOR_LIGHT_BROWN = 14,
	TERMINALCOLOR_COLOR_WHITE = 15,
} TERMINALCOLOR;

typedef struct _CONSOLEPOINT
{
    INT x;
    INT y;
} CONSOLEPOINT, *PCONSOLEPOINT;

void _cdecl InitializeTeriminal(UINT8_T color);
void _cdecl SetTerminalCursorPosition(CONSOLEPOINT point);
CONSOLEPOINT _cdecl GetTerminalCursorPosition();
void _cdecl TerminalSetColor(UINT8_T color);
void _cdecl TerminalOutputCharacter(CHAR c);
void _cdecl TerminalOutputCharacterAt(CONSOLEPOINT point, CHAR c);
void _cdecl TerminalOutput(CSTRING data, SIZE_T size);
void _cdecl TerminalOutputString(CSTRING string);
#endif