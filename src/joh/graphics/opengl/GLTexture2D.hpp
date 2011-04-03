#ifndef JOH_GRAPHICS_GLTEXTURE2D_HPP
#define JOH_GRAPHICS_GLTEXTURE2D_HPP

#include "joh/graphics/opengl/OpenGLForwards.hpp"
#include "joh/graphics/Texture2D.hpp"	

namespace joh
{
    namespace graphics
    {        
        class GLTexture2D : public Texture2D
        {            
        public:
            
            GLTexture2D(Texture2DDescription const& description);
            
            GLTexture2D(Texture2DDescription const& description, void const* data);
            
            ~GLTexture2D();
            
            GLuint GetHandle() const;
            
            virtual void GenerateMipmaps();
            
            void SetData(void const* data, uint8_t mipLevel = 0);
            
            virtual void SetData(void const* data, size_t x, size_t y, size_t width, size_t height, uint8_t mipLevel = 0);
            
            virtual void const* GetData(uint8_t mipLevel = 0) const;
            
        private:
            
            bool allocated_;
            GLuint handle_;
            GLint glinternalFormat_;
            GLenum glformat_;
            GLenum gltype_;
        };
        
        

        inline GLuint GLTexture2D::GetHandle() const {
            return handle_;
        }
    }
}

#endif // JOH_GRAPHICS_GLTEXTURE2D_HPP
