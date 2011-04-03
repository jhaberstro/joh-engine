#include "joh/graphics/opengl/GLRenderer.hpp"
#include "joh/graphics/opengl/GLBuffer.hpp"
#include "joh/graphics/opengl/GLShader.hpp"
#include "joh/graphics/opengl/GLTexture2D.hpp"
#include "joh/graphics/VertexFormat.hpp"

namespace joh
{
    namespace graphics
    {        
        namespace
        {
            GLenum InputElementFormatToGLElementFormat(InputElementFormat::Enum format) {
                switch (format) {
                    case InputElementFormat::SInt8:   return GL_BYTE;
                    case InputElementFormat::UInt8:   return GL_UNSIGNED_BYTE;
                    case InputElementFormat::SInt16:  return GL_SHORT;
                    case InputElementFormat::UInt16:  return GL_UNSIGNED_SHORT;
                    case InputElementFormat::SInt32:  return GL_INT;
                    case InputElementFormat::UInt32:  return GL_UNSIGNED_INT;
                    case InputElementFormat::Float16: return GL_HALF_FLOAT;
                    case InputElementFormat::Float32: return GL_FLOAT;
                    case InputElementFormat::Double:  return GL_DOUBLE;
                    default: {
                        JOH_ASSERT(false, "InputElement format is invalid.");
                        return 0;
                    }
                }
            }
            
            GLenum PrimitiveTopologyToGLPrimitiveMode(PrimitiveTopology::Enum topology) {
                switch(topology) {
                    case PrimitiveTopology::Points: return GL_POINTS;
                    case PrimitiveTopology::LineStrip: return GL_LINE_STRIP;
                    case PrimitiveTopology::LineLoop: return GL_LINE_LOOP;
                    case PrimitiveTopology::Lines: return GL_LINES;
                    case PrimitiveTopology::TriangleStrip: return GL_TRIANGLE_STRIP;
                    case PrimitiveTopology::TriangleFan: return GL_TRIANGLE_FAN;
                    case PrimitiveTopology::Triangles: return GL_TRIANGLES;
                    case PrimitiveTopology::Quads: return GL_QUADS;
                    default: {
                        JOH_ASSERT(false, "PrimitiveTopology is invalid.");
                        return 0;
                    }
                }
            }
        }
        
        void GLRenderer::Apply() {
            ApplyShaderState();
            ApplyInputAssemberState();
			ApplyTextures();
        }
        
        void GLRenderer::ApplyInputAssemberState() {            
            for (uint32_t streamIndex = 0; streamIndex < kMaxStreams; ++streamIndex) {
                internal::Stream const& stream = streams_[streamIndex];
				
				size_t streamSize = 0;
				for (size_t i = 0; i < stream.attributes.size(); ++i) {
                    InputElement const& inputElement = stream.attributes[i];
					streamSize += GetInputElementFormatSize(inputElement.format) * inputElement.componentCount;
				}

                for (size_t i = 0; i < stream.attributes.size(); ++i) {
                    InputElement const& inputElement = stream.attributes[i];

                    if (currentVertexBuffer_ != stream.vertexBuffer) {
                        GLBuffer* vbo = static_cast< GLBuffer* >(stream.vertexBuffer);
                        glBindBuffer(GL_ARRAY_BUFFER, vbo->GetHandle());
                        currentVertexBuffer_ = stream.vertexBuffer;
                    }

                    glEnableVertexAttribArray(inputElement.semanticIndex);
                    glVertexAttribPointer(
                        inputElement.semanticIndex,
                        inputElement.componentCount,
                        InputElementFormatToGLElementFormat(inputElement.format),
                        GL_FALSE,
                        (GLsizei)streamSize, //vertexFormats_[currentVertexFormat_].GetVertexSize(streamIndex),
                        reinterpret_cast< char* >(stream.offset) + inputElement.offset
                    );
                }
            }
        }
        
        void GLRenderer::ApplyShaderState() {
            if (currentShader_ != newShader_) {
                glUseProgram(static_cast< GLShader* >(newShader_)->GetProgramHandle());
                currentShader_ = newShader_;
            }
            
            currentShader_->Apply();
        }

		void GLRenderer::ApplyTextures() {
			for (int i = 0; i < kTextureUnits; ++i) {
				GLTexture2D* currentTexture = static_cast< GLTexture2D* >(currentTextures_[i]);
				GLTexture2D* newTexture = static_cast< GLTexture2D* >(newTextures_[i]);
				if (currentTexture != newTexture) {
					glActiveTexture(GL_TEXTURE0 + i);
					CHECK_GL_ERROR();
					if (newTexture != 0) {
						glEnable(GL_TEXTURE_2D);
						glBindTexture(GL_TEXTURE_2D, newTexture->GetHandle());
						CHECK_GL_ERROR();
					}
					else {
						glBindTexture(GL_TEXTURE_2D, 0);
					}
					
					currentTextures_[i] = newTextures_[i];
				}
			}
		}
        
