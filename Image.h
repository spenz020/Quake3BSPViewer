#pragma once
#include "stb_image.h"

#define TGA_RGB		 2		
#define TGA_A		 3		
#define TGA_RLE		10	

struct Image
{
	int Channels;		
	int SizeX;				
	int SizeY;				
	unsigned char* Data;	
};

bool CreateTexture(unsigned int& Texture, LPSTR Filename);

Image* LoadJPG(const char* Filename);