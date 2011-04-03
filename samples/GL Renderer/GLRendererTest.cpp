//
// RS Game Framework
// Copyright © 2009 Jedd Haberstro
// jhaberstro@gmail.com
// 
// $Id:
//

#include <rs/Portability.hpp>
#include <rs/application/cocoa/CocoaApplication.hpp>
#include <rs/application/cocoa/CocoaWindow.hpp>
#include <rs/application/cocoa/CocoaGraphicsContext.hpp>
#include <rs/application/Input.hpp>
#include <rs/graphics/GraphicsForwards.hpp>
#include <rs/graphics/Shader.hpp>
#include <rs/graphics/opengl/GLRenderer.hpp>


using namespace rs;
using namespace rs::math;
using namespace rs::graphics;
using namespace rs::application;

class TestApp : public CocoaApplication
{
public:
    
    void OnStart() {
        // Create window, graphics context, and renderer
        window = new CocoaWindow;
        window->Create(
            "Cocoa Window",
            Vector2< rs::Int32 >(0, 0),
            Vector2< rs::Int32 >(800, 600),
            WindowStyle::Normal | WindowStyle::FullScreen | WindowStyle::Minimize
        );

        context = GraphicsContextPtr(new CocoaGraphicsContext(window));
        window->SetGraphicsContext(context);
        renderer = RendererPtr(new GLRenderer);
        
        // Create vertex buffer
        enum { kVertexCount = 3 };
        Vector2f data[kVertexCount] = {
            Vector2f(-100.0f, -100.0f),
            Vector2f( 0.0f,    100.0f),
            Vector2f( 100.0f, -100.0f)
        };
        
        BufferDescription vboDescription(
            kVertexCount,
            sizeof(Vector2f),
            ResourceType::VertexBuffer,
            ResourceAccess::Read,
            ResourceUsage::Static
        );
        
        vertexBuffer = renderer->CreateBuffer(vboDescription, static_cast< void const* >(&data[0]));
        
        // Create vertex format
        InputElement inputElements = { "Position", 0, InputElementFormat::Float32, 2, 0, 0 };
        vertexFormat = renderer->CreateVertexFormat(&inputElements, 1);
        
        // Create shader
        Char const* attribute = "Position";
        ShaderDescription shaderDescription(
            "/Users/jhaberstro/Personal-Projects/rs-game-framework/src/trunk/samples/GL Renderer/vertex.vs",
            "/Users/jhaberstro/Personal-Projects/rs-game-framework/src/trunk/samples/GL Renderer/fragment.fs",
            &attribute,
            1
        );
        shader = renderer->CreateShader(shaderDescription);
    }
    
    void OnClose() {
        delete window;
    }
    
    void Update() {
        window->Update();
		
        Vector2< Int32 > windowSize = window->GetSize();
        renderer->set_primitive_topology(PrimitiveTopology::Triangles);
        renderer->SetVertexBuffer(vertexBuffer);
        renderer->SetVertexFormat(vertexFormat, shader);
        renderer->SetShader(shader);
        
        Matrix4x4f projection = Matrix4x4f::OrthographicProjection(windowSize.x, windowSize.y, -1.0f, 1.0f);
        shader->SetConstant("MVP", projection); // view and model are both the identity matrix
        shader->SetConstant("Diffuse", Vector4f(1.0f, 0.0f, 0.0f, 1.0f));
        
        context->Begin();
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glViewport(0, 0, windowSize.x, windowSize.y);
        
        renderer->Apply();
        renderer->Draw(0, 3);
        context->End();
    }
    
private:
    
    CocoaWindow* window;
    GraphicsContextPtr context;
    RendererPtr renderer;
    
    BufferPtr vertexBuffer;
    ShaderPtr shader;
    VertexFormatID vertexFormat;
};

int main() {
    TestApp app;
    app.Launch();
}