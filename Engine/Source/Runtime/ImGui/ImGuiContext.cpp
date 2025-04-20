#include "ImGuiContext.h"

#include "Core/Log.h"
#include "Core/Path.h"
#include "ImGui/IconsMaterialSymbols.h"
#include "ImGui/imgui_impl_opengl3.h"
#include "ImGui/imgui_impl_sdl3.h"

#include <imgui/imgui.h>
#include <imguizmo/ImGuizmo.h>
#include <implot/implot.h>

namespace sl
{

void ImGuiContext::Init(void *pNativeWindow, void *pRenderContext)
{
    SL_LOG_INFO("Initializing ImGui");

    // 1. Init ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();
    ImGuiIO &io = ImGui::GetIO();

    // 2. Setup configs
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    io.ConfigViewportsNoTaskBarIcon = true;
    io.ConfigDebugIsDebuggerPresent = true;

    static std::string s_iniFilePath = Path::FromeRoot("Engine/Config/imgui.ini");
    io.IniFilename = s_iniFilePath.data();

    // 3. Load font
    constexpr float FontSize = 18.0f;
    std::string regularFontPath = sl::Path::FromeAsset("Font/Noto_Sans/static/NotoSans-Regular.ttf");
    std::string boldFontPath = sl::Path::FromeAsset("Font/Noto_Sans/static/NotoSans-Bold.ttf");
    std::string lightFontPath = sl::Path::FromeAsset("Font/Noto_Sans/static/NotoSans-Light.ttf");
    std::string iconFontPath = sl::Path::FromeAsset("Font/MaterialSymbols/MaterialSymbolsOutlined.ttf");

    // TODO: AddFontFromMemoryTTF
    SL_LOG_TRACE("Loading blod font: {}", boldFontPath.data());
    m_pBoldFont = io.Fonts->AddFontFromFileTTF(boldFontPath.data(), FontSize);
    SL_LOG_TRACE("Loading light font: {}", lightFontPath.data());
    m_pLightFont = io.Fonts->AddFontFromFileTTF(lightFontPath.data(), FontSize);
    SL_LOG_TRACE("Loading regular font: {}", regularFontPath.data());
    m_pRegularFont = io.Fonts->AddFontFromFileTTF(regularFontPath.data(), FontSize);
    io.FontDefault = m_pRegularFont;

    // Merge icon font
    ImFontConfig fontConfig;
    fontConfig.GlyphOffset = ImVec2{ 0.0f, 2.0f };
    fontConfig.GlyphMinAdvanceX = FontSize;
    fontConfig.MergeMode = true;
    static const ImWchar s_iconRange[] = { (ImWchar)ICON_MIN_MS, (ImWchar)ICON_MAX_MS, 0 };
    SL_LOG_TRACE("Loading icon font: {}", iconFontPath.data());
    io.Fonts->AddFontFromFileTTF(iconFontPath.data(), FontSize, &fontConfig, s_iconRange);

    // 4. Set color and style
    // TODO: Apply new options
    SetStyle();
    SetColor();

    // 5. Setup Platform / Renderer backends
    ImGui_ImplSDL3_InitForOpenGL(static_cast<SDL_Window *>(pNativeWindow), pRenderContext);
    ImGui_ImplOpenGL3_Init("#version 460");
}

void ImGuiContext::Shutdown()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImPlot::DestroyContext();
    ImGui::DestroyContext();
}

void ImGuiContext::NewFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();
    ImGuizmo::BeginFrame();
}

void ImGuiContext::Submit()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();
}

void ImGuiContext::OnEvent(void *pSDLEvent)
{
    ImGui_ImplSDL3_ProcessEvent(static_cast<const SDL_Event *>(pSDLEvent));
}

bool ImGuiContext::WantCaptureMouse()
{
    return ImGui::GetIO().WantCaptureMouse;
}

bool ImGuiContext::WantCaptureKeyboard()
{
    return ImGui::GetIO().WantCaptureKeyboard;
}

void ImGuiContext::SetMouseUsing(bool enable)
{
    auto &io = ImGui::GetIO();
    if (enable)
    {
        io.ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
    }
    else
    {
        io.ConfigFlags |= ImGuiConfigFlags_NoMouse;
    }
}

void ImGuiContext::SetStyle()
{
    ImGuiStyle &style = ImGui::GetStyle();

    style.WindowRounding = 4.0f;
    style.ChildRounding = 4.0f;
    style.FrameRounding = 4.0f;
    style.PopupRounding = 4.0f;
    style.ScrollbarRounding = 4.0f;
    style.GrabRounding = 4.0f;
    style.TabRounding = 4.0f;

    style.WindowMenuButtonPosition = ImGuiDir_None;
    style.DockingSeparatorSize = 1.0f;
}

