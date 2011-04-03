#include "joh/graphics/opengl/GLShader.hpp"
#include "joh/graphics/opengl/GLTexture2D.hpp"
#include <cstdio>
#include <cstring> 

namespace joh
{
    namespace graphics
    {        
        namespace
        {
            struct GLUniform
            {
                GLsizei nameLength;
                GLint size;
                GLenum type;
                GLchar* name;
            };
            
            void FreeConstant(internal::Constant& constant) {
                delete [] constant.name;
                delete [] constant.data;
            }
            
            void FreeSampler(internal::Sampler& sampler) {
                delete [] sampler.name;
            }
            
            char* GetShadersSource(char const* filepath, GLint& outSize) {
                FILE* file = std::fopen(filepath, "r");
                JOH_ASSERTF(file != 0, "Error opening file: %s", filepath);
                
                std::fseek(file, 0, SEEK_END);
                unsigned long size = (unsigned long)ftell(file);
                std::rewind(file);
                
                char* contents = new char[size];
                size_t result = std::fread(contents, 1, size, file);
                JOH_ASSERTF(result == size, "Unable to read shader at: %s", filepath);
                
                std::fclose(file);
                outSize = (GLint)size;
                return contents;
            }
            
            bool CheckCompiledStatus(GLuint handle) {
                GLint status = GL_TRUE;
                glGetShaderiv(handle, GL_COMPILE_STATUS, &status);
                return status == GL_TRUE;
            }
            
            template< typename T >
            int CompareUniforms(T const& lhs, T const& rhs) {
                return std::strcmp(lhs.name, rhs.name) < 0;
            }
            
            template< typename T >
            T FindUniform(T first, T last, char const* name) {
                //first = lower_bound(first, last, name);
                T it;
                size_t count = (last - first), step;
                while (count > 0) {
                    it = first;
                    step = count >> 2;
                    it += step;
                    if (std::strcmp(it->name, name) < 0) {
                        first = ++it;
                        count -= (step + 1);
                    }
                    else {
                        count = step;
                    }
                }
                
                if (first != last && (std::strcmp(name, first->name) == 0)) {
                    return first;
                }
                
                return 0;
            }
            
            size_t GLUniformTypeSize(GLenum type) {
                switch (type) {
                    case GL_INT: return sizeof(GLint);
                    case GL_BOOL: return sizeof(bool);
                    case GL_FLOAT: return sizeof(GLfloat);
                    case GL_FLOAT_VEC3: return sizeof(joh::math::Vector3f);
                    case GL_FLOAT_VEC4: return sizeof(joh::math::Vector4f);
                    case GL_FLOAT_MAT4: return sizeof(joh::math::Matrix4x4f);
                    default: {
                        JOH_ASSERT(false, "Unsupported uniform type!");
                        return 0;
                    }
                }
                
                return 0;
            }
        }
        
        
        GLShader::GLShader(ShaderDescription const& description)
        : Shader(description),
          uniforms_(0),
          samplers_(0),
 		  uniformsCount_(0),
		  samplerCount_(0) {
            JOH_ASSERT(description.GetVertexShaderFile() != 0, "vertex shader file path is null");
            JOH_ASSERT(description.GetFragmentShaderFile() != 0, "fragment shader file path is null");
            
            programHandle_ = glCreateProgram();
            JOH_ASSERT(programHandle_ != 0, "Error creating OpenGL shader program object");
            
            GLint vsSourceSize = 0;
            char const* vsSource = GetShadersSource(description.GetVertexShaderFile(), vsSourceSize);
            vertexShaderHandle_ = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vertexShaderHandle_, 1, &vsSource, &vsSourceSize);
            glCompileShader(vertexShaderHandle_);
            vertexShaderCompiled_ = CheckCompiledStatus(vertexShaderHandle_);
            if (vertexShaderCompiled_) {
                glAttachShader(programHandle_, vertexShaderHandle_);
            }
            else {
                GLint infoLogLength = 0;
                glGetShaderiv(vertexShaderHandle_, GL_INFO_LOG_LENGTH, &infoLogLength);
                char infoLog[infoLogLength];
                glGetShaderInfoLog(vertexShaderHandle_, infoLogLength, 0, &infoLog[0]);
                JOH_ASSERTF(false, "Compiling vertex shader: '%s' failed.\nInfo log: %s", description.GetVertexShaderFile(), infoLog);
            }
            
