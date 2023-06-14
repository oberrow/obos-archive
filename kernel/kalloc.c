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
	if(size == -1)
	{
		register void* sp asm("sp");
		struct size
		{
			SIZE_T len;
			SIZE_T index;
		};
		struct size* sizes = sp -= g_kMemoryTableSize * sizeof(size);
		for(int i = 0; i < g_kMemoryTableSize; i++)
		{
			memory_block* block = &g_kMemoryTable[i];
			if(!block->isInUse)
			{
				struct size temp;
				temp.len = block->size;
				temp.index = i;
				sizes[i] = temp;
				break;
			}
		}
		int temp = 0;
		for (int i = 0; i < g_kMemoryTableSize; i++) 
			for (int j = i+1; j < g_kMemoryTableSize; j++) 
           		if(sizes[i].len > sizes[j].len) 
				{    
               		temp = sizes[i].len;
               		sizes[i].len = sizes[j].len;
               		sizes[j].len = temp;    
           		}
		SIZE_T biggestBlockIndex = sizes[g_kMemoryTableSize - 1].index;  
		memory_block* block = g_kMemoryTable + biggestBlockIndex;
		ret = (PVOID)block->start;
		block->isInUse = TRUE;
		*real_size = block->size;
	}
	for(int i = 0; i < g_kMemoryTableSize && size != -1; i++)
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

typedef struct __heapMemoryBlock
{
	PVOID location;
	SIZE_T size;
	UINT32_T __padding;
	SIZE_T expectedResize;
} __attribute__((packed)) heapMemoryBlock; 

static PVOID g_kHeap = NULLPTR;
static SIZE_T g_kHeapBlockSize = 0;
static SIZE_T g_kHeapHeaderSize = 0;

void kheapinit(PVOID block, SIZE_T blockSize)
{
	kassert(blockSize % 512 == 0, KSTR_LITERAL("The block of memory for the heap must be divisible by 512."));
	g_kHeap = block;
	// We have a complicated math problem. We need some magic number that we can: a) calculate at runtime b) 
	// can be predefined. This number must be able to divide a number divisible by 512 by this number and get a number we
	// can subtract by this number divisible by 512 and will make the number that is divided / 8 =
	// the number that is subtracted / 512. Is this even possible?
	// I kind of gave up on that, but if you happen to come across this file and find a solution,
	// please make an issue with the solution.
	g_kHeapHeaderSize = blockSize / sizeof(heapMemoryBlock) * 2;
	g_kHeapBlockSize = (blockSize - g_kHeapHeaderSize / 2) / 2;
}
PVOID kheapalloc(SIZE_T size, SIZE_T expectedResize)
{
	heapMemoryBlock* heapHeader = g_kHeap;
	heapMemoryBlock* headerData = NULLPTR;
	int i = 0;
	for(; i < g_kHeapHeaderSize / sizeof(heapMemoryBlock); i++)
	{
		headerData = &heapHeader[i];
		if(headerData->location == NULLPTR && headerData->size == 0)
			break;
		else
			continue;
	}
	if(i == 0)
		return (PCHAR)g_kHeap + g_kHeapHeaderSize;
	if(headerData->location != NULLPTR && headerData->size != 0)
		return (PVOID)-1;
	PVOID blockLocation = (headerData - 1)->location + (headerData - 1)->expectedResize + size / 2;
	if (blockLocation > g_kHeap + g_kHeapBlockSize + g_kHeapHeaderSize || blockLocation < g_kHeap)
		blockLocation -= size / 2;
	if (blockLocation > g_kHeap + g_kHeapBlockSize + g_kHeapHeaderSize || blockLocation < g_kHeap)
		return 0xFFFFFFFF;
	for(; i < g_kHeapHeaderSize / sizeof(heapMemoryBlock); i++)
	{
		heapMemoryBlock* header = &heapHeader[i];
		if(header->location >= blockLocation && header + header->size <= blockLocation)
			return 0xFFFFFFFF;
		else
			continue;
	}
	headerData->location = blockLocation;
	headerData->size = size;
	headerData->expectedResize = expectedResize;
	return blockLocation;
}