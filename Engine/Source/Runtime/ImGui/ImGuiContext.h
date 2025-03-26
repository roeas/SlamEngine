#pragma once

struct ImFont;

namespace sl
{

class ImGuiContext final
{
public:
    static void Init(void *pNativeWindow, void *pRenderContext);
    static void Shutdown();

    static void NewFrame();
    static void Submit();
    static void OnEvent(void *pSDLEvent);

    static bool WantCaptureMouse();
    static bool WantCaptureKeyboard();

    static void SetUsingMouse(bool enable);

    static ImFont *GetRegularFont() { return m_pRegularFont; }
    static ImFont *GetBoldFont() { return m_pBoldFont; }
    static ImFont *GetLightFont() { return m_pLightFont; }

public:
    ImGuiContext() = delete;
    ImGuiContext(const ImGuiContext &) = delete;
    ImGuiContext &operator=(const ImGuiContext &) = delete;
    ImGuiContext(ImGuiContext &&) = delete;
    ImGuiContext &operator=(ImGuiContext &&) = delete;
    ~ImGuiContext() = delete;

private:
    static void SetColor();
    static void SetStyle();

    // ImGui holds the ownership of these datas
    inline static ImFont *m_pRegularFont;
    inline static ImFont *m_pBoldFont;
    inline static ImFont *m_pLightFont;
};

} // namespace sl
