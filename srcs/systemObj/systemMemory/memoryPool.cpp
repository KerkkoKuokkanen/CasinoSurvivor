
#include "memoryPool.h"

MemoryPool::MemoryPool(size_t size)
{
	totalMemory = malloc(size);
	if (totalMemory == NULL)
		return ;
	freeList = (FreeBlock*)totalMemory;
	freeList->size = size - sizeof(FreeBlock);
	freeList->next = NULL;
	totalSize = size;
}

MemoryPool::~MemoryPool()
{
	if (totalMemory != NULL)
		free(totalMemory);
}

void* MemoryPool::Allocate(size_t size)
{
	size = (size + sizeof(void*) - 1) & ~(sizeof(void*) - 1);
	FreeBlock* prev = nullptr;
	FreeBlock* current = freeList;
	while (current)
	{
		if (current->size >= size)
		{
			if (current->size >= size + sizeof(FreeBlock) + sizeof(void*))
			{
				FreeBlock* newBlock = (FreeBlock*)((char*)current + sizeof(FreeBlock) + size);
				newBlock->size = current->size - size - sizeof(FreeBlock);
				newBlock->next = current->next;
				current->size = size;
				if (prev)
					prev->next = newBlock;
				else
					freeList = newBlock;
			}
			else
			{
				if (prev)
					prev->next = current->next;
				else
					freeList = current->next;
			}
			return (void*)((char*)current + sizeof(FreeBlock));
		}
		prev = current;
		current = current->next;
	}
	return (NULL);
}

size_t MemoryPool::Free(void *ptr)
{
	if (ptr == NULL)
		return (0);
	FreeBlock *freed = (FreeBlock*)((char*)ptr - sizeof(FreeBlock));
	freed->next = freeList;
	freeList = freed;
	return (freed->size);
}

void MemoryPool::Coalesce()
{
	FreeBlock* current = freeList;
	while (current && current->next)
	{
		if ((char*)current + current->size + sizeof(FreeBlock) == (char*)current->next)
		{
			current->size += current->next->size + sizeof(FreeBlock);
			current->next = current->next->next;
		} else
			current = current->next;
	}
}
