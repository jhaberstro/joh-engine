#ifndef JOH_GRAPHICS_SHADERDESCRIPTION_HPP
#define JOH_GRAPHICS_SHADERDESCRIPTION_HPP

#include "joh/graphics/ResourceDescription.hpp"

namespace joh
{
    namespace graphics
    {
        class ShaderDescription : public ResourceDescription
        {            
        public:
            
            ShaderDescription(char const* vsFile, char const* fragFile, char const** attributeNames, uint32_t attributesCount);

            ShaderDescription(char const* vsFile, char const* fragFile, char const* geomShader, char const** attributeNames, uint32_t attributesCount);
            
            char const* GetVertexShaderFile() const;
            
            char const* GetFragmentShaderFile() const;
            
            char const* GetGeometryShaderFile() const;
            
            char const** GetAttributeNames() const;
            
            uint32_t GetAttributeNamesCount() const;
            
        private:
            
            char const* vsFile_;
            char const* fragFile_;
            char const* geomShader_;
            char const** attributeNames_;
            uint32_t attributesCount_;
        };
        
                
        inline ShaderDescription::ShaderDescription(char const* vsFile, char const* fragFile, char const** attributeNames, uint32_t attributesCount)
        : ResourceDescription(ResourceType::ShaderResource, ResourceAccess::None, ResourceUsage::Default),
          vsFile_(vsFile),
          fragFile_(fragFile),
          geomShader_(0),
          attributeNames_(attributeNames),
          attributesCount_(attributesCount) {
        }

        inline ShaderDescription::ShaderDescription(char const* vsFile, char const* fragFile, char const* geomShader, char const** attributeNames, uint32_t attributesCount)
        : ResourceDescription(ResourceType::ShaderResource, ResourceAccess::None, ResourceUsage::Default),
          vsFile_(vsFile),
          fragFile_(fragFile),
          geomShader_(geomShader),
          attributeNames_(attributeNames),
          attributesCount_(attributesCount) {
        }
        
        inline char const* ShaderDescription::GetVertexShaderFile() const {
            return vsFile_;
        }
        
        inline char const* ShaderDescription::GetFragmentShaderFile() const {
            return fragFile_;
        }
        
        inline char const* ShaderDescription::GetGeometryShaderFile() const {
            return geomShader_;
        }
        
        inline char const** ShaderDescription::GetAttributeNames() const {
            return attributeNames_;
        }
        
        inline uint32_t ShaderDescription::GetAttributeNamesCount() const {
            return attributesCount_;
        }
    }
}

#endif // JOH_GRAPHICS_SHADERDESCRIPTION_HPP
