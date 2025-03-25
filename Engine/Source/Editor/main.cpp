#include <iostream>

#include "Editor.h"

int main()
{
    Editor *pEditor = new Editor({ "SlamEngine", 1280, 720 });
    pEditor->Run();
    delete pEditor;
}
