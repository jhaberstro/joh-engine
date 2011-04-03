#ifndef JOH_GRAPHICS_IMAGE_HPP
#define JOH_GRAPHICS_IMAGE_HPP

#include "joh/graphics/TextureFormat.hpp"
#include <algorithm>
#include <cstring>

namespace joh
{
    namespace graphics
    {
        class Image
        {            
        public:
            
            static size_t GetMaxMipMapCount(size_t width, size_t height);
            
        public:
            
            Image();
            
            Image(Image const& image);
            
            Image(TextureFormat format, size_t const width, size_t const height, size_t const mipMapCount);

            Image(uint8_t const* data, TextureFormat format, size_t const width, size_t const height, size_t const mipMapCount);
                        
            ~Image();
            
            void Allocate(TextureFormat format, size_t const width, size_t const height, size_t const mipMapCount);
            
            void SetData(uint8_t const* data, TextureFormat format, size_t const width, size_t const height, size_t const mipMapCount);
            
			void DestroyAndClearData();
            
            void GenerateMipMaps(size_t mipMapCount);
            
            void Convert(TextureFormat const newFormat);
			
			void SetPixel(size_t const x, size_t const y, uint8_t const* pixelData);

			void SetPixel(size_t const x, size_t const y, uint8_t const* pixelData, size_t mipmap);
            
            uint8_t const* GetPixels() const;
            
            uint8_t const* GetPixels(size_t mipMapLevel) const;
            
            size_t GetSize() const;
            
            size_t GetMipMapCount() const;
            
            size_t GetWidth() const;
            
            size_t GetHeight() const;
            
            size_t GetWidth(size_t mipMapLevel) const;
            
            size_t GetHeight(size_t mipMapLevel) const;
            
            TextureFormat const& GetFormat() const;
            
        private:
            
            size_t GetMaxMipMapCount() const;
                                    
        private:
            
            uint8_t* pixels_;
            TextureFormat format_;
            size_t width_;
            size_t height_;
            size_t mipMapCount_;
        };

                
        inline size_t Image::GetMaxMipMapCount(size_t width, size_t height) {
            JOH_ASSERT(width > 0, "'width' must be greater than zero");
            JOH_ASSERT(height > 0, "'height' must be greater than zero");
            
            size_t count = 0;
            while (width != 1 && height != 1) {
                width >>= 1;
                height >>= 1;
                ++count;
            }
            
            return count;
        }
        
        inline Image::Image()
        : pixels_(0),
          format_(TextureFormat::Unknown),
          width_(0),
          height_(0),
          mipMapCount_(0) {
        }
        
        inline Image::Image(Image const& image)
        : pixels_(new uint8_t[image.GetSize()]),
          format_(image.GetFormat()),
          width_(image.GetWidth()),
          height_(image.GetHeight()),
          mipMapCount_(image.GetMipMapCount()) {
              std::memcpy(pixels_, image.GetPixels(), GetSize());
        }
        
        inline Image::Image(TextureFormat format, size_t const width, size_t const height, size_t const mipMapCount)
 		: pixels_(0) {
            Allocate(format, width, height, mipMapCount);
        }

        inline Image::Image(uint8_t const* data, TextureFormat format, size_t const width, size_t const height, size_t const mipMapCount)
 		: pixels_(0) {
            SetData(data, format, width, height, mipMapCount);
        }
                    
        inline Image::~Image() {
            DestroyAndClearData();
        }
        
        inline void Image::Allocate(TextureFormat format, size_t const width, size_t const height, size_t const mipMapCount) {
            JOH_ASSERT(format != TextureFormat::Unknown, "'format' is an invalid TextureFormat");
            
            if (pixels_) {
                DestroyAndClearData();
            }
            
            format_ = format;
            width_ = width;
            height_ = height;
            mipMapCount_ = std::min(mipMapCount, GetMaxMipMapCount(width, height));
            pixels_ = new uint8_t[GetSize()];
        }
        
