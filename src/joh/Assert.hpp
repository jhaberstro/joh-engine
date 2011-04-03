#ifndef JOH_ASSERT_HPP
#define JOH_ASSERT_HPP

#ifndef JOH_ASSERT_ENABLED
#   if defined(JOH_DEBUG) || defined(JOH_TEST)
#       define JOH_ASSERT_ENABLED
#   endif
#endif

#ifdef JOH_ASSERT_ENABLED
#   define JOH_ASSERT(condition, message) \
        do { \
            if (!(condition) && joh::Assert::Report(__FILE__, __LINE__, #condition, message)) { \
                JOH_EXIT(); \
            } \
        } while(false)
        
#   define JOH_ASSERTF(condition, message, ...) \
        do { \
            if (!(condition) && joh::Assert::Reportf(__FILE__, __LINE__, #condition, message, __VA_ARGS__)) { \
                JOH_EXIT(); \
            } \
        } while(false)
#else
#   define JOH_ASSERT(condition, message) do { JOH_UNUSED(condition); JOH_UNUSED(message); } while(false)
#   define JOH_ASSERTF(condition, message, ...) do { JOH_UNUSED(condition); JOH_UNUSED(message); } while(false)
#endif

#include "joh/joh.hpp"

namespace joh
{
    class Assert
    {
    public:
        
        typedef bool (*ReportHandler)(char const* file, size_t line, char const* condition, char const* message);
        
        static bool Report(char const* file, size_t line, char const* condition, char const* message);

        static bool Reportf(char const* file, size_t line, char const* condition, char const* message, ...);
        
        static ReportHandler GetReportHandler();
        
        static void SetReportHandler(ReportHandler handler);
        
    private:
        
        Assert();
        
        Assert(Assert const&);
        
        Assert& operator=(Assert const&);
    };
    
}

#endif // JOH_ASSERT_HPP
