#ifndef JOH_GRAPHICS_OPENGL_GLBUFFER_HPP
#define JOH_GRAPHICS_OPENGL_GLBUFFER_HPP

#include "joh/graphics/Buffer.hpp"
#include "joh/graphics/opengl/OpenGLForwards.hpp"

namespace joh
{
    namespace graphics
    {
        class GLBuffer : public Buffer
        {            
        public:
            
            GLBuffer(BufferDescription const& description);

            GLBuffer(BufferDescription const& description, void const* source);
            
            ~GLBuffer();
            
            GLuint GetHandle() const;
            
            GLenum GetTarget() const;
            
            virtual void Map(void* data, ResourceAccess::Enum mapType);
            
            virtual void Unmap();
            
            virtual void SetData(void* source, uint32_t size, uint32_t offset = 0);
                        
        private:
            
            GLuint handle_;
            GLenum target_;
        };
        
                
        inline GLuint GLBuffer::GetHandle() const {
            return handle_;
        }
        
        inline GLenum GLBuffer::GetTarget() const {
            return target_;
        }
        
    }
}

#endif // JOH_GRAPHICS_OPENGL_GLBUFFER_HPP
