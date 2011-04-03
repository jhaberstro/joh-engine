#if JOH_PLATFORM == JOH_PLATFORM_MACOSX

#include "joh/application/cocoa/CocoaGraphicsContext.hpp"
#include "joh/application/cocoa/CocoaWindow.hpp"

namespace joh
{
    namespace application
    {		
		CocoaGraphicsContext::CocoaGraphicsContext(Window* window) {
			JOH_ASSERT(window != 0, "window is null");
            joh::application::CocoaWindow* cocoaWindow = static_cast< joh::application::CocoaWindow* >(window);
            openglContext_ = [[NSOpenGLContext alloc] initWithFormat:cocoaWindow->GetOpenGLPixelFormat() shareContext:nil];
            [openglContext_ setView: cocoaWindow->GetCocoaView()];
            [openglContext_ makeCurrentContext];
		}

        CocoaGraphicsContext::~CocoaGraphicsContext() {
            [openglContext_ release];
        }
        
        void CocoaGraphicsContext::Update() {
            [openglContext_ update];
        }
        
        bool CocoaGraphicsContext::Begin() {
            [openglContext_ makeCurrentContext];
            return true;
        }
        
        bool CocoaGraphicsContext::End() {
            [openglContext_ flushBuffer];
            return true;
        }
    }
}

#endif // JOH_PLATFORM == JOH_PLATFORM_MACOSX
