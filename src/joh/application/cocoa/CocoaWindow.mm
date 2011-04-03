#if JOH_PLATFORM == JOH_PLATFORM_MACOSX

#include "joh/application/cocoa/CocoaWindow.hpp"
#include "joh/application/cocoa/CocoaGraphicsContext.hpp"
#include "joh/math/Mathf.hpp"

namespace joh
{   
    namespace application
    {
        namespace
        {
    		// NSTitledWindowMask | NSClosableWindowMask | NSMiniaturizableWindowMask
    		NSUInteger WindowStyleToCocoaWindowStyle(uint32_t mask) {
    			NSUInteger result = NSClosableWindowMask;
    			if (mask & WindowStyle::Minimize) {
    				result |= NSMiniaturizableWindowMask;
    			}

    			if (mask & WindowStyle::Normal) {
    				result |= NSTitledWindowMask;
    			}

    			return result;
    		}
        }
        
        
        CocoaWindow::~CocoaWindow() {
			Close();
        }

		std::string CocoaWindow::GetTitle() const {
            NSString* title = [window_ title];
            std::string result([title UTF8String]);
            [title release];
            return result;
		}

        joh::math::Vector2< int32_t > CocoaWindow::GetPosition() const {
			JOH_ASSERT(created_, "window is not created");			
	        NSRect frame = [window_ frame];
            float x = joh::math::Mathf::Round((float)frame.origin.x), y = joh::math::Mathf::Round((float)frame.origin.y);
            return joh::math::Vector2< int32_t >(static_cast< int32_t >(x), static_cast< int32_t >(y));
		}

        joh::math::Vector2< int32_t > CocoaWindow::GetSize() const {
            JOH_ASSERT(created_, "window is not created");
			NSRect frame = [view_ frame];
            float x = joh::math::Mathf::Round((float)frame.size.width), y = joh::math::Mathf::Round((float)frame.size.height);
            return joh::math::Vector2< int32_t >(static_cast< int32_t >(x), static_cast< int32_t >(y));
        }

        WindowStyle::Enum CocoaWindow::GetStyle() const {
            JOH_ASSERT(created_, "window is not created");			
			if ([window_ isMiniaturized] == YES) {
                return WindowStyle::Minimize;
            }
            else if ([view_ isInFullScreenMode] == YES) {
                return WindowStyle::FullScreen;
            }
            
            return WindowStyle::Normal;
        }

        void CocoaWindow::Create(
            std::string const& name,
            joh::math::Vector2< int32_t > const& position,
            joh::math::Vector2< int32_t > const& size,
            uint32_t style
        )
        {
            NSRect rect = NSMakeRect(position.x, position.y, size.x, size.y);
            NSUInteger styleMask = WindowStyleToCocoaWindowStyle(style);
            window_ = [[NSWindow alloc] initWithContentRect:rect styleMask:styleMask backing:NSBackingStoreBuffered defer:NO];			
			view_ = [[NSView alloc] initWithFrame:rect];
			
			[window_ setContentView:view_];
			[window_ makeKeyAndOrderFront:nil];
			created_ = true;
			
			SetTitle(name);
			
			bool canFullscreen = (style & WindowStyle::FullScreen);
			bool canWindow = (style & WindowStyle::Normal);
			if (canFullscreen && canWindow) {
				windowStyleRestriction_ = kWindowOrFullscreen;
			}
			else if (canFullscreen) {
				windowStyleRestriction_ = kFullscreenOnly;
			}
			else if (canWindow) {
				windowStyleRestriction_ = kWindowOnly;
			}
			
			std::vector< NSOpenGLPixelFormatAttribute > attributes;
			attributes.push_back(NSOpenGLPFANoRecovery);
			attributes.push_back(NSOpenGLPFADoubleBuffer);
			attributes.push_back(NSOpenGLPFAAccelerated);
			attributes.push_back(NSOpenGLPFABackingStore);
			attributes.push_back(NSOpenGLPFAColorSize); attributes.push_back(24);
			attributes.push_back(NSOpenGLPFAAlphaSize); attributes.push_back(8);
			attributes.push_back(NSOpenGLPFADepthSize); attributes.push_back(24);
			attributes.push_back(NSOpenGLPFAStencilSize); attributes.push_back(8);
            //if (windowStyleRestriction_ == kFullscreenOnly || windowStyleRestriction_ == kWindowOrFullscreen) {
			//	attributes.push_back(NSOpenGLPFAFullScreen);
			//}
			
			//if (windowStyleRestriction_ == kWindowOnly || windowStyleRestriction_ == kWindowOrFullscreen) {
			//	attributes.push_back(NSOpenGLPFAWindow);
			//}
			attributes.push_back(0);
			pixelFormat_ = [[NSOpenGLPixelFormat alloc] initWithAttributes: static_cast< NSOpenGLPixelFormatAttribute* >(&attributes[0])];
            JOH_ASSERT(pixelFormat_ != nil, "Unable to create Cocoa OpenGL pixel format");
        }

