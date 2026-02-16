#define STB_IMAGE_IMPLEMENTATION
#include <windows.h>
#include "Image.h"
#include <GL/gl.h>
#include <GL/glu.h>

bool CreateTexture(unsigned int &Texture, LPSTR Filename)
{
	Image* Img = LoadJPG(Filename);

	if (!Img)
		return false;

	glGenTextures(1, &Texture);
	glBindTexture(GL_TEXTURE_2D, Texture);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Select the texture type based on how many channels the image has
	int TType = GL_RGB;
	if (Img->Channels == 4)
		TType = GL_RGBA;

	gluBuild2DMipmaps(GL_TEXTURE_2D, Img->Channels, Img->SizeX,
		Img->SizeY, TType, GL_UNSIGNED_BYTE, Img->Data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	if (Img)
	{
		if (Img->Data)
			stbi_image_free(Img->Data);
		free(Img);
	}

	return true;
}

Image* LoadJPG(const char* Filename)
{
	Image* Img = (Image*)malloc(sizeof(Image));
	if (!Img)
		return nullptr;

	// Loads
	Img->Data = stbi_load(Filename, &Img->SizeX, &Img->SizeY, &Img->Channels, 0);
	if (!Img->Data)
	{
		free(Img);
		return NULL;
	}
	return Img;
}