#ifndef JOH_GRAPHICS_GRAPHICSCONTEXT_HPP
#define JOH_GRAPHICS_GRAPHICSCONTEXT_HPP

#include "joh/graphics/GraphicsForwards.hpp"

namespace joh
{
    namespace graphics
    {
        class GraphicsContext
        {	
        public:
            
            virtual ~GraphicsContext() { };
            
            virtual void Update() = 0;
			
			virtual bool Begin() = 0;
            
            virtual bool End() = 0;
        };		
    }
}

#endif // JOH_GRAPHICS_GRAPHICSCONTEXT_HPP
