#include "Editor.h"

int main()
{
    Editor *pEditor = new Editor({ "SlamEngine", 1280, 720, sl::GraphicsBackend::OpenGL });
    pEditor->Run();
    delete pEditor;
}
