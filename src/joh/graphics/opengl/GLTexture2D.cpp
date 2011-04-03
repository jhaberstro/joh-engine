#include "joh/graphics/opengl/GLTexture2D.hpp"

namespace joh
{
    namespace graphics
    {
        namespace
        {
            GLint ConvertTextureFormatToGLInternalFormat(TextureFormat const& format) {
                switch(format.GetFormat()) {
                    // Unsigned normalized formats
                    case TextureFormat::UNormRGB8:
                        return GL_RGB8;
                    case TextureFormat::UNormRGBA8:
                        return GL_RGBA8;
                    case TextureFormat::UNormRGB16:
                        return GL_RGB16;
                    case TextureFormat::UNormRGBA16:
                        return GL_RGBA16;

                    // Signed normalized formats
                    #if defined(GL_VEJOHION_3_0)
                    case TextureFormat::SNormRGB8:
                        return GL_RGB8_SNORM;
                    case TextureFormat::SNormRGBA8:
                        return GL_RGBA8_SNORM;
                    case TextureFormat::SNormRGB16:
                        return GL_RGB16_SNORM;
                    case TextureFormat::SNormRGBA16:
                        return GL_RGBA16_SNORM;
                    #endif

                    #if defined(GL_EXT_texture_integer)
                    // Unsigned integer formats
                    case TextureFormat::UIntRGB16:
                        return GL_RGB16UI_EXT;
                    case TextureFormat::UIntRGBA16:
                        return GL_RGBA16UI_EXT;
                    case TextureFormat::UIntRGB32:
                        return GL_RGB32UI_EXT;
                    case TextureFormat::UIntRGBA32:
                        return GL_RGBA32UI_EXT;

                    // Signed integer formats
                    case TextureFormat::SIntRGB16:
                        return GL_RGB16I_EXT;
                    case TextureFormat::SIntRGBA16:
                        return GL_RGBA16I_EXT;
                    case TextureFormat::SIntRGB32:
                        return GL_RGB32I_EXT;
                    case TextureFormat::SIntRGBA32:
                        return GL_RGBA32I_EXT;
                    #endif

                    // Float formats
                    #if defined(GL_ARB_texture_float)
                    case TextureFormat::FloatRGB16:
                        return GL_RGB16F_ARB;
                    case TextureFormat::FloatRGBA16:
                        return GL_RGBA16F_ARB;
                    case TextureFormat::FloatRGB32:
                        return GL_RGB32F_ARB;
                    case TextureFormat::FloatRGBA32:
                        return GL_RGBA32F_ARB;
                    #endif

                    // Packed formats
                    #if defined(GL_EXT_packed_float)
                    case TextureFormat::FloatR11G11B10:
                        return GL_R11F_G11F_B10F_EXT;
                    #elif defined(GL_VEJOHION_3_0)
                    case TextureFormat::FloatR11G11B10:
                        return GL_R11F_G11F_B10F;
                    #endif
                    case TextureFormat::UNormR5G6R5:
                        return GL_RGB5;
                    case TextureFormat::UNormR5G5R5A1: 
                        return GL_RGB5_A1;
                    case TextureFormat::UNormRGBA4:
                        return GL_RGBA4;
                    case TextureFormat::UNormR10G10B10A2:
                        return GL_RGB10_A2;

                    // Depth(-Stencil) formats
                    case TextureFormat::UNormDepth16:
                        return GL_DEPTH_COMPONENT16;
                    case TextureFormat::UNormDepth24:
                        return GL_DEPTH_COMPONENT24;
                    case TextureFormat::UNormDepth24Stencil8:
                        return GL_DEPTH24_STENCIL8_EXT;
                    #if defined(GL_NV_depth_buffer_float)                
                    case TextureFormat::FloatDepth32:
                        return GL_DEPTH_COMPONENT32F_NV;
                    #elif defined(GL_VEJOHION_3_0)
                    case TextureFormat::FloatDepth32:
                        return GL_DEPTH_COMPONENT32F;
                    #endif

                    // Compressed formats
                    #if defined(GL_EXT_texture_compression_s3tc)
                    case TextureFormat::DXT1:
                        return GL_COMPRESSED_RGB_S3TC_DXT1_EXT;
                    case TextureFormat::DXT3:
                        return GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
                    case TextureFormat::DXT5:
                        return GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
                    #endif
                        
					// sRGB formats
					case TextureFormat::UNormSRGB8:
						return GL_SRGB8;
					case TextureFormat::UNormSRGB8A8:
						return GL_SRGB8_ALPHA8;

                    default: return 0;
                }
            }

