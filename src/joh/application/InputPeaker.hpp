#ifndef JOH_APPLICATION_INPUTPEAKER_HPP
#define JOH_APPLICATION_INPUTPEAKER_HPP

#include "joh/application/Input.hpp"

namespace joh
{
    namespace application
    {
        namespace internal
        {
            struct InputImplementation;
            class InputPeaker
            {
            public:
                
                static InputImplementation& GetInputImplementation() {
                    return Input::implementation_;
                }
            };
        }
    }
}

#endif // JOH_APPLICATION_INPUTPEAKER_HPP
