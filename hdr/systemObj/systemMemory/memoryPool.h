
#ifndef MEMORY_POOL_H
# define MEMORY_POOL_H

#include <stdlib.h>

# define KB_SIZE 1024
# define MB_SIZE 1048576

struct FreeBlock
{
	size_t size;
	FreeBlock *next;
};

//Best used with allocations of same size
//Outperforms malloc if the allocation sizes remain consistent
class MemoryPool
{
	private:
		void *totalMemory = NULL;
		size_t totalSize = 0;
		FreeBlock *freeList;
	public:
		MemoryPool(size_t size);
		~MemoryPool();
		void *Allocate(size_t size);
		size_t Free(void *ptr);
		void Coalesce();
};

#endif
