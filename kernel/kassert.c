#include "kassert.h"
#include "terminal.h"

static void defaultKernelPanic()
{
    while(1);
}

static void(*onKernelPanic)() = defaultKernelPanic;

void setOnKernelPanic(void (*callback)())
{
    onKernelPanic = callback;
}

void kpanic(CSTRING message, SIZE_T size)
{
    if((void*)message) 
    {
        TerminalSetColor(TERMINALCOLOR_COLOR_RED | TERMINALCOLOR_COLOR_BLACK << 4);
        TerminalOutputString("Kernel panic! Message: \r\n");
        TerminalOutput((char*)message, (unsigned int)size);
    }
    onKernelPanic();\
}
void kassert(BOOL expression, CSTRING message, SIZE_T size)
{
    if(!(expression))
        kpanic(message, size);
}