#ifndef JOH_APPLICATION_COCOA_COCOAWINDOW_HPP
#define JOH_APPLICATION_COCOA_COCOAWINDOW_HPP
#if JOH_PLATFORM == JOH_PLATFORM_MACOSX

#include "JOH/application/Window.hpp"

extern "C" {
#include <ApplicationServices/ApplicationServices.h>
#include <Cocoa/Cocoa.h>
}


namespace joh
{
    namespace application
    {
        class CocoaWindow : public Window
        {			
		protected:
			
			enum WindowStyleRestricitions
			{
				kWindowOnly,
				kFullscreenOnly,
				kWindowOrFullscreen
			};
	
        public:

            virtual ~CocoaWindow();

            virtual std::string GetTitle() const;

            virtual joh::math::Vector2< int32_t > GetPosition() const;

            virtual joh::math::Vector2< int32_t > GetSize() const;

            virtual WindowStyle::Enum GetStyle() const;

            virtual void Create(
                std::string const& name,
                joh::math::Vector2< int32_t > const& position,
                joh::math::Vector2< int32_t > const& size,
                uint32_t style
            );

            virtual void Close();

            virtual void MakeFocus();

            virtual void SetTitle(std::string const& title);

            virtual void SetPosition(joh::math::Vector2< int32_t > const& position);

            virtual void SetSize(joh::math::Vector2< int32_t > const& size);

            virtual void SetStyle(WindowStyle::Enum style);

            virtual void ShowCursor(bool show);

            virtual void SyncToVBL(bool sync);

            virtual void Update();

			NSWindow* GetCocoaWindow() const;
			
			NSView* GetCocoaView() const;
			
			NSOpenGLPixelFormat* GetOpenGLPixelFormat() const;

        protected:
			
            NSWindow* window_;
            NSView* view_;
			NSOpenGLPixelFormat* pixelFormat_;
            NSAutoreleasePool* pool_;
			WindowStyleRestricitions windowStyleRestriction_;
			bool created_;
        };

				
		inline NSWindow* CocoaWindow::GetCocoaWindow() const {
			return window_;
		}
		
		inline NSView* CocoaWindow::GetCocoaView() const {
			return view_;
		}
		
		inline NSOpenGLPixelFormat* CocoaWindow::GetOpenGLPixelFormat() const {
			JOH_ASSERT(created_, "Window has not yet been created");
			return pixelFormat_;
		}
    }
}

#endif // // JOH_PLATFORM == JOH_PLATFORM_MACOSX
#endif // JOH_APPLICATION_COCOA_COCOAWINDOW_HPP
