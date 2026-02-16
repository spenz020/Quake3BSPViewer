#include "OpenGL.h"

Result COpenGL::OpenRenderer(HWND Window, bool Cull)
{
    hDC = GetDC(Window);
    hGL = NULL;
    int PixelFormat;

    PIXELFORMATDESCRIPTOR PixelFormatDesc;

    PixelFormatDesc.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    PixelFormatDesc.nVersion = 1;

    PixelFormatDesc.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    PixelFormatDesc.dwLayerMask = PFD_MAIN_PLANE;
    PixelFormatDesc.iPixelType = PFD_TYPE_RGBA;
    PixelFormatDesc.cColorBits = 32;
    PixelFormatDesc.cDepthBits = 32;
    PixelFormatDesc.cAccumBits = 0;
    PixelFormatDesc.cStencilBits = 0;

    PixelFormat = ChoosePixelFormat(hDC, &PixelFormatDesc);
    if (!PixelFormat)
        return OPENGL_CPF_FAILED;

    if (!SetPixelFormat(hDC, PixelFormat, &PixelFormatDesc))
        return OPENGL_SPF_FAILED;

    hGL = wglCreateContext(hDC);
    wglMakeCurrent(hDC, hGL);
    if (!hGL)
        return OPENGL_MAKE_FAILED;

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_TEXTURE_2D);
    if (Cull)
    {
        glCullFace(GL_FRONT);
        glEnable(GL_CULL_FACE);
    }

    return ALL_OK;
}

void COpenGL::CloseRenderer()
{
    if (hDC)
    {
        if (hGL)
        {
            wglMakeCurrent(hDC, NULL);
            wglDeleteContext(hGL);
        }
        ReleaseDC(NULL, hDC);
    }
}

void COpenGL::DrawIndexedPrimitive(void* Vertices, unsigned int VertexCount, unsigned int VertexStride, void* Indices, unsigned int IndexCount)
{
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, VertexStride, Vertices);

    glDrawElements(GL_TRIANGLES, IndexCount, GL_UNSIGNED_INT, Indices);

    glDisableClientState(GL_VERTEX_ARRAY);
}

void COpenGL::SetTexturePointer(void* TextureCoords, unsigned int Stride)
{
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2, GL_FLOAT, Stride, TextureCoords);
}

void COpenGL::DisableTexCoord()
{
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void COpenGL::SetMatrixMode(MatrixMode Mode)
{
    switch (Mode)
    {
    case MatrixMode::MODELVIEW:
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        break;
    case MatrixMode::PROJECTION:
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        break;
    }
}

void COpenGL::Viewport(int Width, int Height)
{
    glViewport(0, 0, Width, Height);
}

void COpenGL::Clear(float R, float G, float B, float A)
{
    glClearColor(R, G, B, A);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void COpenGL::Present()
{
    SwapBuffers(hDC);
}

void COpenGL::Perspective(float FOV, int Width, int Height, float Near, float Far)
{
    gluPerspective(FOV, (float)Width / (float)Height, Near, Far);
}

void COpenGL::LookAt(float EyeX, float EyeY, float EyeZ, float CenterX, float CenterY, float CenterZ, float UpX, float UpY, float UpZ)
{
    gluLookAt(EyeX, EyeY, EyeZ, CenterX, CenterY, CenterZ, UpX, UpY, UpZ);
}

void COpenGL::BindTexture(unsigned int TextureID)
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, TextureID);
}