void ImGuiContext::SetColor()
{
    auto NormalizedColor = [](uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) consteval
    {
        return ImVec4{ r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f };
    };

    constexpr ImVec4 TransColor = NormalizedColor(0, 0, 0, 0);
    constexpr ImVec4 BlackColor = NormalizedColor(0, 0, 0);
    constexpr ImVec4 GreyColor = NormalizedColor(127, 127, 127);
    constexpr ImVec4 WhiteColor = NormalizedColor(255, 255, 255);
    constexpr ImVec4 MyColor = NormalizedColor(255, 20, 40);
    constexpr ImVec4 MyColorTrans = NormalizedColor(255, 20, 40, 127);
    constexpr ImVec4 UnknownColor = NormalizedColor(255, 0, 235);

    constexpr ImVec4 BackgroundColor = NormalizedColor(30, 30, 30);
    constexpr ImVec4 BackgroundColor1 = NormalizedColor(32, 31, 31);
    constexpr ImVec4 BackgroundColor2 = NormalizedColor(37, 37, 37);
    constexpr ImVec4 BackgroundColor3 = NormalizedColor(46, 46, 46);
    constexpr ImVec4 BackgroundColor4 = NormalizedColor(56, 56, 56);
    constexpr ImVec4 BackgroundColor5 = NormalizedColor(61, 61, 61);
    constexpr ImVec4 BackgroundColor6 = NormalizedColor(66, 66, 66);

    constexpr ImVec4 ItemColor1 = NormalizedColor(77, 77, 77);
    constexpr ImVec4 ItemColor2 = NormalizedColor(112, 112, 112);
    constexpr ImVec4 ItemColor3 = NormalizedColor(153, 153, 153);

    ImVec4 *colors = ImGui::GetStyle().Colors;

    colors[ImGuiCol_Text] = WhiteColor;
    colors[ImGuiCol_TextDisabled] = GreyColor;

    colors[ImGuiCol_WindowBg] = BackgroundColor;
    colors[ImGuiCol_ChildBg] = BackgroundColor;
    colors[ImGuiCol_PopupBg] = BackgroundColor2;

    colors[ImGuiCol_Border] = BlackColor;
    colors[ImGuiCol_BorderShadow] = TransColor;

    colors[ImGuiCol_FrameBg] = BackgroundColor3;
    colors[ImGuiCol_FrameBgHovered] = BackgroundColor5;
    colors[ImGuiCol_FrameBgActive] = BackgroundColor5;

    colors[ImGuiCol_TitleBg] = BackgroundColor1;
    colors[ImGuiCol_TitleBgActive] = BackgroundColor1;
    colors[ImGuiCol_TitleBgCollapsed] = UnknownColor;
    
    colors[ImGuiCol_MenuBarBg] = BackgroundColor;

    colors[ImGuiCol_ScrollbarBg] = BackgroundColor;
    colors[ImGuiCol_ScrollbarGrab] = ItemColor1;
    colors[ImGuiCol_ScrollbarGrabHovered] = ItemColor3;
    colors[ImGuiCol_ScrollbarGrabActive] = ItemColor3;
    
    colors[ImGuiCol_CheckMark] = WhiteColor;

    colors[ImGuiCol_SliderGrab] = ItemColor1;
    colors[ImGuiCol_SliderGrabActive] = ItemColor3;

    colors[ImGuiCol_Button] = BackgroundColor3;
    colors[ImGuiCol_ButtonHovered] = BackgroundColor5;
    colors[ImGuiCol_ButtonActive] = BackgroundColor6;

    colors[ImGuiCol_Header] = BackgroundColor3;
    colors[ImGuiCol_HeaderHovered] = BackgroundColor5;
    colors[ImGuiCol_HeaderActive] = BackgroundColor5;

    colors[ImGuiCol_Separator] = BlackColor;
    colors[ImGuiCol_SeparatorHovered] = ItemColor3;
    colors[ImGuiCol_SeparatorActive] = ItemColor3;

    colors[ImGuiCol_ResizeGrip] = BackgroundColor3;
    colors[ImGuiCol_ResizeGripHovered] = ItemColor3;
    colors[ImGuiCol_ResizeGripActive] = ItemColor3;

    colors[ImGuiCol_TabHovered] = MyColor;
    colors[ImGuiCol_Tab] = BackgroundColor3;
    colors[ImGuiCol_TabSelected] = MyColor;
    colors[ImGuiCol_TabSelectedOverline] = TransColor;
    colors[ImGuiCol_TabDimmed] = BackgroundColor3;
    colors[ImGuiCol_TabDimmedSelected] = ItemColor1;
    colors[ImGuiCol_TabDimmedSelectedOverline] = TransColor;

    colors[ImGuiCol_DockingPreview] = MyColorTrans;
    colors[ImGuiCol_DockingEmptyBg] = BackgroundColor;

    //colors[ImGuiCol_PlotLines] = ;
    //colors[ImGuiCol_PlotLinesHovered] = ;
    //colors[ImGuiCol_PlotHistogram] = ;
    //colors[ImGuiCol_PlotHistogramHovered] = ;
    
    //colors[ImGuiCol_TableHeaderBg] = ;
    //colors[ImGuiCol_TableBorderStrong] = ;
    //colors[ImGuiCol_TableBorderLight] = ;
    //colors[ImGuiCol_TableRowBg] = ;
    //colors[ImGuiCol_TableRowBgAlt] = ;

    colors[ImGuiCol_TextLink] = NormalizedColor(86, 156, 214);
    colors[ImGuiCol_TextSelectedBg] = NormalizedColor(0, 120, 215);

    colors[ImGuiCol_DragDropTarget] = MyColor;

    colors[ImGuiCol_NavCursor] = ItemColor3;
    //colors[ImGuiCol_NavWindowingHighlight] = ;
    //colors[ImGuiCol_NavWindowingDimBg] = ;
    //colors[ImGuiCol_ModalWindowDimBg] = ;
}

} // namespace sl
