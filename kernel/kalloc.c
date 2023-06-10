#include "kalloc.h"
#include "types.h"
#include "terminal.h"

#include "multiboot/mutliboot.h"

extern multiboot_info_t* g_multibootInfo;

memory_block* g_kMemoryTable;
SIZE_T g_kMemoryTableSize;

void kmeminit()
{
	// Allocate the memory for the table
	multiboot_memory_map_t* mmap = NULLPTR;
	int i = 0;
	for(; i < g_multibootInfo->mmap_length; i += sizeof(multiboot_memory_map_t))
	{
		mmap = (multiboot_memory_map_t*)g_multibootInfo->mmap_addr + i;
		// The memory is available only if it doesn't fall in the stack and mmap->type is MULTIBOOT_MEMORY_AVAILABLE.
		if(mmap->type == MULTIBOOT_MEMORY_AVAILABLE && mmap->addr > 0x106000 && mmap->addr < 0x104000)
		{
			TerminalOutputString("Found memory block for the table.\r\n");
			g_kMemoryTable = (memory_block*)((int)mmap->addr);
			break;
		}
    }
	// Make the table.
	for(; i < g_multibootInfo->mmap_length; i += sizeof(multiboot_memory_map_t))
	{
		mmap = (multiboot_memory_map_t*)g_multibootInfo->mmap_addr + i;
		if(mmap->type == MULTIBOOT_MEMORY_AVAILABLE && mmap->addr > 0x106000 && mmap->addr < 0x104000)
		{
			TerminalOutputString("Found valid memory block.\r\n");
			g_kMemoryTable[i].start = (memory_block*)(((int)mmap->addr));
			g_kMemoryTable[i].end = (memory_block*)(((int)mmap->addr) + mmap->size);
			g_kMemoryTable[i].size = mmap->size;
			g_kMemoryTable[i].isInUse = FALSE;
			g_kMemoryTableSize++;
		}
    }
}
PVOID kalloc(SIZE_T size, SIZE_T* real_size)
{
	void* ret = NULLPTR;
	for(int i = 0; i < g_kMemoryTableSize; i++)
	{
		memory_block* block = &g_kMemoryTable[i];
		if(block->size >= size && !block->isInUse)
		{
			if (real_size)
				*real_size = block->size;
			ret = (PVOID)block->start;
			block->isInUse = TRUE;
		}
	}
	return ret;
}