#ifndef JOH_GRAPHICS_TEXTURE2D_HPP
#define JOH_GRAPHICS_TEXTURE2D_HPP

#include "joh/graphics/GraphicsForwards.hpp"
#include "joh/graphics/Resource.hpp"	
#include "joh/graphics/Texture2DDescription.hpp"

namespace joh
{
    namespace graphics
    {        
        class Texture2D : public Resource
        {            
        public:
            
            size_t GetWidth() const;
            
            size_t GetHeight() const;
            
            size_t GetMipMapCount() const;
            
            TextureFormat const& GetFormat() const;
            
            virtual void GenerateMipmaps() = 0;
            
            virtual void SetData(void const* data, uint8_t mipLevel = 0) = 0;
            
            virtual void SetData(void const* data, size_t x, size_t y, size_t width, size_t height, uint8_t mipLevel = 0) = 0;
            
            virtual void const* GetData(uint8_t mipLevel = 0) const = 0;
            
        protected:
            
            Texture2D(Texture2DDescription const& description);
            
        protected:
            
            size_t width_;
            size_t height_;
            TextureFormat format_;
            uint8_t numMipmaps_;
        };
        
                
        inline Texture2D::Texture2D(Texture2DDescription const& description)
        : Resource(description),
          width_(description.GetWidth()),
          height_(description.GetHeight()),
          format_(description.GetFormat()),
          numMipmaps_(description.GetMipMapCount()) {
        }
        
        
        inline size_t Texture2D::GetWidth() const {
            return width_;
        }
        
        inline size_t Texture2D::GetHeight() const {
            return height_;
        }
        
        inline size_t Texture2D::GetMipMapCount() const {
            return numMipmaps_;
        }
        
        inline TextureFormat const& Texture2D::GetFormat() const {
            return format_;
        }
    }
}

#endif // JOH_GRAPHICS_TEXTURE2D_HPP
