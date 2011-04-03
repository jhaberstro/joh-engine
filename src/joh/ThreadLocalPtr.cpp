#include "joh/ThreadLocalPtr.hpp"
#include <pthread.h>
#include <cstdlib>

namespace joh
{
    namespace internal
    {
        void set_tss_data(void*& key, void (*cleanup)(void*), void* data, bool destroy) {            
            if (destroy && key) {
                ::pthread_key_delete(*static_cast< ::pthread_key_t const* >(key));
                std::free(key);
                key = 0;
            }
            
            if (key == 0) {
                key = std::malloc(sizeof(::pthread_key_t));
                ::pthread_key_create(static_cast< ::pthread_key_t* >(key), cleanup);
            }
            
            ::pthread_setspecific(*static_cast< ::pthread_key_t* >(key), data);
        }
        
        void* tss_data(void const* key) {
            return key ? ::pthread_getspecific(*static_cast< ::pthread_key_t const* >(key)) : 0;
        }
    }
    
}
