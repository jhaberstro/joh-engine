#ifndef JOH_GRAPHICS_BUFFER_HPP
#define JOH_GRAPHICS_BUFFER_HPP

#include "joh/graphics/GraphicsForwards.hpp"
#include "joh/graphics/Resource.hpp"
#include "joh/graphics/BufferDescription.hpp"

namespace joh
{
    namespace graphics
    {
        class Buffer : public Resource
        {            
        public:
            
            virtual void Map(void* data, ResourceAccess::Enum mapType) = 0;
            
            virtual void Unmap() = 0;
            
            virtual void SetData(void* source, uint32_t size, uint32_t offset = 0) = 0;
            
            uint32_t GetSize() const;
                        
            uint32_t GetElementSize() const;
            
        protected:
            
            Buffer(BufferDescription const& description);
            
        protected:
            
            uint32_t count_;
            uint32_t elementSize_;
        };
        
                
        inline Buffer::Buffer(BufferDescription const& description)
        : Resource(description),
          count_(description.GetCount()),
          elementSize_(description.GetElementSize()) {
        }
        
        inline uint32_t Buffer::GetSize() const {
            return count_ * elementSize_;
        }
        
        inline uint32_t Buffer::GetElementSize() const {
            return elementSize_;
        }
    }
}

#endif // JOH_GRAPHICS_BUFFER_HPP
