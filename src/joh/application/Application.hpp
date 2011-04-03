#ifndef JOH_APPLICATION_APPLICATION_HPP
#define JOH_APPLICATION_APPLICATION_HPP

namespace joh
{
    namespace application
    {
        class Application
        {            
        public:
            
            virtual ~Application() { }
            
			virtual void OnStart() = 0;
			
			virtual void OnClose() = 0;

			virtual void Update() { };
						
			// This is not meant to be overriden by the user
			virtual void Launch() = 0;		
        };
    }
}

#endif // JOH_APPLICATION_APPLICATION_HPP
