#pragma once
#include <Windows.h>
#include "Types.h"

enum class MatrixMode
{
	PROJECTION,
	MODELVIEW
};

class CRenderer
{
public:
	virtual Result OpenRenderer(HWND Window, bool Cull) = 0;
	virtual void CloseRenderer() = 0;

	virtual void SetMatrixMode(MatrixMode Mode) = 0;

	virtual void Viewport(int Width, int Height) = 0;

	virtual void Perspective(float FOV, int Width, int Height, float Near, float Far) = 0;

	virtual void Clear(float R, float G, float B, float A) = 0;

	virtual void Present() = 0;

	virtual void LookAt(float EyeX, float EyeY, float EyeZ, float CenterX, float CenterY, float CenterZ, float UpX, float UpY, float UpZ) = 0;

	virtual void DrawIndexedPrimitive(void* Vertices, unsigned int VertexCount, unsigned int VertexStride, void* Indices, unsigned int IndexCount) = 0; // For drawing 

	virtual void BindTexture(unsigned int TextureHandle) = 0;
};