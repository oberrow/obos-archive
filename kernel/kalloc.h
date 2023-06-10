#ifndef __OBOS_KALLOC_H
#define __OBOS_KALLOC_H
#include "types.h"

typedef struct __memory_block
{
    INT size;
    PCVOID start;
    PCVOID end;
    BOOL isInUse;
} memory_block;
// Initializes the memory table.
void kmeminit();
// Gets a block for use by the kernel
void* kalloc(SIZE_T size, SIZE_T* real_size);
#endif