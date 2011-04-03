#ifndef JOH_APPLICATION_COCOA_RSRESPONDER_H
#define JOH_APPLICATION_COCOA_RSRESPONDER_H
#if JOH_PLATFORM == JOH_PLATFORM_MACOSX

#include <ApplicationServices/ApplicationServices.h>
#include <Cocoa/Cocoa.h>

@interface JHResponder : NSWindowController {
}

- (void)mouseMoved:(NSEvent*)theEvent;
- (void)mouseDown:(NSEvent*)theEvent;
- (void)mouseUp:(NSEvent*)theEvent;
- (void)rightMouseDown:(NSEvent*)theEvent;
- (void)rightMouseUp:(NSEvent*)theEvent;
- (void)otherMouseDown:(NSEvent*)theEvent;
- (void)otherMouseUp:(NSEvent*)theEvent;
- (void)flagsChanged:(NSEvent *)theEvent;
- (void)keyDown:(NSEvent*)theEvent;
- (void)keyUp:(NSEvent*)theEvent;
@end

#endif // #if JOH_PLATFORM == JOH_PLATFORM_MACOSX
#endif // JOH_APPLICATION_COCOA_RSRESPONDER_H
