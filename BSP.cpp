#include "BSP.h"
#include "Image.h"
#include <stdio.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "OpenGL.h"

void ChangeGamma(byte* Image, int Size, float Factor)
{
	for (int i = 0; i < Size / 3; i++, Image += 3)
	{
		float scale = 1.0f, temp = 0.0f;
		float r = (float)Image[0];
		float g = (float)Image[1];
		float b = (float)Image[2];

		r = r * Factor / 255.0f;
		g = g * Factor / 255.0f;
		b = b * Factor / 255.0f;

		if (r > 1.0f && (temp = (1.0f / r)) < scale) scale = temp;
		if (g > 1.0f && (temp = (1.0f / g)) < scale) scale = temp;
		if (b > 1.0f && (temp = (1.0f / b)) < scale) scale = temp;

		scale *= 255.0f;
		r *= scale; g *= scale; b *= scale;

		Image[0] = (byte)r;
		Image[1] = (byte)g;
		Image[2] = (byte)b;
	}
}

CBSPMap::CBSPMap()
{
	NumOfFaces = 0;
	NumOfIndices = 0;
	NumOfTextures = 0;
	NumOfVerts = 0;
	NumOfLightmaps = 0;

	memset(Textures, 0, sizeof(Textures));
	memset(LightMaps, 0, sizeof(LightMaps));

	Verts = NULL;
	Faces = NULL;
	Indices = NULL;
}

void CBSPMap::FindTextureExtension(char* Filename)
{
	char JPGPath[260] = { 0 };
	FILE* File = NULL;

	GetCurrentDirectoryA(260, JPGPath);

	strcat_s(JPGPath, sizeof(JPGPath), "\\");
	strcat_s(JPGPath, sizeof(JPGPath), Filename);

	strcat_s(JPGPath, sizeof(JPGPath), ".jpg");

	fopen_s(&File, JPGPath, "rb");
	if (File)
	{
		strcat_s(Filename, 64, ".jpg");
	}
}

