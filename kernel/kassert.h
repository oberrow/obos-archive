#ifndef __KASSERT_H
#define __KASSERT_H

#define KSTR_LITERAL(literal) literal, sizeof(literal)
#define kassert(expression, message, size) { if(!(expression)) { TerminalSetColor(TERMINALCOLOR_COLOR_RED | TERMINALCOLOR_COLOR_BLACK << 4); TerminalOutputString("Kernel panic! Assertion failed: \r\n"); if((void*)message) TerminalOutput(message, size); else { TerminalOutputCharacter('\t'); TerminalOutputString(#expression); } while(1); } }

#endif