            GLint fsSourceSize = 0;
            char const* fsSource = GetShadersSource(description.GetFragmentShaderFile(), fsSourceSize);
            fragmentShaderHandle_ = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fragmentShaderHandle_, 1, &fsSource, &fsSourceSize);
            glCompileShader(fragmentShaderHandle_);
            fragmentShaderCompiled_ = CheckCompiledStatus(fragmentShaderHandle_);
            if (fragmentShaderCompiled_) {
                glAttachShader(programHandle_, fragmentShaderHandle_);
            }
            else {
                GLint infoLogLength = 0;
                glGetShaderiv(fragmentShaderHandle_, GL_INFO_LOG_LENGTH, &infoLogLength);
                char infoLog[infoLogLength];
                glGetShaderInfoLog(fragmentShaderHandle_, infoLogLength, 0, &infoLog[0]);
                JOH_ASSERTF(false, "Compiling fragment shader: '%s' failed.\nInfo log: %s", description.GetFragmentShaderFile(), infoLog);
            }
            
            if (HasGeometryShader()) {
                JOH_ASSERT(false, "Geometry shaders are not yet supported");
            }
            
            glLinkProgram(programHandle_);
            GLint linkStatus = GL_TRUE;
            glGetProgramiv(programHandle_, GL_LINK_STATUS, &linkStatus);
            
            
            if (linkStatus == GL_TRUE) {
                GLint currentProgram = 0;
                glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
                glUseProgram(programHandle_);
                
                // Bind attribute locations
                char const** attributeNames = description.GetAttributeNames();
                if (attributeNames != 0) {
                    for (uint32_t i = 0; i < description.GetAttributeNamesCount(); ++i) {
                        if (attributeNames[i]) {
                            glBindAttribLocation(programHandle_, i, attributeNames[i]);
                            attributesSemanticIndices_[attributeNames[i]] = (int32_t)i;
                        }
                    }
                }

                // Generate data about uniforms
                enum { kBuiltinUniform = GL_INVALID_ENUM };
                GLint maxLength, uniformsCount;
                glGetProgramiv(programHandle_, GL_ACTIVE_UNIFORMS, &uniformsCount);
                glGetProgramiv(programHandle_, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxLength);
                if (uniformsCount > 0) {
                    char name[maxLength];
                    GLUniform gluniforms[uniformsCount];
                    for (int i = 0; i < uniformsCount; ++i) {
                        GLsizei nameLength = 0;
                        GLint size = 0;
                        GLenum type;
                        glGetActiveUniform(programHandle_, (GLuint)i, maxLength, &nameLength, &size, &type, name);
                        if (std::strncmp(name, "gl_", 3) != 0) {
                            gluniforms[i].nameLength = nameLength;
                            gluniforms[i].size = size;
                            gluniforms[i].type = type;
                            gluniforms[i].name = new char[(unsigned long)nameLength];
                            std::strncpy(gluniforms[i].name, name, (size_t)nameLength);
                            std::memset(name, 0, sizeof(char) * (unsigned long)maxLength);
                            if (gluniforms[i].type == GL_SAMPLER_2D) {
                                ++samplerCount_;
                            }
                            else {
                                ++uniformsCount_;
                            }
                        }
                        else {
                            gluniforms[i].type = kBuiltinUniform;
                        }
                        
                    }
                    
                    uniforms_ = new internal::Constant[(unsigned long)uniformsCount_];
                    samplers_ = new internal::Sampler[(unsigned long)samplerCount_];
                    int currentUniform = 0, currentSampler = 0;
                    for (int i = 0; i < uniformsCount; ++i) {
                        char* bracket = std::strchr(gluniforms[i].name, '[');
                        gluniforms[i].nameLength = (GLsizei)(bracket == 0 ? gluniforms[i].nameLength : reinterpret_cast< intptr_t >(bracket - gluniforms[i].nameLength));
                        if (gluniforms[i].type == GL_SAMPLER_2D) {
                            printf("%s\n", gluniforms[i].name);
                            GLint location = glGetUniformLocation(programHandle_, gluniforms[i].name);
							CHECK_GL_ERROR();
							glUniform1i(location, currentSampler);
							CHECK_GL_ERROR();
							samplers_[currentSampler].name = gluniforms[i].name;
                            samplers_[currentSampler].location = location;
                            samplers_[currentSampler].textureUnit = currentSampler;
                            samplers_[currentSampler].type = gluniforms[i].type;
                            ++currentSampler;
                        }
                        else if (gluniforms[i].type != kBuiltinUniform) {
                            uniforms_[currentUniform].name = gluniforms[i].name;
                            uniforms_[currentUniform].data = new uint8_t[(unsigned long)gluniforms[i].size * GLUniformTypeSize(gluniforms[i].type)];                        
                            uniforms_[currentUniform].index = (GLuint)glGetUniformLocation(programHandle_, gluniforms[i].name);
                            uniforms_[currentUniform].size = gluniforms[i].size;
                            uniforms_[currentUniform].type = gluniforms[i].type;
                            uniforms_[currentUniform].dirty = false;
                            ++currentUniform;
                        }
                    }
                }
                
                glUseProgram((GLuint)currentProgram);
            }
            
