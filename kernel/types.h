#ifndef __OBOS_INTEGER_H
#define __OBOS_INTEGER_H

#ifdef _GNU_C
#define _cdecl __attribute__((__cdecl__))
#else
#define _cdecl
#endif

#define NULLPTR ((void*)0)

typedef enum { FALSE, TRUE } BOOL;
typedef unsigned long SIZE_T;
typedef unsigned char UINT8_T;
typedef unsigned short UINT16_T;
typedef unsigned int UINT32_T;
typedef char INT8_T;
typedef unsigned char BYTE;
typedef short INT16_T;
typedef int INT32_T;
typedef unsigned int DWORD;
typedef unsigned short WORD;
typedef int INT;
typedef char CHAR, *PCHAR;
typedef char *STRING;
typedef const char *CSTRING;
typedef void VOID;
typedef void* PVOID;
typedef const void* PCVOID;
#endif