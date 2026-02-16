#pragma once
#include "Renderer.h"
#include <gl/GLU.h>
#include <gl/GL.h>

class COpenGL : public CRenderer
{
public:
	COpenGL() = default;
	~COpenGL() = default;
	Result OpenRenderer(HWND Window, bool Cull) override;
	void CloseRenderer() override;

	void DrawIndexedPrimitive(void* Vertices, unsigned int VertexCount, unsigned int VertexStride, void* Indices, unsigned int IndexCount) override;

	// OTHER SHIT

	void SetTexturePointer(void* TextureCoords, unsigned int Stride);
	void DisableTexCoord();

	// OTHER SHIT 

	void SetMatrixMode(MatrixMode Mode) override;

	void Clear(float R, float G, float B, float A) override;

	void Viewport(int Width, int Height) override;

	void Present() override;

	void Perspective(float FOV, int Width, int Height, float Near, float Far);

	void LookAt(float EyeX, float EyeY, float EyeZ, float CenterX, float CenterY, float CenterZ, float UpX, float UpY, float UpZ) override;

	void BindTexture(unsigned int TextureHandle) override;
private:
	HDC hDC;
	HGLRC hGL;
};