Result CBSPMap::LoadBSP(const char* Filename)
{
	FILE* File = NULL;
	int i = 0;

	fopen_s(&File, Filename, "rb");
	if (!File)
	{
		return BSP_FILE_INVALID;
	}

	BSPHeader Header = { 0 };
	BSPLump Lumps[Lumps::MaxLumps] = { 0 };

	fread(&Header, sizeof(BSPHeader), 1, File);
	fread(&Lumps, sizeof(BSPLump), Lumps::MaxLumps, File);

	if (Header.version != 0x2e)
	{
		return BSP_HEADER_INVALID;
	}

	NumOfVerts = Lumps[Lumps::Vertices].length / sizeof(BSPVertex);
	Verts = new BSPVertex[NumOfVerts];

	NumOfFaces = Lumps[Lumps::Faces].length / sizeof(BSPFace);
	Faces = new BSPFace[NumOfFaces];

	NumOfIndices = Lumps[Lumps::Indices].length / sizeof(int);
	Indices = new int[NumOfIndices];

	NumOfTextures = Lumps[Lumps::Textures].length / sizeof(BSPTexture);
	BSPTexture* BSPTextures = new BSPTexture[NumOfTextures];

	fseek(File, Lumps[Lumps::Vertices].offset, SEEK_SET);

	for (i = 0; i < NumOfVerts; i++)
	{
		fread(&Verts[i], 1, sizeof(BSPVertex), File);

		float Temp = Verts[i].Position.y;
		Verts[i].Position.y = Verts[i].Position.z;
		Verts[i].Position.z = -Temp;
	}

	if (!Verts || NumOfVerts == NULL)
	{
		return BSP_VERTICES_INVALID;
	}

	fseek(File, Lumps[Lumps::Indices].offset, SEEK_SET);

	fread(Indices, sizeof(int), NumOfIndices, File);

	if (!Indices || NumOfIndices == NULL)
	{
		return BSP_INDICES_INVALID;
	}

	fseek(File, Lumps[Lumps::Faces].offset, SEEK_SET);

	fread(Faces, sizeof(BSPFace), NumOfFaces, File);

	if (!Faces || NumOfFaces == NULL)
	{
		return BSP_FACES_INVALID;
	}

#if 0
	// Allocate and read Lightmaps
	NumOfLightmaps = Lumps[Lumps::Lightmaps].length / sizeof(BSPLightmap);
	BSPLightmap* BSPLightmaps = new BSPLightmap[NumOfLightmaps];

	fseek(File, Lumps[Lumps::Lightmaps].offset, SEEK_SET);

	for (i = 0; i < NumOfLightmaps; i++)
	{
		fread(&BSPLightmaps[i], 1, sizeof(BSPLightmap), File);
	}

	if (!BSPLightmaps || NumOfLightmaps == NULL)
	{
		return BSP_LIGHTMAPS_INVALID;
	}

	for (i = 0; i < NumOfLightmaps; i++)
	{
		ChangeGamma((byte*)BSPLightmaps[i].ImageBits, 128 * 128 * 3, 10.0f);

		// Create OpenGL texture
		glGenTextures(1, &LightMaps[i]);
		glBindTexture(GL_TEXTURE_2D, LightMaps[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 128, 128, 0, GL_RGB, GL_UNSIGNED_BYTE, BSPLightmaps[i].ImageBits);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	delete[] BSPLightmaps;

#endif

	fseek(File, Lumps[Lumps::Textures].offset, SEEK_SET);

	fread(BSPTextures, sizeof(BSPTexture), NumOfTextures, File);

	if (!BSPTextures || NumOfTextures == NULL)
	{
		return BSP_TEXTURES_INVALID;
	}

	for(i = 0; i < NumOfTextures; i++)
	{
		FindTextureExtension(BSPTextures[i].Name);
		
		CreateTexture(Textures[i], BSPTextures[i].Name);
	}

	delete[] BSPTextures;

	fclose(File);

	FacesDrawn.Resize(NumOfFaces);

	return ALL_OK;
}

void CBSPMap::RenderFace(int FaceI, CEngineDevice* Dev)
{
	BSPFace* Face = &Faces[FaceI];

	Dev->Renderer->BindTexture(Textures[Face->TexID]);

	Dev->Renderer->SetTexturePointer(&(Verts[Face->StartVertexIndex].TextureCoord), sizeof(BSPVertex));

	Dev->Renderer->DrawIndexedPrimitive(
		&(Verts[Face->StartVertexIndex].Position), // Vertex data
		Face->NumOfVerts, // Pretty self explanatory
		sizeof(BSPVertex), // Stride
		&(Indices[Face->StartIndex]), // Index data
		Face->NumOfIndices // Pretty self explanatory
	);

	Dev->Renderer->DisableTexCoord();

#if 0
	// Draw lightmap on top
	if (Face->LightMapID >= 0 && Face->LightMapID < NumOfLightmaps)
	{
		// Enable blending
		glEnable(GL_BLEND);
		glBlendFunc(GL_DST_COLOR, GL_ZERO); 
		glDepthFunc(GL_EQUAL); 

		glBindTexture(GL_TEXTURE_2D, LightMaps[Face->LightMapID]);
		glTexCoordPointer(2, GL_FLOAT, sizeof(BSPVertex), &(Verts[Face->StartVertexIndex].LightMapCoord));

		// Draw lightmap over the texture
		glDrawElements(GL_TRIANGLES, Face->NumOfIndices, GL_UNSIGNED_INT, &(Indices[Face->StartIndex]));

		glDepthFunc(GL_LESS);
		glDisable(GL_BLEND);
	}
#endif

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void CBSPMap::RenderLevel(CEngineDevice* Dev)
{	
	// Clear the number of faces drawn
	FacesDrawn.ClearAll();

	for(int i = NumOfFaces - 1; i >= 0; i--)
	{
		if (Faces[i].Type != FACE_POLYGON) continue;

		if (!FacesDrawn.On(i))
		{
			FacesDrawn.Set(i);
			RenderFace(i, Dev);
		}
	}
}

void CBSPMap::Destroy()
{
	if (Verts)
	{
		delete[] Verts;
		Verts = NULL;
	}
	if (Faces)
	{
		delete[] Faces;
		Faces = NULL;
	}
	if (Indices)
	{
		delete[] Indices;
		Indices = NULL;
	}
	glDeleteTextures(NumOfTextures, Textures);
	glDeleteTextures(NumOfLightmaps, LightMaps);
}

CBSPMap::~CBSPMap()
{
	Destroy();
}