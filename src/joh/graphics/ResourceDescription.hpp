#ifndef JOH_GRAPHICS_RESOURCEDESCRIPTION_HPP
#define JOH_GRAPHICS_RESOURCEDESCRIPTION_HPP

#include "joh/graphics/GraphicsForwards.hpp"
#include "joh/joh.hpp"

namespace joh
{
    namespace graphics
    {
        class ResourceDescription
        {            
        public:
            
            ResourceDescription(ResourceType::Enum type, ResourceAccess::Enum access, ResourceUsage::Enum usage);
            
            ResourceAccess::Enum GetAccess() const;
            
            ResourceType::Enum GetType() const;
            
            ResourceUsage::Enum GetUsage() const;
            
            void SetAccess(ResourceAccess::Enum access);
            
            void SetType(ResourceType::Enum type);
            
            void SetUsage(ResourceUsage::Enum usage);
            
        protected:
            
            uint8_t type_;
            uint8_t access_;
            uint8_t usage_;
        };
        
                
        inline ResourceDescription::ResourceDescription(ResourceType::Enum type, ResourceAccess::Enum access, ResourceUsage::Enum usage)
        : type_(static_cast< uint8_t >(type)),
          access_(static_cast< uint8_t >(access)),
          usage_(static_cast< uint8_t >(usage)) {
        }
        
        inline ResourceAccess::Enum ResourceDescription::GetAccess() const {
            return static_cast< ResourceAccess::Enum >(access_);
        }
        
        inline ResourceType::Enum ResourceDescription::GetType() const {
            return static_cast< ResourceType::Enum >(type_);
        }
        
        inline ResourceUsage::Enum ResourceDescription::GetUsage() const {
            return static_cast< ResourceUsage::Enum >(usage_);
        }
        
        inline void ResourceDescription::SetAccess(ResourceAccess::Enum access) {
            access_ = static_cast< uint8_t >(access);
        }
        
        inline void ResourceDescription::SetType(ResourceType::Enum type) {
            type_ = static_cast< uint8_t >(type);
        }
        
        inline void ResourceDescription::SetUsage(ResourceUsage::Enum usage) {
            usage_ = static_cast< uint8_t >(usage);
        }
    }
}

#endif // JOH_GRAPHICS_RESOURCEDESCRIPTION_HPP
