#include "Engine.h"

bool CEngineDevice::Open(HWND Window)
{
    Renderer = new COpenGL;
    CurrentWindow = Window;
    if(!Renderer->OpenRenderer(Window, true))
        return false;
    return true;
}

void CEngineDevice::Close()
{
    Renderer->CloseRenderer();
}

void CEngineDevice::Present()
{
    Renderer->Present();
}

void CEngineDevice::ClearScreen(float R, float G, float B, float A)
{
    Renderer->Clear(R, G, B, A);
}

void CEngineDevice::SetupProjection(float FOV, int Width, int Height, float Near, float Far)
{
    Renderer->Viewport(Width, Height);

    Renderer->SetMatrixMode(MatrixMode::PROJECTION);

    Renderer->Perspective(FOV, Width, Height, Near, Far);

    Renderer->SetMatrixMode(MatrixMode::MODELVIEW);
}