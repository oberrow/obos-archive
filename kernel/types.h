#ifndef __OBOS_INTEGER_H
#define __OBOS_INTEGER_H
#ifdef __GNUC__
#define _cdecl __attribute__((thiscall))
#else
#define _cdecl
#endif

typedef int BOOL;
typedef unsigned long SIZE_T;
typedef unsigned char UINT8_T;
typedef unsigned short UINT16_T;
typedef unsigned int UINT32_T;
typedef char INT8_T;
typedef short INT16_T;
typedef int INT32_T;
typedef int INT;
typedef char CHAR, *PCHAR;
typedef char *STRING;
typedef const char *CSTRING;
#endif