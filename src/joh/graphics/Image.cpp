#include "joh/graphics/Image.hpp"
#include "joh/math/HalfFloat.hpp"

namespace joh
{
    namespace graphics
    {
        namespace
        {
            inline size_t Index(size_t x, size_t y, size_t width, size_t channels) {
                return (x * channels) + (y * width * channels);
            }
            
#if 0
            void BuildMipMapOld(uint8_t* dst, uint8_t const* src, size_t width, size_t height, size_t channels) {
                size_t const blockSize = 2;
                size_t mipWidth = width >> 1, mipHeight = height >> 1;
                
                for (size_t j = 0; j < mipHeight; ++j) {
                    for (size_t i = 0; i < mipWidth; ++i) {
                        for (size_t c = 0; c < channels; ++c) {
                            size_t const index = (j * blockSize) * width * channels + (i * blockSize) * channels + c;
                            size_t ublock = blockSize, vblock = blockSize;
                            if (blockSize * (i + 1) > width) {
                                ublock = width - i * blockSize;
                            }
                            
                            if (blockSize * (j + 1) > height) {
                                vblock = height - j * blockSize;
                            }
                            
                            size_t blockArea = ublock * vblock;
                            size_t sum = blockArea >> 1;
                            sum += src[index];                                  // index + 0 * width * channels + 0 * channels
                            sum += src[index + channels];                       // index + 0 * width * channels + 1 * channels
                            sum += src[index + width * channels];               // index + 1 * width * channels + 0 * channels
                            sum += src[index + width * channels + channels];
                            dst[j * mipWidth * channels + i * channels + c] = static_cast< uint8_t >(sum / blockArea);
                        }
                    }
                }
            }
#endif
            
            template< typename T >
            void BuildMipMap(T* dst, T const* src, size_t srcwidth, size_t srcheight, TextureFormat const& format) {
                size_t channels = format.GetChannelCount();
                for (size_t y = 0; y < srcheight; y += 2) {
                    for (size_t x = 0; y < srcwidth; y += 2) {
                        for (size_t c = 0; c < channels; ++c) {
                            T sum = (
                                src[Index(x,     y,     srcwidth, channels) + c] +
                                src[Index(x + 1, y,     srcwidth, channels) + c] +
                                src[Index(x,     y + 1, srcwidth, channels) + c] + 
                                src[Index(x + 1, y + 1, srcwidth, channels) + c]
                            );
                            
                            sum /= 4;
                            *dst++ = sum;
                        }
                    }                    
                }
            }
        }
        
        
        void Image::GenerateMipMaps(size_t mipMapCount) {
            JOH_ASSERT(mipMapCount > 0, "'mipMapCount' is negative");
            JOH_ASSERT(GetFormat().IsPlain() || GetFormat().IsPacked(), "Unable to generate mipmaps for image's texture format");
            
            // Grow pixel data if needed
            mipMapCount = std::min(GetMaxMipMapCount(GetWidth(), GetHeight()), mipMapCount);
            if (mipMapCount > mipMapCount_) {
                mipMapCount_ = mipMapCount;
                pixels_ = static_cast< uint8_t* >(std::realloc(pixels_, GetSize()));
            }
            
            // Generate the actual mipmaps
            #if 0
            uint8_t* src = GetPixels(0), dst = GetPixels(1);
            for (size_t level = 1; level < GetMipMapCount(); ++level) {
                size_t width = GetWidth(level - 1);
                size_t height = GetHeight(level - 1);
                uint8_t srcSize = dst - src, dstSize = GetWidth(level) * GetWidth(level) * format_.GetBytesPerPixel();
                
            }
            #endif
            
            uint8_t *src = const_cast< uint8_t* >(GetPixels(0)), *dst = const_cast< uint8_t* >(GetPixels(1));
            for (size_t level = 1; level < GetMipMapCount(); ++level) {
                size_t width = GetWidth(level - 1);
                size_t height = GetHeight(level - 1);
                intptr_t srcSize = dst - src;
                size_t dstSize = GetWidth(level) * GetWidth(level) * format_.GetBytesPerPixel();
                uint32_t bytesPerChannel = format_.GetBytesPerChannel();

                if (format_.IsUnsignedNormalized() || format_.IsUnsignedInteger()) {
                    if (bytesPerChannel == 8) {
                        BuildMipMap(reinterpret_cast< uint8_t* >(src), reinterpret_cast< uint8_t const* >(dst), width, height, format_);
                    }
                    else if (bytesPerChannel == 16) {
                        BuildMipMap(reinterpret_cast< uint16_t* >(src), reinterpret_cast< uint16_t const* >(dst), width, height, format_);
                    }
                    else if (bytesPerChannel == 32) {
                        BuildMipMap(reinterpret_cast< uint32_t* >(src), reinterpret_cast< uint32_t const* >(dst), width, height, format_);
                    }
                }
                else if (format_.IsSignedNormalized() || format_.IsSignedInteger()) {
                    if (bytesPerChannel == 8) {
                        BuildMipMap(reinterpret_cast< int8_t* >(src), reinterpret_cast< int8_t const* >(dst), width, height, format_);
                    }
                    else if (bytesPerChannel == 16) {
                        BuildMipMap(reinterpret_cast< int16_t* >(src), reinterpret_cast< int16_t const* >(dst), width, height, format_);
                    }
                    else if (bytesPerChannel == 32) {
                        BuildMipMap(reinterpret_cast< int32_t* >(src), reinterpret_cast< int32_t const* >(dst), width, height, format_);
                    }
                }
                else if (format_.IsFloat()) {
                    if (bytesPerChannel == 16) {
                        BuildMipMap(reinterpret_cast< joh::math::HalfFloat* >(src), reinterpret_cast< joh::math::HalfFloat const* >(dst), width, height, format_);
                    }
                    else if (bytesPerChannel == 32) {
                        BuildMipMap(reinterpret_cast< float* >(src), reinterpret_cast< float const* >(dst), width, height, format_);
                    }
                }
                else {
                    JOH_ASSERT(false, "Unsupported mipmap generation for Image's texture format");
                }
                
                src += srcSize;
                dst += dstSize;
            }
        }
    }
}
