#ifndef JOH_GRAPHICS_VERTEXFORMAT_HPP
#define JOH_GRAPHICS_VERTEXFORMAT_HPP

#include "joh/graphics/GraphicsForwards.hpp"
#include "joh/Assert.hpp"
#include <vector>

namespace joh
{
    namespace graphics
    {
        class VertexFormat
        {
            friend class Renderer;
            friend class GLRenderer;
                        
        protected:
            typedef std::vector< InputElement > InputElementContainer;
              
        public:
                        
            InputElement const& GetInputElement(uint32_t i) const;
            
            size_t GetInputElementCount() const;
            
            uint32_t GetVertexSize() const;
                        
        protected:
            
            VertexFormat(InputElement const* inputElements, size_t count);
          
        protected:
                      
            InputElementContainer inputElements_;
            uint32_t size_;
        };
        
        
        inline VertexFormat::VertexFormat(InputElement const* inputElements, size_t count)
        : inputElements_(inputElements, inputElements + count),
		  size_(0) {
            JOH_ASSERT(inputElements != 0, "inputElements is null");
            for (size_t i = 0; i < count; ++i) {
                size_ += GetInputElementFormatSize(inputElements[i].format) * inputElements[i].componentCount;
            }
        }
                
        inline InputElement const& VertexFormat::GetInputElement(uint32_t i) const {
            JOH_ASSERT(i < inputElements_.size(), "index 'i' is out of range");
            return inputElements_[i];
        }
        
        inline size_t VertexFormat::GetInputElementCount() const {
            return inputElements_.size();
        }
        
        inline uint32_t VertexFormat::GetVertexSize() const {
            return size_;
        }
        
    }
}

#endif // JOH_GRAPHICS_VERTEXFORMAT_HPP
