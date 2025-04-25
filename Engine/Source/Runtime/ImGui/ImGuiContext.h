#pragma once

struct ImFont;

namespace sl
{

class ImGuiContext final
{
public:
    ImGuiContext() = delete;
    ImGuiContext(const ImGuiContext &) = delete;
    ImGuiContext &operator=(const ImGuiContext &) = delete;
    ImGuiContext(ImGuiContext &&) = delete;
    ImGuiContext &operator=(ImGuiContext &&) = delete;
    ~ImGuiContext() = delete;

    static void Init(void *pNativeWindow, void *pRenderContext);
    static void Shutdown();

    static void NewFrame();
    static void Submit();
    static void OnEvent(void *pSDLEvent);

    static bool WantCaptureMouse();
    static bool WantCaptureKeyboard();
    static void SetMouseUsing(bool enable);

    static ImFont *GetBoldFont() { return pBoldFont; }
    static ImFont *GetLightFont() { return pLightFont; }
    static ImFont *GetRegularFont() { return pRegularFont; }

private:
    static void SetStyle();
    static void SetColor();

    // ImGui holds the ownership of these datas, no need to delete them
    inline static ImFont *pBoldFont;
    inline static ImFont *pLightFont;
    inline static ImFont *pRegularFont;
};

} // namespace sl
