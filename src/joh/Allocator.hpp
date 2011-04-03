#ifndef JOH_ALLOCATOR_HPP
#define JOH_ALLOCATOR_HPP

#include <cstddef>

namespace joh
{
	//=================================================================
	// Allocator
	//=================================================================
	class Allocator
	{
	public:
		
		virtual ~Allocator();
		
		template< typename T >
		T* MakeNew();
		
		template< typename T, typename P1 >
		T* MakeNew(P1 const& p1);
		
		template< typename T, typename P1, typename P2 >
		T* MakeNew(P1 const& p1, P2 const& p2);
		
		template< typename T, typename P1, typename P2, typename P3 >
		T* MakeNew(P1 const& p1, P2 const& p2, P3 const& p3);
		
		template< typename T, typename P1, typename P2, typename P3, typename P4 >
		T* MakeNew(P1 const& p1, P2 const& p2, P3 const& p3, P4 const& p4);
		
		template< typename T >
		void MakeDelete(T* p);
		
		virtual void* Allocate(size_t size, size_t align) = 0;
		
		virtual void Deallocate(void* p) = 0;
				
	protected:
		
		size_t size_;
		size_t allocations_;
	};
	
	
	//=================================================================
	// HeapAllocator
	//=================================================================
	class HeapAllocator : public Allocator
	{
	public:
		
		virtual void* Allocate(size_t size, size_t align);
		
		virtual void Deallocate(void* p);
	};
}


#define JOH_ALLOCATOR_INL_GUARD
#include "Allocator.ipp"
#undef JOH_ALLOCATOR_INL_GUARD

#endif // JOH_ALLOCATOR_HPP
