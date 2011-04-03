#ifndef JOH_GRAPHICS_TEXTURE2DDESCRIPTION_HPP
#define JOH_GRAPHICS_TEXTURE2DDESCRIPTION_HPP

#include "joh/graphics/ResourceDescription.hpp"
#include "joh/graphics/TextureFormat.hpp"

namespace joh
{
    namespace graphics
    {
        class Texture2DDescription : public ResourceDescription
        {            
        public:
            
            Texture2DDescription(size_t width, size_t height, TextureFormat::Enum format, uint8_t mipmaps, ResourceAccess::Enum access, ResourceUsage::Enum usage);
            
            size_t GetWidth() const;
            
            size_t GetHeight() const;
            
            TextureFormat::Enum GetFormat() const;
            
            uint8_t GetMipMapCount() const;
            
            void SetWidth(size_t width);
            
            void SetHeight(size_t height);
            
            void SetFormat(TextureFormat::Enum format);
            
            void SetMipMapCount(uint8_t mipmap);
        
        private:
            
            size_t width_;
            size_t height_;
            TextureFormat::Enum format_;
            uint8_t mipmaps_;
        };
        
                
        inline Texture2DDescription::Texture2DDescription(size_t width, size_t height, TextureFormat::Enum format, uint8_t mipmaps, ResourceAccess::Enum access, ResourceUsage::Enum usage)
        : ResourceDescription(ResourceType::Texture2D, access, usage),
          width_(width),
          height_(height),
          format_(format),
          mipmaps_(mipmaps) {
        }
        
        inline size_t Texture2DDescription::GetWidth() const {
            return width_;
        }
        
        inline size_t Texture2DDescription::GetHeight() const {
            return height_;
        }
        
        inline TextureFormat::Enum Texture2DDescription::GetFormat() const {
            return format_;
        }
        
        inline uint8_t Texture2DDescription::GetMipMapCount() const {
            return mipmaps_;
        }
        
        inline void Texture2DDescription::SetWidth(size_t width) {
            width_ = width;
        }
        
        inline void Texture2DDescription::SetHeight(size_t height) {
            height_ = height;
        }
        
        inline void Texture2DDescription::SetFormat(TextureFormat::Enum format) {
            format_ = format;
        }
        
        inline void Texture2DDescription::SetMipMapCount(uint8_t mipmap) {
            mipmaps_ = mipmap;
        }
    }
}

#endif // JOH_GRAPHICS_TEXTURE2DDESCRIPTION_HPP