            std::sort(uniforms_, uniforms_ + uniformsCount_, CompareUniforms< internal::Constant >);
            std::sort(samplers_, samplers_ + samplerCount_, CompareUniforms< internal::Sampler >);
        }
        
        GLShader::~GLShader() {
            for (int32_t i = 0; i < uniformsCount_; ++i) {
                FreeConstant(uniforms_[i]);
            }
            
            for (int32_t i = 0; i < samplerCount_; ++i) {
                FreeSampler(samplers_[i]);
            }
            
            delete [] uniforms_;
            delete [] samplers_;
        }
        
        int32_t GLShader::GetAttributeIndex(char const* semanticName) const {
            AttributeSemanticIndexMap::const_iterator itr = attributesSemanticIndices_.find(semanticName);
            if (itr != attributesSemanticIndices_.end()) {
                return itr->second;
            }
            
            return -1;
        }
        
        void GLShader::Apply() {
            for (int32_t i = 0; i < uniformsCount_; ++i) {
                if (uniforms_[i].dirty) {
                    internal::Constant& uniform = uniforms_[i];
                    switch(uniform.type) {
                        case GL_INT:
                            glUniform1iv((GLint)uniform.index, uniform.size, reinterpret_cast< GLint* >(uniform.data));
                            break;
                        case GL_BOOL:
                            glUniform1iv((GLint)uniform.index, uniform.size, reinterpret_cast< GLint* >(uniform.data));
                            break;
                        case GL_FLOAT:
                            glUniform1fv((GLint)uniform.index, uniform.size, reinterpret_cast< GLfloat* >(uniform.data));
                            break;
                        case GL_FLOAT_VEC3:
                            glUniform3fv((GLint)uniform.index, uniform.size, reinterpret_cast< GLfloat* >(uniform.data));
                            break;
                        case GL_FLOAT_VEC4:
                            glUniform4fv((GLint)uniform.index, uniform.size, reinterpret_cast< GLfloat* >(uniform.data));
                            break;
                        case GL_FLOAT_MAT4:
                            glUniformMatrix4fv((GLint)uniform.index, uniform.size, GL_FALSE, reinterpret_cast< GLfloat* >(uniform.data));
                            break;
                        default: {
                            JOH_ASSERT(false, "Unsupported uniform type!");
                            break;
                        }
                    }
                    
                    uniform.dirty = false;
                }
            }
        }
        
        void GLShader::SetRawConstant(char const* name, uint8_t const* data, size_t dataSize) {
            JOH_ASSERT(name != 0, "shader constant name is null");
            JOH_ASSERT(data != 0, "new shader constant data is null");
            internal::Constant* constant = FindUniform(uniforms_, uniforms_ + uniformsCount_, name);
            if (constant != 0) {
                std::memcpy(constant->data, data, dataSize);
                constant->dirty = true;
            }
        }

		int GLShader::GetSamplersUnit(char const* name) {
			internal::Sampler* sampler = FindUniform(samplers_, samplers_ + samplerCount_, name);
			return sampler == (internal::Sampler*)(0 ? -1 : sampler - samplers_);
		}
    }
}