            GLenum ConvertTextureFormatToGLFormat(TextureFormat const& format) {
                if (format.IsDepth()) {
                    return GL_DEPTH_COMPONENT;
                }

                switch(format.GetChannelCount()) {
                    case 1: return GL_LUMINANCE;
                    case 2: return GL_LUMINANCE_ALPHA;                    
                    case 3: return GL_RGB;
                    case 4: return GL_RGBA;
                    default: return 0;
                }
            }

            GLenum ConvertTextureFormatToGLType(TextureFormat const& format) {
                switch(format.GetFormat()) {
                    case TextureFormat::UNormRGB8: 
                    case TextureFormat::UNormRGBA8:
                        return GL_UNSIGNED_BYTE;

                    case TextureFormat::UNormRGB16:
                    case TextureFormat::UNormRGBA16:
                        return GL_UNSIGNED_SHORT;

                    case TextureFormat::SNormRGB8: 
                    case TextureFormat::SNormRGBA8:
                        return GL_BYTE;

                    case TextureFormat::SNormRGB16:
                    case TextureFormat::SNormRGBA16:
                        return GL_SHORT;

                    case TextureFormat::UIntRGB16: 
                    case TextureFormat::UIntRGBA16:
                        return GL_UNSIGNED_SHORT;

                    case TextureFormat::UIntRGB32:
                    case TextureFormat::UIntRGBA32:
                        return GL_UNSIGNED_INT;

                    case TextureFormat::SIntRGB16: 
                    case TextureFormat::SIntRGBA16:
                        return GL_SHORT;

                    case TextureFormat::SIntRGB32:
                    case TextureFormat::SIntRGBA32:
                        return GL_INT;

                    case TextureFormat::FloatRGB16:
                    case TextureFormat::FloatRGBA16:
                        return GL_HALF_FLOAT_ARB;

                    case TextureFormat::FloatRGB32:
                    case TextureFormat::FloatRGBA32:
                        return GL_FLOAT;
                    
                    #if defined(GL_EXT_packed_float)
                    case TextureFormat::FloatR11G11B10:
                        return GL_UNSIGNED_INT_10F_11F_11F_REV_EXT;
                    #elif defined(GL_VEJOHION_3_0)
                    case TextureFormat::FloatR11G11B10:
                        return GL_UNSIGNED_INT_10F_11F_11F_REV;
                    #endif
                    case TextureFormat::UNormR5G6R5:
                        return GL_UNSIGNED_SHORT_5_6_5;
                    case TextureFormat::UNormR5G5R5A1:
                        return GL_UNSIGNED_SHORT_1_5_5_5_REV;
                    case TextureFormat::UNormRGBA4:
                        return GL_UNSIGNED_SHORT_4_4_4_4_REV;
                    case TextureFormat::UNormR10G10B10A2:
                        return GL_UNSIGNED_INT_2_10_10_10_REV;

                    case TextureFormat::UNormDepth16:
                        return GL_UNSIGNED_INT;
                    case TextureFormat::UNormDepth24:
                        return GL_UNSIGNED_INT;
                    case TextureFormat::UNormDepth24Stencil8:
                        return GL_UNSIGNED_INT_24_8_EXT;
                    case TextureFormat::FloatDepth32:
                        return GL_FLOAT;

					case TextureFormat::UNormSRGB8:
					case TextureFormat::UNormSRGB8A8:
						return GL_UNSIGNED_BYTE;

                    default:
                        return 0;
                }
            }
        }
                
        GLTexture2D::GLTexture2D(Texture2DDescription const& description)
        : Texture2D(description),
          allocated_(false),
          glinternalFormat_(ConvertTextureFormatToGLInternalFormat(GetFormat())),
          glformat_(ConvertTextureFormatToGLFormat(GetFormat())),
          gltype_(ConvertTextureFormatToGLType(GetFormat())) {
            JOH_ASSERT(gltype_ != 0, "Invalid OpenGL type");
            JOH_ASSERT(glformat_ != 0, "Invalid OpenGL type");
            JOH_ASSERT(glinternalFormat_ != 0, "Invalid OpenGL type");
            JOH_ASSERT(GetUsage() != ResourceUsage::Static, "Static resource's must be provided with data upon creaiton.");
            JOH_ASSERT(GetType() == ResourceType::Texture2D || GetType() == ResourceType::ShaderResource, "Invalid resource type for Texture2D");
            glGenTextures(1, &handle_);
            glBindTexture(GL_TEXTURE_2D, handle_);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            for (uint8_t mipmap = 0; mipmap <= GetMipMapCount(); ++mipmap) {
                SetData(0, mipmap);
            }
            
            allocated_ = true;
        }
        
