#include "Allocator.hpp"
#include "nedmalloc/nedmalloc.h"
#include <cassert>

namespace joh
{
	//=================================================================
	// Allocator
	//=================================================================
	Allocator::~Allocator() {
		assert(size_ == 0);
		assert(allocations_ == 0);
	}
	
	
	//=================================================================
	// HeapAllocator
	//=================================================================
	void* HeapAllocator::Allocate(size_t size, size_t align) {
        void* p = nedalloc::nedmemalign(align, size);
        size_ += nedalloc::nedblksize(p);
        allocations_ += 1;
        return p;
	}
	
	void HeapAllocator::Deallocate(void* p) {
        size_ -= nedalloc::nedblksize(p);
        allocations_ -= 1;
        nedalloc::nedfree(p);
	}
}
