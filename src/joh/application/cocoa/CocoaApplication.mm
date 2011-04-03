#if JOH_PLATFORM == JOH_PLATFORM_MACOSX

#include "joh/application/cocoa/CocoaApplication.hpp"
#include "joh/application/cocoa/JHResponder.h"
#include "joh/application/Input.hpp"
#include "joh/Assert.hpp"

@interface JHApplicationDelegate : NSObject {
	JHResponder* inputFeed_;
    joh::application::CocoaApplication* app_;
}

- (void)dealloc;
- (void)setApplication:(joh::application::CocoaApplication*)app;
@end


@implementation JHApplicationDelegate

- (void)dealloc {
	[inputFeed_ release];
	[super dealloc];
}

- (void)setApplication:(joh::application::CocoaApplication*)app {
	app_ = app;
}

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
	JOH_ASSERT(app_ != 0, "CocoaApplication is null");
	app_->OnStart();
	NSApplication* app = [aNotification object];
	inputFeed_ = [[JHResponder alloc] initWithWindow:[app mainWindow]];
}

- (void)applicationWillTerminate:(NSNotification *)aNotification {
	JOH_UNUSED(aNotification);
    JOH_ASSERT(app_ != 0, "CocoaApplication is null");
	app_->OnClose();
}

- (NSApplicationTerminateReply)applicationShouldTerminate:(NSApplication *)sender {
    JOH_UNUSED(sender);
	return YES;
}

@end

@interface NSApplication (JHApplication)
- (void)setAppleMenu:(NSMenu *)menu;
@end

namespace joh
{
    namespace application
    {
		namespace
		{
			void ConstructMenu() {
				[NSApp setMainMenu:[[NSMenu alloc] init]];
				NSString* appName = [[NSProcessInfo processInfo] processName];
				NSMenu* menu = [[NSMenu alloc] initWithTitle:@""];
				[menu addItemWithTitle:[@"About " stringByAppendingString:appName] action:@selector(orderFrontStandardAboutPanel:) keyEquivalent:@""];
				[menu addItem:[NSMenuItem separatorItem]];
				[menu addItemWithTitle:[@"Hide " stringByAppendingString:appName] action:@selector(hide:) keyEquivalent:@""];
				NSMenuItem* hideOthers = [menu addItemWithTitle:@"Hide Others" action:@selector(hideOtherApplications:) keyEquivalent:@""];
				[hideOthers setKeyEquivalentModifierMask:(NSAlternateKeyMask|NSCommandKeyMask)];
				[menu addItemWithTitle:@"Show All" action:@selector(unhideAllApplications:) keyEquivalent:@""];
				[menu addItem:[NSMenuItem separatorItem]];
				[menu addItemWithTitle:[@"Quit " stringByAppendingString:appName] action:@selector(terminate:) keyEquivalent:@""];
				
				NSMenuItem* menuItem = [[NSMenuItem alloc] initWithTitle:@"" action:nil keyEquivalent:@""];
			    [menuItem setSubmenu:menu];
			    [[NSApp mainMenu] addItem:menuItem];
			    [menuItem release];

			    [NSApp setAppleMenu:menu];
			    [menu release];
			}
    		
    		void ProcessCocoaEvents() {
                NSEvent* event = nil;
    		    while((event = [NSApp nextEventMatchingMask:NSAnyEventMask
    		                            untilDate:nil
    		                            inMode:NSDefaultRunLoopMode
                                        dequeue:YES]) != nil)
                {                    
                    [[NSApp mainMenu] update];
                    [NSApp sendEvent:event];
                    [NSApp updateWindows];
                }
    		}
		}
		
		
		CocoaApplication::CocoaApplication() {
			pool_ = [[NSAutoreleasePool alloc] init];
			appDelegate_ = [[[JHApplicationDelegate alloc] init] autorelease];
			[appDelegate_ setApplication:this];
		}
        
        CocoaApplication::~CocoaApplication() {
			[pool_ drain];
		}
	
        void CocoaApplication::Launch() {
            ProcessSerialNumber psn;
		    if (!GetCurrentProcess(&psn)) {
		        TransformProcessType(&psn, kProcessTransformToForegroundApplication);
		        SetFrontProcess(&psn);
		    }
			
			[NSApplication sharedApplication];
			[NSApp setDelegate:appDelegate_];
			ConstructMenu();
			[NSApp finishLaunching];
			
			while (true) {
				ProcessCocoaEvents();
				Input::Update();
				Update();
			}
        }
    }
}

#endif // JOH_PLATFORM == JOH_PLATFORM_MACOSX
