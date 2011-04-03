//
// RS Game Framework
// Copyright © 2009 Jedd Haberstro
// jhaberstro@gmail.com
// 
// $Id:
//

#include <joh/joh.hpp>
#include <joh/graphics/GraphicsForwards.hpp>
#include <joh/application/cocoa/CocoaApplication.hpp>
#include <joh/application/cocoa/CocoaWindow.hpp>
#include <joh/application/cocoa/CocoaGraphicsContext.hpp>

using namespace joh;
using namespace joh::math;
using namespace joh::graphics;
using namespace joh::application;

class TestApp : public CocoaApplication
{
public:
    
    void OnStart() {
        window = new CocoaWindow;
        window->Create(
            "Cocoa Window",
            Vector2< int32_t >(0, 0),
            Vector2< int32_t >(800, 600),
            WindowStyle::Normal | WindowStyle::FullScreen | WindowStyle::Minimize
        );
        
        context = (new CocoaGraphicsContext(window));
        window->SetGraphicsContext(context);
    }
    
    void OnClose() {
        delete context;
        delete window;
    }
    
    void Update() {
        window->Update();
        context->Begin();
        glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        context->End();
    }
    
private:
    
    CocoaWindow* window;
    GraphicsContext* context;
};

int main() {
    TestApp app;
    app.Launch();
}
