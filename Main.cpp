#include "Engine.h"
#include "Camera.h"
#include "BSP.h"
#include <iostream>

CCamera MainCamera;
CEngineDevice MainEngine;
CBSPMap Map;

float MoveSpeed = 1.0f;
float MouseSensitivity = 0.1f;

int ScreenCenterX = 400;
int ScreenCenterY = 300;

LRESULT CALLBACK WndProc(HWND Win, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    switch (iMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
    }
	return DefWindowProcW(Win, iMsg, wParam, lParam);
}

bool KeyDown(int Key)
{
    return (GetAsyncKeyState(Key) & 0x8000) != 0;
}

void UpdateCamera()
{
    POINT mousePos;
    GetCursorPos(&mousePos);

    float deltaX = (float)(mousePos.x - ScreenCenterX);
    float deltaY = (float)(mousePos.y - ScreenCenterY);

    MainCamera.Rotate({ -deltaY * MouseSensitivity, deltaX * MouseSensitivity, 0.0f });

    SetCursorPos(ScreenCenterX, ScreenCenterY);

    Vec3 MoveAmount = { 0.0f, 0.0f, 0.0f };

    if (KeyDown('W')) MoveAmount.z += MoveSpeed;
    if (KeyDown('S')) MoveAmount.z -= MoveSpeed;
    if (KeyDown('A')) MoveAmount.x -= MoveSpeed;
    if (KeyDown('D')) MoveAmount.x += MoveSpeed;
    if (KeyDown(VK_SPACE)) MoveAmount.y += MoveSpeed;   // move up
    if (KeyDown(VK_SHIFT)) MoveAmount.y -= MoveSpeed;   // move down

    MainCamera.Translate(MoveAmount);

    MainCamera.Update(&MainEngine);
}

void Frame()
{
    MainEngine.ClearScreen(0.0f, 0.0f, 0.0f, 1.0f);

    UpdateCamera();

    Map.RenderLevel(&MainEngine);

    MainEngine.Present();
}

void Idle()
{
    // Other idle stuff here
    Frame();
}

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
    WNDCLASSW wc = { 0 }; 
    HWND Window = NULL;
    MSG Msg = { 0 };
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = L"WINDOWCLASS";
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursorA(NULL, IDC_ARROW);
	RegisterClassW(&wc);

	Window = CreateWindowW(L"WINDOWCLASS", L"OpenGL", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, NULL, NULL, hInstance, NULL);
	if (!Window)
		return false;
	ShowWindow(Window, nShowCmd);

	ShowCursor(FALSE);

    MainEngine.Open(Window);
    MainEngine.SetupProjection(70.0f, 800, 600, 0.1f, 1000.0f);

    MainCamera.Translate(Vec3(0.0f, 400.0f, 0.0f));

    Map.LoadBSP("Maps\\Level.bsp");

	while (Msg.message != WM_QUIT)
	{
		if (PeekMessage(&Msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&Msg);
			DispatchMessageW(&Msg);
		}
        Idle();
	}
    return (int)Msg.wParam;
}