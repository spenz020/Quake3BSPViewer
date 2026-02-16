#pragma once
#include "OpenGL.h"

class CEngineDevice // Kind of abstracts over renderer
{
public:
    CEngineDevice() = default;
    ~CEngineDevice() = default;

    bool Open(HWND Window);
    void Close();

    void SetupProjection(float FOV, int Width, int Height, float Near, float Far);

    void ClearScreen(float R, float G, float B, float A);

    void Present();
    COpenGL* Renderer;
    HWND CurrentWindow;
};