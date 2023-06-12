#ifndef __KASSERT_H
#define __KASSERT_H

#include "types.h"

#define KSTR_LITERAL(literal) literal, sizeof(literal) - 1

void setOnKernelPanic(void(*callback)());

void kpanic(CSTRING message, SIZE_T size);
void kassert(BOOL expression, CSTRING message, SIZE_T size);

#endif