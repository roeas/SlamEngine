#include "ImGuiContext.h"

#include "Core/Log.h"
#include "Core/Path.hpp"

#include <ImGui/IconsMaterialSymbols.h>
#include <imgui/imgui.h>
#include <ImGui/imgui_impl_opengl3.h>
#include <ImGui/imgui_impl_sdl3.h>

namespace sl
{

void ImGuiContext::Init(void *pNativeWindow, void *pRenderContext)
{
    // 1. Init ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
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
    m_pRegularFont = io.Fonts->AddFontFromFileTTF(
        sl::Path::FromeAsset("Font/Noto_Sans/static/NotoSans-Regular.ttf").data(), FontSize);
    io.FontDefault = m_pRegularFont;

    // Merge icon font
    ImFontConfig fontConfig;
    fontConfig.GlyphOffset = ImVec2{ 0.0f, 2.0f };
    fontConfig.GlyphMinAdvanceX = FontSize;
    fontConfig.MergeMode = true;
    static const ImWchar s_iconRange[] = { (ImWchar)ICON_MIN_MS, (ImWchar)ICON_MAX_MS, 0 };
    io.Fonts->AddFontFromFileTTF(
        sl::Path::FromeAsset("Font/GoogleMaterialSymbols/MaterialSymbolsOutlined[FILL,GRAD,opsz,wght].ttf").data(),
        FontSize, &fontConfig, s_iconRange);
    m_pBoldFont = io.Fonts->AddFontFromFileTTF(
        sl::Path::FromeAsset("Font/Noto_Sans/static/NotoSans-Bold.ttf").data(), FontSize);
    m_pLightFont = io.Fonts->AddFontFromFileTTF(
        sl::Path::FromeAsset("Font/Noto_Sans/static/NotoSans-Light.ttf").data(), FontSize);

    // 4. Set color and style
    SetColor();
    SetStyle();

    // 5. Setup Platform / Renderer backends
    ImGui_ImplSDL3_InitForOpenGL(static_cast<SDL_Window *>(pNativeWindow), pRenderContext);
    ImGui_ImplOpenGL3_Init("#version 460");
}

void ImGuiContext::Shutdown()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
}

void ImGuiContext::NewFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();
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

void ImGuiContext::SetUsingMouse(bool enable)
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

void ImGuiContext::SetColor()
{
    constexpr auto ColorFromByte = [](uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255)
    {
        return ImVec4{ (float)r / 255.0f, (float)g / 255.0f, (float)b / 255.0f, (float)a / 255.0f };
    };

    constexpr ImVec4 whiteColor = ColorFromByte(255, 255, 255);
    constexpr ImVec4 greyColor = ColorFromByte(127, 127, 127);
    constexpr ImVec4 blackColor = ColorFromByte(0, 0, 0);
    constexpr ImVec4 myColor = ColorFromByte(255, 50, 30);
    constexpr ImVec4 unknownColor = ColorFromByte(255, 0, 235);

    constexpr ImVec4 bgColor = ColorFromByte(30, 30, 30);
    constexpr ImVec4 bgColorLight = ColorFromByte(32, 31, 31);
    constexpr ImVec4 bgColorVeryLight = ColorFromByte(68, 66, 66);

    constexpr ImVec4 itemColor = ColorFromByte(46, 46, 46);
    constexpr ImVec4 itemColorLight = ColorFromByte(61, 61, 61);
    constexpr ImVec4 itemColorVeryLight = ColorFromByte(153, 153, 153);

    ImVec4 *colors = ImGui::GetStyle().Colors;

    colors[ImGuiCol_Text] = whiteColor;
    colors[ImGuiCol_TextDisabled] = greyColor;

    colors[ImGuiCol_WindowBg] = bgColor;
    colors[ImGuiCol_ChildBg] = bgColor;
    colors[ImGuiCol_PopupBg] = bgColorLight;

    colors[ImGuiCol_Border] = blackColor;
    colors[ImGuiCol_BorderShadow] = bgColorVeryLight;

    colors[ImGuiCol_FrameBg] = itemColor;
    colors[ImGuiCol_FrameBgHovered] = itemColorLight;
    colors[ImGuiCol_FrameBgActive] = itemColorLight;

    colors[ImGuiCol_TitleBg] = bgColorLight;
    colors[ImGuiCol_TitleBgActive] = bgColorLight;
    colors[ImGuiCol_TitleBgCollapsed] = bgColorLight;

    colors[ImGuiCol_MenuBarBg] = bgColor;

    colors[ImGuiCol_ScrollbarBg] = bgColorLight;
    colors[ImGuiCol_ScrollbarGrab] = itemColorLight;
    colors[ImGuiCol_ScrollbarGrabHovered] = itemColorVeryLight;
    colors[ImGuiCol_ScrollbarGrabActive] = itemColorVeryLight;

    colors[ImGuiCol_CheckMark] = itemColorVeryLight;

    colors[ImGuiCol_SliderGrab] = itemColorLight;
    colors[ImGuiCol_SliderGrabActive] = itemColorVeryLight;

    colors[ImGuiCol_Button] = itemColor;
    colors[ImGuiCol_ButtonHovered] = itemColorLight;
    colors[ImGuiCol_ButtonActive] = itemColorLight;

    colors[ImGuiCol_Header] = itemColor;
    colors[ImGuiCol_HeaderHovered] = itemColorLight;
    colors[ImGuiCol_HeaderActive] = itemColorLight;

    colors[ImGuiCol_Separator] = blackColor;
    colors[ImGuiCol_SeparatorHovered] = itemColorVeryLight;
    colors[ImGuiCol_SeparatorActive] = itemColorVeryLight;

    colors[ImGuiCol_ResizeGrip] = itemColorLight;
    colors[ImGuiCol_ResizeGripHovered] = itemColorVeryLight;
    colors[ImGuiCol_ResizeGripActive] = itemColorVeryLight;

    colors[ImGuiCol_Tab] = itemColor;
    colors[ImGuiCol_TabHovered] = itemColorVeryLight;
    colors[ImGuiCol_TabActive] = myColor;
    colors[ImGuiCol_TabUnfocused] = itemColor;
    colors[ImGuiCol_TabUnfocusedActive] = itemColorLight;

    colors[ImGuiCol_DockingPreview] = bgColorVeryLight;
    colors[ImGuiCol_DockingEmptyBg] = bgColorLight;

    // colors[ImGuiCol_PlotLines] = unknownColor;
    // colors[ImGuiCol_PlotLinesHovered] = unknownColor;
    // colors[ImGuiCol_PlotHistogram] = unknownColor;
    // colors[ImGuiCol_PlotHistogramHovered] = unknownColor;
    // 
    // colors[ImGuiCol_TableHeaderBg] = unknownColor;
    // colors[ImGuiCol_TableBorderStrong] = unknownColor;
    // colors[ImGuiCol_TableBorderLight] = unknownColor;
    // colors[ImGuiCol_TableRowBg] = unknownColor;
    // colors[ImGuiCol_TableRowBgAlt] = unknownColor;
    // 
    // colors[ImGuiCol_TextSelectedBg] = unknownColor;
    // colors[ImGuiCol_DragDropTarget] = unknownColor;
    // 
    // colors[ImGuiCol_NavHighlight] = unknownColor;
    // colors[ImGuiCol_NavWindowingHighlight] = unknownColor;
    // colors[ImGuiCol_NavWindowingDimBg] = unknownColor;
    // 
    // colors[ImGuiCol_ModalWindowDimBg] = unknownColor;
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

} // namespace sl
