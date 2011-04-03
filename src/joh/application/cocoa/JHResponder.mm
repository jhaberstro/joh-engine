#if JOH_*PLATFORM == JOH_*PLATFORM_MACOSX

#include "joh/application/cocoa/JHResponder.h"
#include "joh/application/InputPeaker.hpp"
#import <Carbon/Carbon.h>

unichar map[] = {
	'\0',
    '\0',
    '\0',
    '\0',
    '\0',
    '\0',
    '0',
    '1',
    '2',
    '3',
    '4',
    '5',
    '6',
    '7',
    '8',
    '9',
    '-',
    '=',
    '\0',
    '\0',
    '\0',
    '\0',
    '[',
    ']',
    '\\',
    ';',
    '\'',
    ',',
    '.',
    '/',
    'a',
    'b',
    'c',
    'd',
    'e',
    'f',
    'g',
    'h',
    'i',
    'j',
    'k',
    'l',
    'm',
    'n',
    'o',
    'p',
    'q',
    'r',
    's',
    't',
    'u',
    'v',
    'w',
    'x',
    'u',
    'z'
};

using namespace joh;
using namespace joh::application;

KeyCode::Enum GetKeyCode(unichar key) {
	for (int i = 0; i < KeyCode::Z; ++i) {
		if (map[i] == key) {
			return static_cast< KeyCode::Enum >(i);
		}
	}
	
	return KeyCode::KeyCodeCount;
}

void OnKeyDown(NSEvent* theEvent, KeyCode::Enum key) {
	joh::application::internal::InputImplementation& data = joh::application::internal::InputPeaker::GetInputImplementation();
    bool repeat = [theEvent isARepeat];
	data.keyDownQueue[key] = !repeat;
	data.keyUpQueue[key] = false;
	data.keyHeld[key] = (repeat || (!repeat && !data.keyHeld[key]));
	if (!repeat) {
		data.keyCount += 1;
	}
}

void OnKeyUp(NSEvent* theEvent, KeyCode::Enum key) {
	joh::application::internal::InputImplementation& data = joh::application::internal::InputPeaker::GetInputImplementation();
	data.keyUpQueue[key] = true;
	data.keyDownQueue[key] = false;
	data.keyHeld[key] = false;
	data.keyCount -= 1;
}

void OnModifierKey(NSEvent* theEvent, KeyCode::Enum key) {
	joh::application::internal::InputImplementation& data = joh::application::internal::InputPeaker::GetInputImplementation();
	
	// key released
	// should i add a check for keyDownQueue[key] == true here?
	if (data.keyHeld[key] == true) {
		data.keyDownQueue[key] = false;
		data.keyHeld[key] = false;
		data.keyUpQueue[key] = true;
		data.keyCount -= 1;
	}
	// key down
	else {
		data.keyDownQueue[key] = true;
		data.keyHeld[key] = true;
		data.keyUpQueue[key] = false;
		data.keyCount += 1;
	}
}

void OnMouseDown(NSEvent* theEvent, KeyCode::Enum key) {
	joh::application::internal::InputImplementation& data = joh::application::internal::InputPeaker::GetInputImplementation();
	data.keyDownQueue[key] = true;
	data.keyUpQueue[key] = false;
	data.keyHeld[key] = true;
	data.keyCount += 1;
}

void OnMouseUp(NSEvent* theEvent, KeyCode::Enum key) {
	joh::application::internal::InputImplementation& data = joh::application::internal::InputPeaker::GetInputImplementation();
	data.keyDownQueue[key] = false;
	data.keyUpQueue[key] = true;
	data.keyHeld[key] = false;
	data.keyCount -= 1;
}

@implementation JHResponder

- (BOOL)acceptsFirstResponder {
	return YES;
}

- (void)mouseMoved:(NSEvent*)theEvent {
	NSWindow* window = [theEvent window];
	if (window == nil) {
		[[self nextResponder] mouseMoved:theEvent];
		return;
	}
	
	NSView* view = [window contentView];
	if (view == nil) {
		[[self nextResponder] mouseMoved:theEvent];
		return;
	}
	
	NSPoint eventLocation = [theEvent locationInWindow];
	NSPoint point = [view convertPoint:eventLocation fromView:nil];
	if ([view mouse:point inRect:[view bounds]] == YES) {
        joh::application::internal::InputPeaker::GetInputImplementation().mousePosition = joh::math::Vector2f((float)point.x, (float)point.y);
	}
}

- (void)mouseDown:(NSEvent*)theEvent {
	OnMouseDown(theEvent, KeyCode::LeftMouse);
	[[self nextResponder] mouseDown:theEvent];
}

- (void)mouseUp:(NSEvent*)theEvent {
	OnMouseUp(theEvent, KeyCode::LeftMouse);
	[[self nextResponder] mouseUp:theEvent];
}

- (void)rightMouseDown:(NSEvent*)theEvent {
	OnMouseDown(theEvent, KeyCode::RightMouse);
	[[self nextResponder] rightMouseDown:theEvent];
}

