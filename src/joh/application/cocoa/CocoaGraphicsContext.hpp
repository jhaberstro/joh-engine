#ifndef JOH_GRAPHICS_COCOA_COCOAGRAPHICSCONTEXT_HPP
#define JOH_GRAPHICS_COCOA_COCOAGRAPHICSCONTEXT_HPP

#if JOH_PLATFORM == JOH_PLATFORM_MACOSX

#include "joh/graphics/GraphicsContext.hpp"
#include <Cocoa/Cocoa.h>

namespace joh
{
    namespace application
    {
        class Window;
        class CocoaGraphicsContext : public joh::graphics::GraphicsContext
        {	
        public:

			CocoaGraphicsContext(Window* window);

            virtual ~CocoaGraphicsContext();

			virtual void Update();
            
            virtual bool Begin();
            
            virtual bool End();

			NSOpenGLContext* GetOpenGLContext() const;

		private:
			
			NSOpenGLContext* openglContext_;
        };

		inline NSOpenGLContext* CocoaGraphicsContext::GetOpenGLContext() const {
			return openglContext_;
		}
    }
}

#endif // JOH_PLATFORM == JOH_PLATFORM_MACOSX
#endif // JOH_GRAPHICS_COCOA_COCOAGRAPHICSCONTEXT_HPP

