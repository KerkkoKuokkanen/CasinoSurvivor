
#ifndef MEM_POOL_COMPONENT_H
# define MEM_POOL_COMPONENT_H

# include "memoryPool.h"
# include "customComponent.h"
# include "componentRegistry.h"

class MemPool : public CustomComponent
{
	private:
		MemoryPool *memPool = NULL;
	public:
		void SetUpMemoryPool(size_t size);
		void *Allocate(size_t size);
		size_t Free(void *ptr);
		void Coalesce();
};

REGISTER_COMPONENT(MemPool);

#endif
