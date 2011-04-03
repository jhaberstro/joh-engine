#ifndef JOH_THREADLOCALPTR_HPP
#define JOH_THREADLOCALPTR_HPP

namespace joh
{
    namespace internal
    {   
        void set_tss_data(void*& key, void (*cleanup)(void*), void* data, bool destroy);
        
        void* tss_data(void const* key);
        
        template< typename T >
        void default_cleanup_function(void* data) {
            delete static_cast< T* >(data);
        }
    }
    
    template< typename T >
    class ThreadLocalPtr
    {
    public:
        
        typedef T ValueType;
        typedef T& Reference;
        typedef T const& ConstReference;
        typedef T* Pointer;
        typedef T const* ConstPointer;
                
        typedef void(*CleanupFunction)(void*);
        
    public:
        
        ThreadLocalPtr();
        
        explicit ThreadLocalPtr(CleanupFunction cleanup);
        
        ~ThreadLocalPtr();
        
        Pointer get() const;
        
        Pointer operator->() const;
        
        Reference operator*() const;
        
        Pointer release();
        
        void reset(Pointer value = 0);
        
    private:
                
        ThreadLocalPtr(ThreadLocalPtr const& other);
        
        ThreadLocalPtr& operator=(ThreadLocalPtr const& other);
        
    private:
        
        void* key_;
        CleanupFunction cleanup_;
    };
    
    template< typename T >
    inline ThreadLocalPtr< T >::ThreadLocalPtr()
    : cleanup_(&(internal::default_cleanup_function< ValueType >)) {
    }
    
    template< typename T >
    inline ThreadLocalPtr< T >::ThreadLocalPtr(CleanupFunction cleanup)
    : cleanup_(cleanup) {
    } 
    
    template< typename T >
    inline ThreadLocalPtr< T >::~ThreadLocalPtr() {
        reset();
    }
    
    template< typename T >
    inline typename ThreadLocalPtr< T >::Pointer ThreadLocalPtr< T >::get() const {
        return static_cast< Pointer >(internal::tss_data(key_));
    }
    
    template< typename T >
    inline typename ThreadLocalPtr< T >::Pointer ThreadLocalPtr< T >::operator->() const {
        return get();
    }
    
    template< typename T >
    inline typename ThreadLocalPtr< T >::Reference ThreadLocalPtr< T >::operator*() const {
        return *get();
    }
    
    template< typename T >
    inline typename ThreadLocalPtr< T >::Pointer ThreadLocalPtr< T >::release() {
        ConstPointer result = get();
        internal::set_tss_data(key_, cleanup_, 0, false);
        return result;
    }
    
    template< typename T >
    inline void ThreadLocalPtr< T >::reset(Pointer value) {
        if (get() != value) {
            internal::set_tss_data(key_, cleanup_, value, true);
        }
    }
}

#endif // JOH_THREADLOCALPTR_HPP
