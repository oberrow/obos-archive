#include "kalloc.h"
#include "kassert.h"
#include "types.h"
#include "terminal.h"

#include "multiboot/mutliboot.h"

extern multiboot_info_t* g_multibootInfo;

memory_block* g_kMemoryTable = NULLPTR;
SIZE_T g_kMemoryTableSize = 0;

void kmeminit()
{
	// Allocate the memory for the table
	multiboot_memory_map_t* table = (PVOID)g_multibootInfo->mmap_addr;
	multiboot_memory_map_t* mmap = NULLPTR;
	int tableSize = 0;
	for(int i = 0; i < g_multibootInfo->mmap_length / sizeof(multiboot_memory_map_t); i++)
	{
		mmap = &table[i];
		// The memory is available only if it doesn't fall in the stack and mmap->type is MULTIBOOT_MEMORY_AVAILABLE.
		if(mmap->type == MULTIBOOT_MEMORY_AVAILABLE && (mmap->addr > 0x106000 || mmap->addr < 0x104000))
			tableSize++;
    }
	kassert(tableSize > 0, KSTR_LITERAL("No avaliable memory in the system."));
	tableSize *= sizeof(memory_block);
	int i = 0;
	BOOL foundMemory = FALSE;
	for(; i < g_multibootInfo->mmap_length / sizeof(multiboot_memory_map_t); i++)
	{
		mmap = &table[i];
		// The memory is available only if it doesn't fall in the stack and mmap->type is MULTIBOOT_MEMORY_AVAILABLE.
		if(mmap->type == MULTIBOOT_MEMORY_AVAILABLE && (mmap->addr > 0x106000 || mmap->addr < 0x104000) && mmap->len >= tableSize)
		{
			TerminalOutputString("Found memory block for the table.\r\n");
			g_kMemoryTable = (memory_block*)((UINTPTR_T)mmap->addr);
			foundMemory = TRUE;
			break;
		}
    }
	kassert(foundMemory, KSTR_LITERAL("Could not find a block of memory for the table.\r\n"));
	// Make the table.
	int i2 = 0;
	for(int i3 = 0; i3 < g_multibootInfo->mmap_length / sizeof(multiboot_memory_map_t); i3++)
	{
		mmap = &table[i3];
		if(mmap->type == MULTIBOOT_MEMORY_AVAILABLE && (mmap->addr > 0x106000 || mmap->addr < 0x104000) && i3 != i)
		{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wint-to-pointer-cast"
			TerminalOutputString("Found valid memory block.\r\n");
			g_kMemoryTable[i2].start = (memory_block*)mmap->addr;
			g_kMemoryTable[i2].end = (memory_block*)(mmap->addr + mmap->len);
#pragma GCC diagnostic pop
			g_kMemoryTable[i2].size = mmap->len;
			g_kMemoryTable[i2].isInUse = FALSE;
			g_kMemoryTableSize++;
			i2++;
		}
    }
}
PVOID kfindmemblock(SIZE_T size, SIZE_T* real_size)
{
	void* ret = (void*)0xFFFFFFFF;
	for(int i = 0; i < g_kMemoryTableSize; i++)
	{
		memory_block* block = &g_kMemoryTable[i];
		if(block->size >= size && !block->isInUse)
		{
			if (real_size)
				*real_size = block->size;
			ret = (PVOID)block->start;
			block->isInUse = TRUE;
			break;
		}
	}
	return ret;
}