- (void)rightMouseUp:(NSEvent*)theEvent {
	OnMouseUp(theEvent, KeyCode::RightMouse);
	[[self nextResponder] rightMouseUp:theEvent];
}

- (void)otherMouseDown:(NSEvent*)theEvent {
	OnMouseDown(theEvent, KeyCode::MiddleMouse);
	[[self nextResponder] otherMouseDown:theEvent];
}

- (void)otherMouseUp:(NSEvent*)theEvent {
	OnMouseUp(theEvent, KeyCode::MiddleMouse);
	[[self nextResponder] otherMouseUp:theEvent];
}

- (void)flagsChanged:(NSEvent *)theEvent {
	unsigned short keyCode = [theEvent keyCode];
	// For some reason, there is no kVK_RightCommand, but pressing the right command
	// button will yield a keyCode 0x36
	switch (keyCode) {
		case kVK_Shift: 		OnModifierKey(theEvent, KeyCode::LeftShift); break;
		case kVK_RightShift: 	OnModifierKey(theEvent, KeyCode::RightShift); break;
		case kVK_Control: 		OnModifierKey(theEvent, KeyCode::LeftControl); break;
		case kVK_RightControl: 	OnModifierKey(theEvent, KeyCode::RightControl); break;
		case kVK_Option: 		OnModifierKey(theEvent, KeyCode::LeftAlt); break;
		case kVK_RightOption: 	OnModifierKey(theEvent, KeyCode::RightAlt); break;
		case kVK_Command: 		OnModifierKey(theEvent, KeyCode::LeftApple); break;
		case 0x36: 				OnModifierKey(theEvent, KeyCode::RightApple); break;
	}
	
	[[self nextResponder] flagsChanged:theEvent];
}

- (void)keyDown:(NSEvent*)theEvent {
	unsigned short keyCode = [theEvent keyCode];
	switch(keyCode) {
		case kVK_Delete: 		OnKeyDown(theEvent, KeyCode::Backspace); break;
		case kVK_ForwardDelete: OnKeyDown(theEvent, KeyCode::Delete); break;
		case kVK_Tab: 			OnKeyDown(theEvent, KeyCode::Tab); break;
		case kVK_Return: 		OnKeyDown(theEvent, KeyCode::Return); break;
		case kVK_Escape: 		OnKeyDown(theEvent, KeyCode::Escape); break;
		case kVK_Space: 		OnKeyDown(theEvent, KeyCode::Space); break;
		case kVK_UpArrow: 		OnKeyDown(theEvent, KeyCode::UpArrow); break;
		case kVK_DownArrow: 	OnKeyDown(theEvent, KeyCode::DownArrow); break;
		case kVK_RightArrow: 	OnKeyDown(theEvent, KeyCode::RightArrow); break;
		case kVK_LeftArrow: 	OnKeyDown(theEvent, KeyCode::LeftArrow); break;
	}
	
	NSString* characters = [theEvent charactersIgnoringModifiers];
	for (unsigned int i = 0; i < [characters length]; ++i) {
		unichar character = [characters characterAtIndex:i];
		KeyCode::Enum keyCode = GetKeyCode(character);
		if (keyCode != KeyCode::KeyCodeCount) {
			OnKeyDown(theEvent, keyCode);
		}
	}
	
	[[self nextResponder] keyDown:theEvent];
}

- (void)keyUp:(NSEvent*)theEvent {
	unsigned short keyCode = [theEvent keyCode];
	switch(keyCode) {
		case kVK_Delete: 		OnKeyUp(theEvent, KeyCode::Backspace); break;
		case kVK_ForwardDelete: OnKeyUp(theEvent, KeyCode::Delete); break;
		case kVK_Tab: 			OnKeyUp(theEvent, KeyCode::Tab); break;
		case kVK_Return: 		OnKeyUp(theEvent, KeyCode::Return); break;
		case kVK_Escape: 		OnKeyUp(theEvent, KeyCode::Escape); break;
		case kVK_Space: 		OnKeyUp(theEvent, KeyCode::Space); break;
		case kVK_UpArrow: 		OnKeyUp(theEvent, KeyCode::UpArrow); break;
		case kVK_DownArrow: 	OnKeyUp(theEvent, KeyCode::DownArrow); break;
		case kVK_RightArrow: 	OnKeyUp(theEvent, KeyCode::RightArrow); break;
		case kVK_LeftArrow: 	OnKeyUp(theEvent, KeyCode::LeftArrow); break;
	}
	
	NSString* characters = [theEvent charactersIgnoringModifiers];
	for (unsigned int i = 0; i < [characters length]; ++i) {
		unichar character = [characters characterAtIndex:i];
		KeyCode::Enum keyCode = GetKeyCode(character);
		if (keyCode != KeyCode::KeyCodeCount) {
			OnKeyUp(theEvent, keyCode);
		}
	}
	
	[[self nextResponder] keyUp:theEvent];
}

@end

#endif // #if JOH_*PLATFORM == JOH_*PLATFORM_MACOSX

