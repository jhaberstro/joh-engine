#ifndef JOH_APPLICATION_INPUT_HPP
#define JOH_APPLICATION_INPUT_HPP

#include "joh/joh.hpp"
#include "joh/math/Vector2.hpp"

namespace joh
{
    namespace application
    {
        namespace KeyCode
        {
            enum Enum
            {
                Backspace,
                Delete,
                Tab,
                Return,
                Escape,
                Space,
                Zero,
                One,
                Two,
                Three,
                Four,
                Five,
                Six,
                Seven,
                Eight,
                Nine,
                Minus,
                Equals,
                UpArrow,
                DownArrow,
                RightArrow,
                LeftArrow,
                LeftBracket,
                RightBracket,
                BackSlash,
                Semicolon,
                SingleQuote,
                Comma,
                Period,
                Slash,
                A,
                B,
                C,
                D,
                E,
                F,
                G,
                H,
                I,
                J,
                K,
                L,
                M,
                N,
                O,
                P,
                Q,
                R,
                S,
                T,
                U,
                V,
                W,
                X,
                Y,
                Z,
                LeftShift,
                RightShift,
                LeftControl,
                RightControl,
                LeftAlt,
                RightAlt,
                LeftApple,
                RightApple,
                LeftMouse,
                MiddleMouse,
                RightMouse,
                KeyCodeCount
            };
        }
        
        namespace internal
        {
            struct InputImplementation
            {
				int keyCount;
                bool keyHeld[KeyCode::KeyCodeCount];
                bool keyDown[KeyCode::KeyCodeCount];
                bool keyDownQueue[KeyCode::KeyCodeCount];
            	bool keyUp[KeyCode::KeyCodeCount];
                bool keyUpQueue[KeyCode::KeyCodeCount];
                joh::math::Vector2f mousePosition;
            };
        }
        
        namespace internal { class InputPeaker; }
        class Input
        {
            friend class internal::InputPeaker;
                        
        public:
                   
            static bool IsKeyHeld(KeyCode::Enum key);
            
            static bool IsKeyDown(KeyCode::Enum key);
            
            static bool IsKeyUp(KeyCode::Enum key);
            
            static bool IsAnyKey();
            
            static joh::math::Vector2f GetMousePosition();
            
            static void Update();
            
        protected:
            
            static internal::InputImplementation implementation_;
        };
    }
}

#endif // JOH_APPLICATION_INPUT_HPP
