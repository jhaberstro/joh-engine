#ifndef JOH_GRAPHICS_OPENGL_GLRENDERER_HPP
#define JOH_GRAPHICS_OPENGL_GLRENDERER_HPP

#include "joh/graphics/GraphicsForwards.hpp"
#include "joh/graphics/Renderer.hpp"
#include <vector>

namespace joh
{   
    namespace graphics
    {
        class GLRenderer : public Renderer
        {            
        public:
            			
            void Apply();
            
            void ApplyInputAssemberState();
            
            void ApplyShaderState();

			void ApplyTextures();
            
            VertexFormatID CreateVertexFormat(InputElement const* inputElements, size_t count);
            
            Buffer* CreateBuffer(BufferDescription const& desciption, void const* data);
            
            Shader* CreateShader(ShaderDescription const& desciption);

			Texture2D* CreateTexture2D(Texture2DDescription const& description);

			Texture2D* CreateTexture2D(Texture2DDescription const& description, void const* data);
            
            void Draw(uint32_t startVertexLocation, uint32_t verticeCount);
            
            void DrawIndexed(uint32_t startIndexLocation, uint32_t indiceCount);

            PrimitiveTopology::Enum GetPrimitiveTopology() const;
            
            void SetPrimitiveTopology(PrimitiveTopology::Enum topology);
            
            void SetIndexBuffer(Buffer* indexBuffer);
            
            void SetShader(Shader* shader);
            
            void SetTexture(char const* name, Texture2D* texture);
			
			void SetVertexBuffer(Buffer* vertexBuffer, uint32_t const stream, uintptr_t const offset);
            
            void SetVertexFormat(VertexFormatID vertexFormat, Shader* shader);
            
        protected:
            
            PrimitiveTopology::Enum currentPrimitiveTopology_;
        };
    }
}

#endif // JOH_GRAPHICS_OPENGL_GLRENDERER_HPP
