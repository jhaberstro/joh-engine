#ifndef JOH_GRAPHICS_TEXTUREFORMAT_HPP
#define JOH_GRAPHICS_TEXTUREFORMAT_HPP

#include "joh/Assert.hpp"
#include "joh/joh.hpp"

namespace joh
{
    namespace graphics
    {   
        class TextureFormat
        {
        public:
            
            // [Type][Components][Components Bit Count]
            enum Enum
            {
                Unknown = 0,
                
                // Unsigned normalized formats
                UNormRGB8 = 1,
                UNormRGBA8 = 2,
                UNormRGB16 = 3,
                UNormRGBA16 = 4,
                
                // Signed normalized formats
                SNormRGB8 = 5,
                SNormRGBA8 = 6,
                SNormRGB16 = 7,
                SNormRGBA16 = 8,
                
                // Unsigned integer formats
                UIntRGB16 = 9,
                UIntRGBA16 = 10,
                UIntRGB32 = 11,
                UIntRGBA32 = 12,
                
                // Signed integer formats
                SIntRGB16 = 13,
                SIntRGBA16 = 14,
                SIntRGB32 = 15,
                SIntRGBA32 = 16,
                
                // Float formats
                FloatRGB16 = 17,
                FloatRGBA16 = 18,
                FloatRGB32 = 19,
                FloatRGBA32 = 20,
                
                // Packed formats
                FloatR11G11B10 = 21,
                UNormR5G6R5 = 22,
                UNormR5G5R5A1 = 23,
                UNormRGBA4 = 24,
                UNormR10G10B10A2 = 25,
                
                // Depth(-Stencil) formats
                UNormDepth16 = 26,
                UNormDepth24 = 27,
                UNormDepth24Stencil8 = 28,
                FloatDepth32 = 29,
                
                // Compressed formats
                DXT1 = 30,
                DXT3 = 31,
                DXT5 = 32,

				// sRGB
				UNormSRGB8 = 33,
				UNormSRGB8A8 = 34
            };
            
        public:
            
            TextureFormat();
            
            TextureFormat(TextureFormat::Enum format);
            
            TextureFormat::Enum GetFormat() const;
            
            bool IsPlain() const;
            
            bool IsPacked() const;
            
            bool IsDepth() const;
            
            bool IsStencil() const;
            
            bool IsCompressed() const;
            
            bool IsUnsignedNormalized() const;
            
            bool IsSignedNormalized() const;
            
            bool IsUnsignedInteger() const;
            
            bool IsSignedInteger() const;
            
            bool IsFloat() const;
            
            uint32_t GetChannelCount() const;
            
            uint32_t GetBytesPerChannel() const;
            
            uint32_t GetBytesPerPixel() const;
            
            uint32_t GetSize(uint32_t width, uint32_t height) const;
                        
        private:
            
            TextureFormat::Enum format_;
            uint32_t channelCount_;
            uint32_t bytesPerChannel_;
            uint32_t bytesPerPixel_;
        };
        
        
        inline bool operator==(TextureFormat const& lhs, TextureFormat const& rhs) {
            return lhs.GetFormat() == rhs.GetFormat();
        } 
        
        inline bool operator!=(TextureFormat const& lhs, TextureFormat const& rhs) {
            return lhs.GetFormat() != rhs.GetFormat();
        }
        
        inline bool operator==(TextureFormat const& lhs, TextureFormat::Enum const rhs) {
            return lhs.GetFormat() == rhs;
        } 
        
        inline bool operator!=(TextureFormat const& lhs, TextureFormat::Enum const rhs) {
            return lhs.GetFormat() != rhs;
        }
        
        inline bool operator==(TextureFormat::Enum const lhs, TextureFormat const& rhs) {
            return lhs == rhs.GetFormat();
        } 
        
        inline bool operator!=(TextureFormat::Enum const lhs, TextureFormat const& rhs) {
            return lhs != rhs.GetFormat();
        }
        
        inline TextureFormat::TextureFormat()
        : format_(Unknown) {
        }
        
