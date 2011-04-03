#ifndef JOH_GRAPHICS_OPENGL_GLSHADER_HPP
#define JOH_GRAPHICS_OPENGL_GLSHADER_HPP

#include "joh/graphics/opengl/OpenGLForwards.hpp"
#include "joh/graphics/Shader.hpp"
#include <map>
#include <string>

namespace joh
{
    namespace graphics
    {
        namespace internal
        {
            struct Constant
            {
                char* name;
                uint8_t* data;
                GLuint index;
                GLint size;
                GLenum type;
                bool dirty;
            };
            
            struct Sampler
            {
                char* name;
                GLint location;
                GLint textureUnit;
                GLenum type;
            };
        }
        
        class GLShader : public Shader
        {
			friend class GLRenderer;
            
        private:
            
            typedef std::map< std::string, int32_t > AttributeSemanticIndexMap;
            
        public:
            
            GLShader(ShaderDescription const& description);
            
            virtual ~GLShader();
            
            GLuint GetProgramHandle() const;
            
            GLuint GetVertexShaderHandle() const;
            
            GLuint GetFragmentShaderHandle() const;
            
            GLuint GetGeometryShaderHandle() const;
            
            int32_t GetAttributeIndex(char const* semanticName) const;
            
            virtual void Apply();
                        
        protected:
            
            virtual void SetRawConstant(char const* name, uint8_t const* data, size_t dataSize);

			int GetSamplersUnit(char const* name);
            
        protected:
            
            AttributeSemanticIndexMap attributesSemanticIndices_;
            internal::Constant* uniforms_;
            internal::Sampler* samplers_;
			GLuint programHandle_;
            GLuint vertexShaderHandle_;
            GLuint fragmentShaderHandle_;
            GLuint geometryShaderHandle_;
            int32_t uniformsCount_;
            int32_t samplerCount_;
        };
        
                
        inline GLuint GLShader::GetProgramHandle() const {
            return programHandle_;
        }
        
        inline GLuint GLShader::GetVertexShaderHandle() const {
            return vertexShaderHandle_;
        }
        
        inline GLuint GLShader::GetFragmentShaderHandle() const {
            return fragmentShaderHandle_;
        }
        
        inline GLuint GLShader::GetGeometryShaderHandle() const {
            return geometryShaderHandle_;
        }
    }
}

#endif // JOH_GRAPHICS_OPENGL_GLSHADER_HPP
