#include "joh/Assert.hpp"
#include "joh/ThreadLocalPtr.hpp"
#include <cstdarg>
#include <cstdio>

namespace joh
{
    namespace
    {
        struct ReportHandlerContainer
        {
            ReportHandlerContainer(Assert::ReportHandler handler)
            : handler_(handler) {
            }
            
            bool operator()(char const* file, size_t line, char const* condition, char const* message) {
                return handler_(file, line, condition, message);
            }
            
            Assert::ReportHandler handler_;
            
        private:
            ReportHandlerContainer(ReportHandlerContainer const& other);
            ReportHandlerContainer& operator=(ReportHandlerContainer const& other);
        };
        
        bool DefaultReportHandler(char const* file, size_t line, char const* condition, char const* message) {
            std::fprintf(stderr, "%s(%d): ASSERTION FAILURE:\n", file, (int)line);
            
            if (condition != 0) {
                std::fprintf(stderr, "'%s'\n", condition);
            }
            
            if (message != 0) {
                std::fprintf(stderr, "%s\n", message);
            }
            
            std::fprintf(stderr, "\n");
            
            return true;
        }
        
        ThreadLocalPtr< ReportHandlerContainer > handler_;
        //Assert::ReportHandler handler_ = &DefaultReportHandler;
    }
    
    bool Assert::Report(char const* file, size_t line, char const* condition, char const* message) {
        ReportHandler handler = GetReportHandler();
        return (handler ? handler(file, line, condition, message) : DefaultReportHandler(file, line, condition, message));    }

    bool Assert::Reportf(char const* file, size_t line, char const* condition, char const* message, ...) {
        if (message != 0) {
            char formattedMessageBuffer[1024];
            std::va_list arguments;
            va_start(arguments, message);
            std::vsnprintf(formattedMessageBuffer, sizeof(formattedMessageBuffer), message, arguments);
            va_end(arguments);
            return Report(file, line, condition, formattedMessageBuffer);
        }
        else {
            return Report(file, line, condition, 0);
        }
    }
    
    Assert::ReportHandler Assert::GetReportHandler() {
        if (handler_.get() == 0) {
            handler_.reset(new ReportHandlerContainer(&DefaultReportHandler));
        }
        
        return handler_.get()->handler_;
        //return handler_;
    }
    
    void Assert::SetReportHandler(ReportHandler handler) {
        handler_.reset(new ReportHandlerContainer(handler));
        //handler_ = handler;
    }
}
