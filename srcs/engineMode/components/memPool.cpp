
#include "memPool.h"

void MemPool::SetUpMemoryPool(size_t size)
{
	if (memPool != NULL)
		delete memPool;
	memPool = new MemoryPool(size);
}

void *MemPool::Allocate(size_t size)
{
	if (memPool == NULL)
		return (NULL);
	return (memPool->Allocate(size));
}

size_t MemPool::Free(void *ptr)
{
	if (memPool == NULL)
		return (0);
	return (memPool->Free(ptr));
}

void MemPool::Coalesce()
{
	if (memPool == NULL)
		return ;
	memPool->Coalesce();
}