        inline TextureFormat::TextureFormat(TextureFormat::Enum format)
        : format_(format) {
            static uint32_t const channels[] = {
                0,
                3, 4, 3, 4,
                3, 4, 3, 4,
                3, 4, 3, 4,
                3, 4, 3, 4,
                3, 4, 3, 4,
                3, 3, 4, 4, 4,
                1, 1, 2, 1,
                3, 4, 4,
				3, 4
            };
            
            static uint32_t const bpc[] = {
                0,
                1, 1, 2, 2,
                1, 1, 2, 2,
                2, 2, 4, 4,
                2, 2, 4, 4,
                2, 2, 4, 4,
               	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				1, 1
            };
            
            static uint32_t const bpp[] = {
				0,
                3, 4, 6, 8,
                3, 4, 6, 8,
                6, 8, 12, 16,
                6, 8, 12, 16,
                6, 8, 12, 16,
                4, 2, 2, 2, 4,
                2, 3, 4, 4,
				0, 0, 0,
				3, 4
            };
            
            channelCount_ = channels[format_];
            //BranchlessTernary(!(IsPacked() || IsDepth() || IsCompressed()), bpc[format_], 0U, bytesPerChannel_);
            if (!(IsPacked() || IsDepth() || IsCompressed())) {
				bytesPerChannel_ = bpc[format_];
			}
			else {
				bytesPerChannel_ = 0u;
			}
			
			//BranchlessTernary(!IsCompressed(), bpp[format_], 0U, bytesPerPixel_);
			if (!IsCompressed()) {
				bytesPerPixel_ = bpp[format_];
			}
			else {
				bytesPerPixel_ = 0u;
			}
        }
        
        inline TextureFormat::Enum TextureFormat::GetFormat() const {
            return format_;
        }
        
        inline bool TextureFormat::IsPlain() const {
            return format_ <= SIntRGBA32;
        }
        
        inline bool TextureFormat::IsPacked() const {
            return FloatR11G11B10 <= format_ && format_ <= UNormR10G10B10A2;
        }
        
        inline bool TextureFormat::IsDepth() const {
            return UNormDepth16 <= format_ && format_ <= FloatDepth32;
        }
        
        inline bool TextureFormat::IsStencil() const {
            return format_ == UNormDepth24Stencil8;
        }
        
        inline bool TextureFormat::IsCompressed() const {
            return format_ >= DXT1 && format_ <= DXT5;
        }
        
        inline bool TextureFormat::IsUnsignedNormalized() const {
            return (UNormRGB8 <= format_ && format_ <= UNormRGBA16) || (UNormSRGB8 <= format_ && format_ <= UNormSRGB8A8);
        }
        
        inline bool TextureFormat::IsSignedNormalized() const {
            return SNormRGB8 <= format_ && format_ <= SNormRGBA16;
        }
        
        inline bool TextureFormat::IsUnsignedInteger() const {
            return UIntRGB16 <= format_ && format_ <= UIntRGBA32;
        }
        
        inline bool TextureFormat::IsSignedInteger() const {
            return UIntRGB16 <= format_ && format_ <= UIntRGBA32;
        }
        
        inline bool TextureFormat::IsFloat() const {
            return FloatRGB16 <= format_ && format_ <= FloatRGBA32;
        }
        
        inline uint32_t TextureFormat::GetChannelCount() const {
            return channelCount_;
        }
        
        inline uint32_t TextureFormat::GetBytesPerChannel() const {
            JOH_ASSERT(!IsPacked(), "Unable to retrieve bytes per channel for packed format");
            JOH_ASSERT(!IsDepth(), "Unable to retrieve bytes per channel for depth format");
            JOH_ASSERT(!IsCompressed(), "Unable to retrieve bytes per channel for compressed format");
            return bytesPerChannel_;
        }
        
        inline uint32_t TextureFormat::GetBytesPerPixel() const {
            return bytesPerPixel_;
        }
        
        inline uint32_t TextureFormat::GetSize(uint32_t width, uint32_t height) const {
            if (!IsCompressed()) {
                return width * height * GetBytesPerPixel();
            }
            else if (format_ == DXT1) {
                return ((width + 3) >> 2) * ((height + 3) >> 2) * 8;
            }
            else if (format_ == DXT3 || format_ == DXT5) {
                return ((width + 3) >> 2) * ((height + 3) >> 2) * 16; 
            }
            
            return 0;
        }
    }
}

#endif // JOH_GRAPHICS_TEXTUREFORMAT_HPP
