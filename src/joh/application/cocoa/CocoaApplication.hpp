#ifndef JOH_APPLICATION_COCOA_COCOAPPLICATION_HPP
#define JOH_APPLICATION_COCOA_COCOAPPLICATION_HPP
#if JOH_PLATFORM == JOH_PLATFORM_MACOSX

#include "joh/application/Application.hpp"

extern "C" {
#include <ApplicationServices/ApplicationServices.h>
#include <Cocoa/Cocoa.h>
}


@class JHApplicationDelegate;
namespace joh
{
    namespace application
    {
        class CocoaApplication : public Application
        {            
        public:
				
            CocoaApplication();
            
            virtual ~CocoaApplication();	
					
			// This is not meant to be overriden by the user
		    void Launch();
			
		private:
		    
            JHApplicationDelegate* appDelegate_;   		
            NSAutoreleasePool* pool_;
        };
    }
}

#endif // JOH_PLATFORM == JOH_PLATFORM_MACOSX
#endif
