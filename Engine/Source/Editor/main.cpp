#include <iostream>

#include "Editor.h"

int main()
{
    sl::Editor *pEditor = new sl::Editor({ "SlamEngine", 1280, 720 });
    pEditor->Run();
    delete pEditor;
}
