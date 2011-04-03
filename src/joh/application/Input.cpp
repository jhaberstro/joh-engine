#if JOH_PLATFORM == JOH_PLATFORM_MACOSX

#include "joh/application/Input.hpp"
#include "joh/Assert.hpp"

namespace joh
{
    namespace application
    {         
        internal::InputImplementation Input::implementation_;
         
        bool Input::IsKeyHeld(KeyCode::Enum key) {
			JOH_ASSERT(key < KeyCode::KeyCodeCount, "input key code 'key' is invalid.");
			return implementation_.keyHeld[key];
        }
        
        bool Input::IsKeyDown(KeyCode::Enum key) {
			JOH_ASSERT(key < KeyCode::KeyCodeCount, "input key code 'key' is invalid.");
			return implementation_.keyDown[key];
		}
        
        bool Input::IsKeyUp(KeyCode::Enum key) {
			JOH_ASSERT(key < KeyCode::KeyCodeCount, "input key code 'key' is invalid.");
			return implementation_.keyUp[key];
		}
        
        bool Input::IsAnyKey() {
			return implementation_.keyCount > 0;
		}
        
        joh::math::Vector2f Input::GetMousePosition() {
			return implementation_.mousePosition;
		}
        
        void Input::Update() {
			for (int i = 0; i < KeyCode::KeyCodeCount; ++i) {
				implementation_.keyUp[i] = implementation_.keyUpQueue[i];
				implementation_.keyUpQueue[i] = false;
				
				implementation_.keyDown[i] = implementation_.keyDownQueue[i];
				implementation_.keyDownQueue[i] = false;
			}
		}
    }
}


#endif // JOH_PLATFORM == JOH_PLATFORM_MACOSX