        inline void Image::SetData(uint8_t const* data, TextureFormat format, size_t const width, size_t const height, size_t const mipMapCount) {
            JOH_ASSERT(format != TextureFormat::Unknown, "'format' is an invalid TextureFormat");
            JOH_ASSERT(data != 0, "'data' is null");
            
            if (pixels_) {
                DestroyAndClearData();
            }
            
            format_ = format;
            width_ = width;
            height_ = height;
            mipMapCount_ = std::min(mipMapCount, GetMaxMipMapCount(width, height));
            pixels_ = new uint8_t[GetSize()];
            std::memcpy(pixels_, data, GetSize());
            
        }

		inline void Image::DestroyAndClearData() {
			//delete [] pixels_;
		}
        
        inline void Image::Convert(TextureFormat const newFormat) {
            JOH_ASSERT(false, "Not Implemented");
        }
        
		inline void Image::SetPixel(size_t const x, size_t const y, uint8_t const* pixelData) {
			JOH_ASSERT(pixelData, "pixelData is null");
			size_t w = GetWidth(), h = GetHeight();
			JOH_ASSERT(x < w, "x is out of range for desired mip level");
			JOH_ASSERT(y < h, "y is out of range for desired mip level");
			size_t offset = ((y * w) + x) * format_.GetBytesPerPixel();
			for (size_t i = offset; i < offset + format_.GetBytesPerPixel(); ++i) {
				pixels_[i] = pixelData[i - offset];
			}
			//std::memcpy(pixels_ + offset, &pixelData, format_.GetBytesPerPixel());
		}
		
		inline void Image::SetPixel(size_t const x, size_t const y, uint8_t const* pixelData, size_t mipmap) {
			JOH_ASSERT(pixelData, "pixelData is null");
			size_t w = GetWidth(), h = GetHeight();
            size_t offset = 0;
            while (mipmap && w != 1 && h != 1) {
                offset += w * h;
                w >>= 1;
                h >>= 1;
                --mipmap;
            }
			
			JOH_ASSERT(x < w, "x is out of range for desired mip level");
			JOH_ASSERT(y < h, "y is out of range for desired mip level");
			offset += ((y * w) + x) * format_.GetBytesPerPixel();;
			for (size_t i = offset; i < offset + format_.GetBytesPerPixel(); ++i) {
				pixels_[i] = pixelData[i];
			}
			//std::memcpy(pixels_ + offset, &pixelData, format_.GetBytesPerPixel());
		}

        inline uint8_t const* Image::GetPixels() const {
            return pixels_;
        }
        
        inline uint8_t const* Image::GetPixels(size_t mipMapLevel) const {
            JOH_ASSERT(0 <= mipMapLevel && mipMapLevel < mipMapCount_, "mipMapLevel is out of bounds");
            size_t width = GetWidth(), height = GetHeight();
            size_t offset = 0;
            while (mipMapLevel && width != 1 && height != 1) {
                offset += width * height;
				width >>= 1;
				height >>= 1;
                --mipMapLevel;
            }
            
            return pixels_ + (offset * format_.GetBytesPerPixel());
        }
        
        inline size_t Image::GetSize() const {            
            size_t width = GetWidth(), height = GetHeight();
            size_t size = width * height;
            size_t mipMap = 1;
            while (mipMap < GetMipMapCount() && width != 1 && height != 1) {
                width >>= 1;
                height >>= 1;
                size += width * height;
            }
            
            return size * format_.GetBytesPerPixel();
        }
       
        inline size_t Image::GetMipMapCount() const {
            return mipMapCount_;
        }
        
        inline size_t Image::GetWidth() const {
            return width_;
        }
        
        inline size_t Image::GetHeight() const {
            return height_;
        }
        
        inline size_t Image::GetWidth(size_t mipMapLevel) const {
            JOH_ASSERT(0 <= mipMapLevel && mipMapLevel < mipMapCount_, "mipMapLevel is out of bounds");
            return GetWidth() >> mipMapLevel;
        }
        
        inline size_t Image::GetHeight(size_t mipMapLevel) const {
            JOH_ASSERT(0 <= mipMapLevel && mipMapLevel < mipMapCount_, "mipMapLevel is out of bounds");
            return GetHeight() >> mipMapLevel;
        }
        
        inline TextureFormat const& Image::GetFormat() const {
            return format_;
        }
    }
}

#endif // JOH_GRAPHICS_IMAGE_HPP
