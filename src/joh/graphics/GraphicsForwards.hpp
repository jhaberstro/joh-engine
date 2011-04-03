#ifndef JOH_GRAPHICS_GRAPHICSFORWARDS_HPP
#define JOH_GRAPHICS_GRAPHICSFORWARDS_HPP

#include "joh/joh.hpp"

namespace joh
{
    namespace graphics
    {        
        class Buffer;
        class BufferDescription;
        class GraphicsContext;
        class Image;
        class Renderer;
        class Resource;
        class ResourceDescription;
        class Shader;
        class ShaderDescription;
        class Texture2D;
        class Texture2DDescription;
		class TextureFormat;
        class VertexFormat;
        
        namespace InputElementFormat
        {
            enum Enum
            {
                SInt8,
                UInt8,
                SInt16,
                UInt16,
                SInt32,
                UInt32,
                Float16,
                Float32,
                Double
            };
        }
        
        namespace PrimitiveTopology
        {
            enum Enum
            {
                Points,
                LineStrip,
                LineLoop,
                Lines,
                TriangleStrip,
                TriangleFan,
                Triangles,
                Quads,
            };
        }
        
        namespace ResourceAccess
        {
            enum Enum
            {
                None      = 0x00000000,
                Read      = 0x00000001,
                Write     = 0x00000002,
                ReadWrite = 0x00000004
            };
        }
        
        namespace ResourceType
        {
            enum Enum
            {
                Unknown             = 0x00000000,
                DepthStencilBuffer  = 0x00000001,
                IndexBuffer         = 0x00000002,
                RenderTarget        = 0x00000004,
                ShaderResource      = 0x00000008,
                Texture2D           = 0x00000010,
                ConstantBuffer      = 0x00000020,
                VertexBuffer        = 0x00000040
            };
        }
        
        namespace ResourceUsage
        {
            enum Enum
            {
                Default = 0x00000000,
                Static  = 0x00000001,
                Dynamic = 0x00000002,
                Stream  = 0x00000004
            };
        }
        
        
        struct InputElement
        {
            char const* name;
            uint8_t semanticIndex;
            InputElementFormat::Enum format;
            uint8_t componentCount;
            uint32_t offset;
            uint32_t stream; // inputSlot
        };
        
        inline uint32_t GetInputElementFormatSize(InputElementFormat::Enum format) {
            static uint32_t const sizes[] =
            {
                sizeof(int8_t), sizeof(uint8_t),
                sizeof(int16_t), sizeof(uint16_t),
                sizeof(int32_t), sizeof(uint32_t),
                sizeof(int16_t), sizeof(float),
                sizeof(double)
            };
            
            uint32_t result = sizes[format];
            return result;
        }
    }
}

#endif // JOH_GRAPHICS_GRAPHICSFORWARDS_HPP
