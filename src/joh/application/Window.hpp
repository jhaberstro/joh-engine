#ifndef JOH_APPLICATION_WINDOW_HPP
#define JOH_APPLICATION_WINDOW_HPP

#include "joh/math/Vector2.hpp"
#include "joh/graphics/GraphicsContext.hpp"
#include "joh/graphics/Renderer.hpp"
#include "joh/joh.hpp"
#include <string>

namespace joh
{
    namespace application
    {
        namespace WindowStyle
        {
            enum Enum
            {
                Minimize = 1 << 0,
                Normal = 1 << 1,
                FullScreen = 1 << 2
            };
        }

        class Window
        {            
        public:

            virtual ~Window() { };

            virtual std::string GetTitle() const = 0;

            virtual joh::math::Vector2< int32_t > GetPosition() const = 0;

            virtual joh::math::Vector2< int32_t > GetSize() const = 0;

            virtual WindowStyle::Enum GetStyle() const = 0;

            virtual void Create(
                std::string const& name,
                joh::math::Vector2< int32_t > const& position,
                joh::math::Vector2< int32_t > const& size,
                uint32_t style
            ) = 0;

            virtual void Close() = 0;

            virtual void MakeFocus() = 0;

            virtual void SetTitle(std::string const& title) = 0;

            virtual void SetPosition(joh::math::Vector2< int32_t > const& position) = 0;

            virtual void SetSize(joh::math::Vector2< int32_t > const& size) = 0;

            virtual void SetStyle(WindowStyle::Enum style) = 0;

            virtual void ShowCursor(bool show) = 0;

            virtual void SyncToVBL(bool sync) = 0;

            virtual void Update() = 0;

            joh::graphics::Renderer* GetRenderer() const;

            void SetRenderer(joh::graphics::Renderer* renderer);
            
            joh::graphics::GraphicsContext* GetGraphicsContext() const;
            
            void SetGraphicsContext(joh::graphics::GraphicsContext* context);

        protected:

            joh::graphics::Renderer* renderer_;
            joh::graphics::GraphicsContext* graphicsContext_;
        };

        
        inline joh::graphics::Renderer* Window::GetRenderer() const {
            return renderer_;
        }

        inline void Window::SetRenderer(joh::graphics::Renderer* renderer) {
            renderer_ = renderer;
        }
        
        inline joh::graphics::GraphicsContext* Window::GetGraphicsContext() const {
            return graphicsContext_;
        }
        
        inline void Window::SetGraphicsContext(joh::graphics::GraphicsContext* context) {
            graphicsContext_ = context;
        }
    }
}

#endif // JOH_APPLICATION_WINDOW_HPP
