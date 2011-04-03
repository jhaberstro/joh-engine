// http://www.opengl.org/registry/api/enum.spec

#ifndef JOH_GRAPHICS_OPENGL_OPENGLFORWARDS_HPP
#define JOH_GRAPHICS_OPENGL_OPENGLFORWARDS_HPP

#if JOH_PLATFORM == JOH_PLATFORM_MACOSX
#   include <OpenGL/gl.h>
#   include <OpenGL/glext.h>
#   include <OpenGL/OpenGL.h>
#else
#   error "Unsupported platform."
#endif

#define CHECK_GL_ERROR() \
	{ GLenum err = glGetError(); JOH_ASSERTF(err == GL_NO_ERROR, "OpenGL op failes with error: %i", err); }

namespace joh
{
    namespace graphics
    {
        class GLBuffer;
        class GLRenderer;
        class GLShader;
        class GLTexture2D;
    }
}

#endif // JOH_GRAPHICS_OPENGL_OPENGLFORWARDS_HPP
