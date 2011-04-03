#ifndef JOH_GRAPHICS_SHADEROBJECT_HPP
#define JOH_GRAPHICS_SHADEROBJECT_HPP

#include "joh/graphics/GraphicsForwards.hpp"
#include "joh/graphics/Resource.hpp"
#include "joh/graphics/ShaderDescription.hpp"
#include "joh/math/Vector2.hpp"
#include "joh/math/Vector3.hpp"
#include "joh/math/Vector4.hpp"
#include "joh/math/Matrix4x4.hpp"

namespace joh
{
    namespace graphics
    {
        class Shader : public Resource
        {            
        public:
            
            virtual ~Shader() { };
            
            virtual void Apply() = 0;
                        
            void SetConstant(char const* name, float const value);
            
            void SetConstant(char const* name, int const value);
            
            void SetConstant(char const* name, bool const value);
            
            void SetConstant(char const* name, joh::math::Vector2f const& value);

            void SetConstant(char const* name, joh::math::Vector3f const& value);
            
            void SetConstant(char const* name, joh::math::Vector4f const& value);
            
            void SetConstant(char const* name, joh::math::Matrix4x4f const& value);
            
            void SetConstant(char const* name, float const* array, uint32_t const count);

            void SetConstant(char const* name, int const* array, uint32_t const count);
            
            void SetConstant(char const* name, bool const* array, uint32_t const count);
            
            void SetConstant(char const* name, joh::math::Vector2f const* array, uint32_t const count);

            void SetConstant(char const* name, joh::math::Vector3f const* array, uint32_t const count);

            void SetConstant(char const* name, joh::math::Vector4f const* array, uint32_t const count);
            
            void SetConstant(char const* name, joh::math::Matrix4x4f const* array, uint32_t const count);
                                    
            bool HasGeometryShader() const;
            
            bool IsVertexShaderCompiled() const;
            
            bool IsFragmentShaderCompiled() const;
            
            bool IsGeometryShaderCompile() const;
            
        protected:
            
            Shader(ShaderDescription const& description);
                    
            virtual void SetRawConstant(char const* name, uint8_t const* data, size_t dataSize) = 0;
                    
        protected:
            
            bool hasGeometryShader_;
            bool vertexShaderCompiled_;
            bool fragmentShaderCompiled_;
            bool geometryShaderCompiled_;
        };
                
        inline Shader::Shader(ShaderDescription const& description)
        : Resource(description),
          hasGeometryShader_(description.GetGeometryShaderFile() != 0),
          vertexShaderCompiled_(true),
          fragmentShaderCompiled_(true),
          geometryShaderCompiled_(true) {
        }
        
        inline void Shader::SetConstant(char const* name, float const value) {
            SetRawConstant(name, reinterpret_cast< uint8_t const* >(&value), sizeof(value));
        }
        
        inline void Shader::SetConstant(char const* name, int const value) {
            SetRawConstant(name, reinterpret_cast< uint8_t const* >(&value), sizeof(value));
        }
        
        inline void Shader::SetConstant(char const* name, bool const value) {
            SetRawConstant(name, reinterpret_cast< uint8_t const* >(&value), sizeof(value));
        }
        
        inline void Shader::SetConstant(char const* name, joh::math::Vector2f const& value) {
            SetRawConstant(name, reinterpret_cast< uint8_t const* >(&value), sizeof(value));
        }
        
        inline void Shader::SetConstant(char const* name, joh::math::Vector3f const& value) {
            SetRawConstant(name, reinterpret_cast< uint8_t const* >(&value), sizeof(value));
        }
        
        inline void Shader::SetConstant(char const* name, joh::math::Vector4f const& value) {
            SetRawConstant(name, reinterpret_cast< uint8_t const* >(&value), sizeof(value));
        }
        
        inline void Shader::SetConstant(char const* name, joh::math::Matrix4x4f const& value) {
            SetRawConstant(name, reinterpret_cast< uint8_t const* >(&value), sizeof(value));
        }
        
        inline void Shader::SetConstant(char const* name, float const* array, uint32_t const count) {
            SetRawConstant(name, reinterpret_cast< uint8_t const* >(array), sizeof(array[0]) * count);
        }

        inline void Shader::SetConstant(char const* name, int const* array, uint32_t const count) {
            SetRawConstant(name, reinterpret_cast< uint8_t const* >(array), sizeof(array[0]) * count);
        }
        
        inline void Shader::SetConstant(char const* name, bool const* array, uint32_t const count) {
            SetRawConstant(name, reinterpret_cast< uint8_t const* >(array), sizeof(array[0]) * count);
        }
        
        inline void Shader::SetConstant(char const* name, joh::math::Vector2f const* array, uint32_t const count) {
            SetRawConstant(name, reinterpret_cast< uint8_t const* >(array), sizeof(array[0]) * count);
        }
        
        inline void Shader::SetConstant(char const* name, joh::math::Vector3f const* array, uint32_t const count) {
            SetRawConstant(name, reinterpret_cast< uint8_t const* >(array), sizeof(array[0]) * count);
        }

        inline void Shader::SetConstant(char const* name, joh::math::Vector4f const* array, uint32_t const count) {
            SetRawConstant(name, reinterpret_cast< uint8_t const* >(array), sizeof(array[0]) * count);
        }
        
        inline void Shader::SetConstant(char const* name, joh::math::Matrix4x4f const* array, uint32_t const count) {
            SetRawConstant(name, reinterpret_cast< uint8_t const* >(array), sizeof(array[0]) * count);
        }

        inline bool Shader::HasGeometryShader() const {
            return hasGeometryShader_;
        }
        
        inline bool Shader::IsVertexShaderCompiled() const {
            return vertexShaderCompiled_;
        }
        
        inline bool Shader::IsFragmentShaderCompiled() const {
            return fragmentShaderCompiled_;
        }
        
        inline bool Shader::IsGeometryShaderCompile() const {
            return (HasGeometryShader() == false) || geometryShaderCompiled_;
        }
    }
}

#endif // JOH_GRAPHICS_SHADEROBJECT_HPP