        VertexFormatID GLRenderer::CreateVertexFormat(InputElement const* inputElements, size_t count) {
            JOH_ASSERT(inputElements != 0, "inputElements is null");
            VertexFormat vertexFormat(inputElements, count);
            vertexFormats_.push_back(vertexFormat);
            return static_cast< VertexFormatID >(vertexFormats_.size() - 1);
        }
        
        Buffer* GLRenderer::CreateBuffer(BufferDescription const& desciption, void const* data) {
            if (data != 0) {
                return (new GLBuffer(desciption, data));
            }
            else {
                return (new GLBuffer(desciption));
            }
        }
        
        Shader* GLRenderer::CreateShader(ShaderDescription const& desciption) {
            return (new GLShader(desciption));
        }

		Texture2D* GLRenderer::CreateTexture2D(Texture2DDescription const& description) {
			return (new GLTexture2D(description));
		}

		Texture2D* GLRenderer::CreateTexture2D(Texture2DDescription const& description, void const* data) {
			return (new GLTexture2D(description, data));
		}
        
        void GLRenderer::Draw(uint32_t startVertexLocation, uint32_t verticeCount) {
            glDrawArrays(PrimitiveTopologyToGLPrimitiveMode(currentPrimitiveTopology_), (GLsizei)startVertexLocation, (GLint)verticeCount);
        }
        
        void GLRenderer::DrawIndexed(uint32_t startIndexLocation, uint32_t indiceCount) {
            JOH_ASSERT(currentIndexBuffer_ != 0, "Can draw index when no index buffer is specified");
            uint32_t elementSize = currentIndexBuffer_->GetElementSize();
            GLenum type;
            if (elementSize == sizeof(uint8_t)) type = GL_UNSIGNED_BYTE;
            else if (elementSize == sizeof(uint16_t)) type = GL_UNSIGNED_SHORT;
            else if (elementSize == sizeof(uint32_t)) type = GL_UNSIGNED_INT;
            else JOH_ASSERT(false,  "Invalid element size for index buffer");
            GLvoid const* offset = reinterpret_cast< GLvoid const* >(elementSize * startIndexLocation);
            glDrawElements(PrimitiveTopologyToGLPrimitiveMode(currentPrimitiveTopology_), (GLsizei)indiceCount, type, offset);
        }
        
        PrimitiveTopology::Enum GLRenderer::GetPrimitiveTopology() const {
            return currentPrimitiveTopology_;
        }
        
        void GLRenderer::SetPrimitiveTopology(PrimitiveTopology::Enum topology) {
            currentPrimitiveTopology_ = topology;
        }
        
        void GLRenderer::SetIndexBuffer(Buffer* indexBuffer) {
            JOH_ASSERT(indexBuffer->GetType() == ResourceType::IndexBuffer, "invalid buffer type passed in for indexBuffer");
            if (currentIndexBuffer_ != 0 && currentIndexBuffer_ != indexBuffer) {
                currentIndexBuffer_ = indexBuffer;
                GLBuffer* ibo = static_cast< GLBuffer* >(currentIndexBuffer_);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo->GetHandle());
            }
        }
        
        void GLRenderer::SetShader(Shader* shader) {
            newShader_ = shader;
        }

		void GLRenderer::SetTexture(char const* name, Texture2D* texture) {
            JOH_ASSERT(name != 0, "texture name is null");
            JOH_ASSERT(texture != 0, "texture is null");
			GLShader* shader = 0;
			
			// This is a small hack so that SetTexture is applied to the correct shader
			if (currentShader_ != newShader_) {
				shader = static_cast< GLShader* >(newShader_);
			}
			else {
				shader = static_cast< GLShader* >(currentShader_);
			}
			
			int unit = shader->GetSamplersUnit(name);
			if (unit >= 0) {
				newTextures_[unit] = texture;
			}
		}
        
        void GLRenderer::SetVertexBuffer(Buffer* vertexBuffer, uint32_t const stream, uintptr_t const offset) {
            JOH_ASSERT(stream < kMaxStreams, "'stream' is out of index");
            JOH_ASSERT(vertexBuffer->GetType() == ResourceType::VertexBuffer, "invalid buffer type passed in for vertexBuffer");
            streams_[stream].vertexBuffer = vertexBuffer;
            streams_[stream].offset = offset;
            if (currentVertexBuffer_ != vertexBuffer) {
                GLBuffer* vbo = static_cast< GLBuffer* >(vertexBuffer);
                glBindBuffer(GL_ARRAY_BUFFER, vbo->GetHandle());
                currentVertexBuffer_ = vertexBuffer;
            }
        }
        
        void GLRenderer::SetVertexFormat(VertexFormatID vertexFormat, Shader* shader) {
            JOH_UNUSED(shader);
            if (currentVertexFormat_ != vertexFormat) {
                for (size_t i = 0; i < kMaxStreams; ++i) {
                    streams_[i].attributes.clear();
                }
                
                VertexFormat const& vf = vertexFormats_[(size_t)vertexFormat];
                for (uint32_t i = 0; i < vf.GetInputElementCount(); ++i) {                    
                    InputElement const& inputElement = vf.GetInputElement(i);
                    streams_[inputElement.stream].attributes.push_back(inputElement);
                }
                
                currentVertexFormat_ = vertexFormat;
            }
        }
    }
}
