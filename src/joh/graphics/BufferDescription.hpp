#ifndef JOH_GRAPHICS_BUFFERDESCRIPTION_HPP
#define JOH_GRAPHICS_BUFFERDESCRIPTION_HPP

#include "joh/graphics/ResourceDescription.hpp"

namespace joh
{
    namespace graphics
    {
        class BufferDescription : public ResourceDescription
        {            
        public:
            
            BufferDescription(uint32_t count, uint32_t elementSize, ResourceType::Enum type, ResourceAccess::Enum access, ResourceUsage::Enum usage);
            
            uint32_t GetSize() const;
                        
            uint32_t GetCount() const;
            
            void SetCount(uint32_t count);
            
            uint32_t GetElementSize() const;
            
            void SetElementSize(uint32_t elementSize);
            
        protected:
            
            uint32_t count_;
            uint32_t elementSize_;
        };
        
                
        inline BufferDescription::BufferDescription(uint32_t count, uint32_t elementSize, ResourceType::Enum type, ResourceAccess::Enum access, ResourceUsage::Enum usage)
        : ResourceDescription(type, access, usage),
          count_(count),
          elementSize_(elementSize) {
        }
        
        inline uint32_t BufferDescription::GetSize() const {
            return count_ * elementSize_;
        }
        
        inline uint32_t BufferDescription::GetCount() const {
            return count_;
        }
        
        inline void BufferDescription::SetCount(uint32_t count) {
            count_ = count;
        }
        
        inline uint32_t BufferDescription::GetElementSize() const {
            return elementSize_;
        }
        
        inline void BufferDescription::SetElementSize(uint32_t elementSize) {
            elementSize_ = elementSize;
        }
    }
}

#endif // JOH_GRAPHICS_BUFFERDESCRIPTION_HPP
