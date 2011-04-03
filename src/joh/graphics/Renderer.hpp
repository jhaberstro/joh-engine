#ifndef JOH_GRAPHICS_RENDERER_HPP
#define JOH_GRAPHICS_RENDERER_HPP

#include "joh/graphics/GraphicsForwards.hpp"
#include "joh/graphics/Buffer.hpp"
#include "joh/graphics/Shader.hpp"
#include "joh/graphics/Texture2D.hpp"
#include "joh/graphics/VertexFormat.hpp"
#include "joh/graphics/opengl/OpenGLForwards.hpp"
#include <vector>

namespace joh
{
    namespace graphics
    {
        namespace internal
        {
            struct Stream
            {
                std::vector< InputElement > attributes;
                Buffer* vertexBuffer;
                uintptr_t offset;
            };
        }
        
        typedef int32_t VertexFormatID;  
        class Renderer
        {            
        protected:
            
            typedef std::vector< VertexFormat > VertexFormatContainer;
            
        public:
            
            Renderer();
            
            virtual ~Renderer() { }
             
            virtual void Apply() = 0; 
            
            virtual void ApplyInputAssemberState() = 0;
            
            virtual void ApplyShaderState() = 0;

			virtual void ApplyTextures() = 0;
            
            virtual VertexFormatID CreateVertexFormat(InputElement const* inputElements, size_t count) = 0;
            
            virtual Buffer* CreateBuffer(BufferDescription const& desciption, void const* data) = 0;
                        
            virtual Shader* CreateShader(ShaderDescription const& desciption) = 0;

			virtual Texture2D* CreateTexture2D(Texture2DDescription const& description) = 0;

			virtual Texture2D* CreateTexture2D(Texture2DDescription const& description, void const* data) = 0;
            
            virtual void Draw(uint32_t startVertexLocation, uint32_t verticeCount) = 0;
            
            virtual void DrawIndexed(uint32_t startIndexLocation, uint32_t indiceCount) = 0;
            
            virtual PrimitiveTopology::Enum GetPrimitiveTopology() const = 0;
            
            virtual void SetPrimitiveTopology(PrimitiveTopology::Enum topology) = 0;
            
            virtual void SetIndexBuffer(Buffer* indexBuffer) = 0;
            
            virtual void SetShader(Shader* shader) = 0;

			virtual void SetTexture(char const* name, Texture2D* texture) = 0;
            
            virtual void SetVertexBuffer(Buffer* vertexBuffer, uint32_t const stream = 0, uintptr_t const offset = 0) = 0;
            
            virtual void SetVertexFormat(VertexFormatID vertexFormat, Shader* shader) = 0;
                        
        protected:
            enum { kMaxStreams = 8 };
			enum { kTextureUnits = 16 };
            
            // Resources created by the renderer, and thus owned by the renderer
            VertexFormatContainer vertexFormats_;
            
            // State data
            internal::Stream streams_[kMaxStreams];
            Buffer* currentIndexBuffer_;
            Buffer* currentVertexBuffer_;
            VertexFormatID currentVertexFormat_;
            Shader* currentShader_;
            Shader* newShader_;
			Texture2D* currentTextures_[kTextureUnits];
			Texture2D* newTextures_[kTextureUnits];
        };
        
        
        inline Renderer::Renderer()
        : currentVertexFormat_(-1) {
        }       
    }
}

#endif // JOH_GRAPHICS_RENDERER_HPP
