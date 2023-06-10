#ifndef __KASSERT_H
#define __KASSERT_H

#define KSTR_LITERAL(literal) literal, sizeof(literal)
#define kpanic(message, size) { \
    if((void*)message) \
    {\
        TerminalSetColor(TERMINALCOLOR_COLOR_RED | TERMINALCOLOR_COLOR_BLACK << 4);\
        TerminalOutputString("Kernel panic! Message: \r\n");\
        TerminalOutput((char*)message, (unsigned int)size);\
    }\
    while(1);\
}
#define kassert(expression, message, size) { \
    if(!(expression))\
    {\
        if((void*)message)\
            {kpanic(message, size);} \
        else \
        {\
            TerminalSetColor(TERMINALCOLOR_COLOR_RED | TERMINALCOLOR_COLOR_BLACK << 4);\
            TerminalOutputString("Kernel panic! Message: \r\n\t");\
            TerminalOutputString(#expression);\
        }\
        while(1);\
     }\
}

#endif