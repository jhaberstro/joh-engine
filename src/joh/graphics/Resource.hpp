#ifndef JOH_GRAPHICS_RESOURCE_HPP
#define JOH_GRAPHICS_RESOURCE_HPP

#include "joh/graphics/GraphicsForwards.hpp"
#include "joh/graphics/ResourceDescription.hpp"

namespace joh
{
    namespace graphics
    {
        class Resource
        {            
        public:
            
            Resource();
            
            Resource(ResourceDescription const& description);
            
            virtual ~Resource();
            
            ResourceAccess::Enum GetAccess() const;
            
            ResourceType::Enum GetType() const;
            
            ResourceUsage::Enum GetUsage() const;
            
        private:
            
            uint8_t access_;
            uint8_t type_;
            uint8_t usage_;
        };
        
                
        inline Resource::Resource()
        : access_(ResourceAccess::None),
          type_(ResourceType::Unknown),
          usage_(ResourceUsage::Default) {
        }
        
        inline Resource::Resource(ResourceDescription const& description)
        : access_(static_cast< uint8_t >(description.GetAccess())),
          type_(static_cast< uint8_t >(description.GetType())),
          usage_(static_cast< uint8_t >(description.GetUsage())) {
        }
        
        inline Resource::~Resource() {
        }
        
        inline ResourceAccess::Enum Resource::GetAccess() const {
            return static_cast< ResourceAccess::Enum >(access_);
        }
        
        inline ResourceType::Enum Resource::GetType() const {
            return static_cast< ResourceType::Enum >(type_);
        }
        
        inline ResourceUsage::Enum Resource::GetUsage() const {
            return static_cast< ResourceUsage::Enum >(usage_);
        }
    }
}

#endif // JOH_GRAPHICS_RESOURCE_HPP
