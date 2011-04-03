#ifndef JOH_ALLOCATOR_IPP
#define JOH_ALLOCATOR_IPP

#ifndef JOH_ALLOCATOR_INL_GUARD
	#error "Do not include Allocator.ipp directly. Please include Allocator.hpp"
#endif

#include "joh.hpp"

namespace joh
{
	//=================================================================
	// Allocator
	//=================================================================
	template< typename T >
	inline T* Allocator::MakeNew() {
		return new (Allocate(sizeof(T), alignof(T))) T();
	}

	template< typename T, typename P1 >
	inline T* Allocator::MakeNew(P1 const& p1) {
		return new (Allocate(sizeof(T), alignof(T))) T(p1);
	}

	template< typename T, typename P1, typename P2 >
	inline T* Allocator::MakeNew(P1 const& p1, P2 const& p2) {
		return new (Allocate(sizeof(T), alignof(T))) T(p1, p2);
	}

	template< typename T, typename P1, typename P2, typename P3 >
	inline T* Allocator::MakeNew(P1 const& p1, P2 const& p2, P3 const& p3) {
		return new (Allocate(sizeof(T), alignof(T))) T(p1, p2, p3);
	}

	template< typename T, typename P1, typename P2, typename P3, typename P4 >
	inline T* Allocator::MakeNew(P1 const& p1, P2 const& p2, P3 const& p3, P4 const& p4) {
		return new (Allocate(sizeof(T), alignof(T))) T(p1, p2, p3, p4);
	}
	
	template< typename T >
	inline void Allocator::MakeDelete(T* p) {
		if (p) {
			p->~T();
			Deallocate(p);
		}
	}
}

#endif // JOH_ALLOCATOR_IPP
