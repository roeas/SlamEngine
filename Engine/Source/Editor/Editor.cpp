#include "Editor.h"

#include "Core/Log.h"
#include "Core/Path.h"
#include "Event/WindowEvent.h"
#include "ImGui/ImGuiContext.h"
#include "Renderer/RenderCore.h"
#include "Scene/World.h"
#include "Window/Window.h"

#include "Layers/CameraControllerLayer.h"
#include "Layers/ImGuiLayer.h"
#include "Layers/RendererLayer.h"
#include "Layers/SandboxLayer.h"

Editor::Editor(const EditorInitor &initor)
{
    m_clock.Tick();
    sl::Log::Init();
    sl::RenderCore::Init(initor.m_backend);

    // Init window, graphics context and GUI
    sl::Window::Init();
    m_pMainWindow = std::make_unique<sl::Window>(initor.m_title, initor.m_width, initor.m_height);
    m_pMainWindow->SetEventCallback(SL_BIND_EVENT_CALLBACK(Editor::OnEvent));
    sl::ImGuiContext::Init(m_pMainWindow->GetNativeWindow(), m_pMainWindow->GetRenderContext());

    // Create main frame buffer, size is meaningless here
    sl::RenderCore::SetMainFramebuffer(sl::FrameBuffer::Create(
    {
        sl::Texture2D::Create(1, 1, sl::TextureFormat::RGB8, false, SL_SAMPLER_LINEAR),
        sl::Texture2D::Create(1, 1, sl::TextureFormat::D32, false, SL_SAMPLER_LINEAR),
    }));

    // Create camera uniform buffer
    sl::UniformBufferLayout cameraUniformBufferLayout;
    cameraUniformBufferLayout.AddElement("ub_cameraPos", sl::UniformBufferLayoutElement{ 0, sizeof(glm::vec4) });
    cameraUniformBufferLayout.AddElement("ub_viewProjection", sl::UniformBufferLayoutElement{ sizeof(glm::vec4), sizeof(glm::mat4) });
    cameraUniformBufferLayout.SetSize(sizeof(glm::vec4) + sizeof(glm::mat4));
    sl::RenderCore::SetUniformBuffer("CameraUniformBuffer", sl::UniformBuffer::Create(0, std::move(cameraUniformBufferLayout)));

    // Main camrea entity
    sl::World::CreateEntity("Main Camera").AddComponent<sl::CameraComponent>().m_isMainCamera = true;

    // Create and attach layers
    auto pCameraControllerLayer = std::make_unique<CameraControllerLayer>();
    auto pRendererLayer = std::make_unique<RendererLayer>();
    auto pImGuiLayer = std::make_unique<ImGuiLayer>();
    auto pSandBoxLayer = std::make_unique<SandboxLayer>();
    pImGuiLayer->SetEventCallback(SL_BIND_EVENT_CALLBACK(Editor::OnEvent));
    pImGuiLayer->SetMainWindow(m_pMainWindow->GetNativeWindow());
    m_layerStack.PushLayer(std::move(pCameraControllerLayer));
    m_layerStack.PushLayer(std::move(pRendererLayer));
    m_layerStack.PushLayer(std::move(pImGuiLayer));
    m_layerStack.PushLayer(std::move(pSandBoxLayer));
}

Editor::~Editor()
{
    sl::ImGuiContext::Shutdown();
    sl::Window::Quit();
}

void Editor::Run()
{
    while (m_isRunning)
    {
        BeginFrame();

        if (!m_isMinimized)
        {
            Update();
            Render();
        }

        EndFrame();
    }
}

void Editor::BeginFrame()
{
    m_clock.Tick();

    m_pMainWindow->BeginFrame();
    m_layerStack.BeginFrame();
}

void Editor::Update()
{
    m_layerStack.OnUpdate(m_clock.GetDeltatIme());
}

void Editor::Render()
{
    m_layerStack.OnRender();
}

void Editor::EndFrame()
{
    m_layerStack.EndFrame();
    m_pMainWindow->EndFrame();
}

void Editor::OnEvent(sl::Event &event)
{
    sl::EventDispatcher dispatcher{ event };
    dispatcher.Dispatch<sl::WindowCloseEvent>(SL_BIND_EVENT_CALLBACK(Editor::OnWindowClose));
    dispatcher.Dispatch<sl::WindowMinimizeEvent>(SL_BIND_EVENT_CALLBACK(Editor::OnWindowMinimize));
    dispatcher.Dispatch<sl::WindowRestoreEvent>(SL_BIND_EVENT_CALLBACK(Editor::OnWindowRestore));

    m_layerStack.OnEvent(event);
}

bool Editor::OnWindowClose(sl::WindowCloseEvent &event)
{
    SL_LOG_TRACE("Window close");
    m_isRunning = false;
    return true;
}

bool Editor::OnWindowMinimize(sl::WindowMinimizeEvent &event)
{
    m_isMinimized = true;
    return true;
}

bool Editor::OnWindowRestore(sl::WindowRestoreEvent &event)
{
    m_isMinimized = false;
    return true;
}