        GLTexture2D::GLTexture2D(Texture2DDescription const& description, void const* data)
        : Texture2D(description),
          allocated_(false),
          glinternalFormat_(ConvertTextureFormatToGLInternalFormat(GetFormat())),
          glformat_(ConvertTextureFormatToGLFormat(GetFormat())),
          gltype_(ConvertTextureFormatToGLType(GetFormat())) {
            JOH_ASSERT(gltype_ != 0, "Invalid OpenGL type");
            JOH_ASSERT(glformat_ != 0, "Invalid OpenGL type");
            JOH_ASSERT(glinternalFormat_ != 0, "Invalid OpenGL type");JOH_ASSERT(GetType() == ResourceType::Texture2D || GetType() == ResourceType::ShaderResource, "Invalid resource type for Texture2D.");
            glGenTextures(1, &handle_);
            glBindTexture(GL_TEXTURE_2D, handle_);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            uint8_t const* bytes = static_cast< uint8_t const* >(data);
            for (uint8_t mipmap = 0; mipmap <= GetMipMapCount(); ++mipmap) {
                uint32_t imageSize = GetFormat().GetSize((uint32_t)GetWidth() >> mipmap, (uint32_t)GetHeight() >> mipmap);
                SetData(bytes, mipmap);
                bytes += imageSize;
            }
            
            allocated_ = true;
            if (!data) {
                JOH_ASSERT(GetUsage() != ResourceUsage::Static, "Static texture-resource's must be provided with data upon creation.");
            }
        }
        
        GLTexture2D::~GLTexture2D() {
            glDeleteTextures(1, &handle_);
			allocated_ = false;
			handle_ = 0;
        }
        
        void GLTexture2D::GenerateMipmaps() {
            JOH_ASSERT(GetUsage() != ResourceUsage::Static, "Cannot generate mipmaps for static texture-resource.");
            glBindTexture(GL_TEXTURE_2D, handle_);
            glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);
            glGenerateMipmapEXT(GL_TEXTURE_2D);
        }
        
        void GLTexture2D::SetData(void const* data, uint8_t mipLevel) {
            if (allocated_) JOH_ASSERT(GetUsage() != ResourceUsage::Static, "Cannot change data for static texture-resource.");

            uint32_t imageSize = GetFormat().GetSize((uint32_t)GetWidth() >> mipLevel, (uint32_t)GetHeight() >> mipLevel);
            glBindTexture(GL_TEXTURE_2D, handle_);
			CHECK_GL_ERROR();
			if (GetFormat().IsCompressed()) {
                glCompressedTexImage2D(GL_TEXTURE_2D, mipLevel, (GLenum)glinternalFormat_, (GLsizei)GetWidth() >> mipLevel, (GLsizei)GetHeight() >> mipLevel, 0, (GLint)imageSize, data);
            }
            else {
                glTexImage2D(GL_TEXTURE_2D, mipLevel, (GLenum)glinternalFormat_, (GLsizei)GetWidth() >> mipLevel, (GLsizei)GetHeight() >> mipLevel, 0, glformat_, gltype_, data);
				CHECK_GL_ERROR();
			}
        }
        
        void GLTexture2D::SetData(void const* data, size_t x, size_t y, size_t width, size_t height, uint8_t mipLevel) {
            if (allocated_) JOH_ASSERT(GetUsage() != ResourceUsage::Static, "Cannot change data for static texture-resource.");

            uint32_t imageSize = GetFormat().GetSize((uint32_t)width, (uint32_t)height);
            glBindTexture(GL_TEXTURE_2D, handle_);
            if (GetFormat().IsCompressed()) {
                glCompressedTexSubImage2D(GL_TEXTURE_2D, mipLevel, (GLint)x, (GLint)y, (GLsizei)width, (GLsizei)height, 0, (GLsizei)imageSize, data);
            }
            else {
                glTexSubImage2D(GL_TEXTURE_2D, mipLevel, (GLint)x, (GLint)y, (GLsizei)width, (GLsizei)height, (GLenum)glinternalFormat_, imageSize, data);
            }
        }
        
        void const* GLTexture2D::GetData(uint8_t mipLevel) const {
            JOH_ASSERT(mipLevel <= GetMipMapCount(), "mipLevel is out of range");
            uint32_t imageSize = GetFormat().GetSize((uint32_t)GetWidth() >> mipLevel, (uint32_t)GetHeight() >> mipLevel);
            uint8_t* result = new uint8_t[imageSize];
            glBindTexture(GL_TEXTURE_2D, handle_);
            glGetTexImage(GL_TEXTURE_2D, mipLevel, glformat_, gltype_, result);
            return result;
        }
    }
}