		void CocoaWindow::Close() {
            created_ = false;
            graphicsContext_ = 0;
            [pixelFormat_ release];
            [view_ release];
            [window_ close];
		}

        void CocoaWindow::MakeFocus() {
			JOH_ASSERT(created_, "window is not created");
			[window_ makeKeyAndOrderFront:nil];
		}

        void CocoaWindow::SetTitle(std::string const& title) {
			JOH_ASSERT(created_, "window is not created");
			NSString* string = [[NSString alloc] initWithUTF8String:title.c_str()];
			[window_ setTitle:string];
			[string release];
		}

        void CocoaWindow::SetPosition(joh::math::Vector2< int32_t > const& position) {
			JOH_ASSERT(created_, "window is not created");
			NSRect frame = [window_ frame];
			frame.origin.x = position.x;
			frame.origin.y = position.y;
			[window_ setFrame:frame display:YES];
		}

        void CocoaWindow::SetSize(joh::math::Vector2< int32_t > const& size) {
			JOH_ASSERT(created_, "window is not created");
			NSRect frame = [window_ frame];
			frame.size.width = size.x;
			frame.size.height = size.y;
			[window_ setFrame:frame display:YES];
		}

        void CocoaWindow::SetStyle(WindowStyle::Enum style) {
			JOH_ASSERT(created_, "window is not created");
			bool inFullScreen = [view_ isInFullScreenMode];
			if (style == WindowStyle::Minimize) {
				if (!inFullScreen) {
					[window_ performMiniaturize:nil];
				}
			}
			else {
				if (windowStyleRestriction_ == kWindowOrFullscreen) {
					if (style == WindowStyle::Normal && inFullScreen) {
						[view_ exitFullScreenModeWithOptions:nil];
					}
					else if (style == WindowStyle::FullScreen && !inFullScreen) {
						[view_ enterFullScreenMode:[window_ screen] withOptions:nil];
					}
				}
			}
		}

        void CocoaWindow::ShowCursor(bool show) {
			JOH_ASSERT(created_, "window is not created");
			if (show) {
				CGDisplayShowCursor(kCGDirectMainDisplay);
			}
			else {
				CGDisplayHideCursor(kCGDirectMainDisplay);
			}
		}

        void CocoaWindow::SyncToVBL(bool sync) {
			JOH_ASSERT(created_, "window is not created");
			JOH_ASSERT(graphicsContext_ != 0, "graphicsContext_ is null");
			CocoaGraphicsContext* context = static_cast< CocoaGraphicsContext* > (graphicsContext_);
            GLint vbl = sync ? 1 : 0;
			[context->GetOpenGLContext() setValues:&vbl forParameter:NSOpenGLCPSwapInterval];
		}

        void CocoaWindow::Update() {
			JOH_ASSERT(created_, "window is not created");
		}
    }
}

#endif // JOH_PLATFORM == JOH_PLATFORM_MACOSX
