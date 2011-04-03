#include "joh/graphics/opengl/GLBuffer.hpp"
#include "joh/Assert.hpp"

namespace joh
{
    namespace graphics
    {
        GLenum ResourceTypeToGLTarget(ResourceType::Enum type) {
            switch(type) {
                case ResourceType::IndexBuffer:    return GL_ELEMENT_ARRAY_BUFFER_ARB;                
                case ResourceType::VertexBuffer:   return GL_ARRAY_BUFFER_ARB;
                #if defined(GL_VEJOHION_3_0)
                case ResourceType::ConstantBuffer: return GL_UNIFORM_BUFFER;
                #endif
                default:                           return 0;
            }
        }
        
        GLenum ResourceUsageToGLUsage(ResourceUsage::Enum usage) {
            switch(usage) {
                case ResourceUsage::Default: return GL_STATIC_DRAW;
                case ResourceUsage::Static:  return GL_STATIC_DRAW;
                case ResourceUsage::Dynamic: return GL_DYNAMIC_DRAW;
                case ResourceUsage::Stream:  return GL_STREAM_DRAW;                
                default:                      return 0;
            }
        }
        
        GLenum ResourceAccessToGLMapAccess(ResourceAccess::Enum access) {
            switch(access) {
                case ResourceAccess::None:      return GL_READ_ONLY;                
                case ResourceAccess::Read:      return GL_READ_ONLY;                
                case ResourceAccess::Write:     return GL_WRITE_ONLY;                
                case ResourceAccess::ReadWrite: return GL_READ_WRITE;                
                default:                        return 0;
            }
        }
        
        
        GLBuffer::GLBuffer(BufferDescription const& description)
        : Buffer(description) {
            GLenum target = ResourceTypeToGLTarget(GetType());
            JOH_ASSERTF(target != 0, "Error calculating OpenGL target for ResourceType: %i", GetType());
            glGenBuffers(1, &handle_);
            glBindBuffer(target, handle_);
        }

        GLBuffer::GLBuffer(BufferDescription const& description, void const* source)
        : Buffer(description) {
            GLenum target = ResourceTypeToGLTarget(GetType());
            GLenum glusage = ResourceUsageToGLUsage(GetUsage());
            JOH_ASSERT(source != 0, "source data is null");
            JOH_ASSERTF(target != 0, "Error calculating OpenGL target for ResourceType: %i", GetType());
            JOH_ASSERTF(glusage != 0, "Error calculating OpenGL usage for ResourceUsage: %i", GetUsage());
            glGenBuffers(1, &handle_);
            glBindBuffer(target, handle_);
            glBufferData(target, GetSize(), source, glusage);
        }
        
        GLBuffer::~GLBuffer() {
            glDeleteBuffers(1, &handle_);
        }
        
        void GLBuffer::Map(void* data, ResourceAccess::Enum mapType) {
            GLenum glmaptype = ResourceAccessToGLMapAccess(mapType);
            GLenum target = ResourceTypeToGLTarget(GetType());
            JOH_ASSERTF(glmaptype != 0, "Error calculating OpenGL map access for ResourceAccess: %i", mapType);
            JOH_ASSERTF(target != 0, "Error calculating OpenGL target for ResourceType: %i", GetType());
            glBindBuffer(target, handle_);
            data = glMapBuffer(target, glmaptype);
        }
        
        void GLBuffer::Unmap() {
            GLenum target = ResourceTypeToGLTarget(GetType());
            JOH_ASSERTF(target != 0, "Error calculating OpenGL target for ResourceType: %i", GetType());
            glBindBuffer(target, handle_);
            GLboolean result = glUnmapBuffer(target);
            JOH_ASSERT(result != GL_FALSE, "Buffer's contents have become corrupt");
        }
        
        void GLBuffer::SetData(void* source, uint32_t size, uint32_t offset) {
            GLenum target = ResourceTypeToGLTarget(GetType());
            GLenum glusage = ResourceUsageToGLUsage(GetUsage());
            JOH_ASSERT(source != 0, "source data is null");
            JOH_ASSERTF(target != 0, "Error calculating OpenGL target for ResourceType: %i", GetType());
            JOH_ASSERTF(glusage != 0, "Error calculating OpenGL usage for ResourceUsage: %i", GetUsage());
            
            glBindBuffer(target, handle_);
            if (offset == 0) {
                glBufferData(target, size, source, glusage);
                uint32_t elementSize = GetElementSize();
                count_ = size / elementSize;
            }
            else {
                JOH_ASSERT(offset + size <= GetSize(), "offset and size together extends beyond the buffer's allocated memory");
                glBufferSubData(target, offset, size, source);
            }
        }
    }
}
