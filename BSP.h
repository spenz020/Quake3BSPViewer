#pragma once
#include "Types.h"
#include <windows.h>
#include "Engine.h"

#define FACE_POLYGON 1

// Enum of all lumps in the BSP
enum Lumps
{
	Entities = 0,
	Textures,
	Planes,
	Nodes,
	Leafs,
	LeafFaces,
	LeafBrushes,
	Models,
	Brushes,
	BrushSides,
	Vertices,
	Indices,
	Shaders,
	Faces,
	Lightmaps,
	LightVolumes,
	VisData,
	MaxLumps
};

// Our BSP header. Basically tells us if this is a bsp file and what version it is
struct BSPHeader
{
	char ID[4];  // This should be IBSP
	int version; // For quake 3 this should be 0x2e
};

// BSP Lump ( AKA A section in the File such as the faces or textures )
struct BSPLump
{
	int offset, length; // Offset into the file and length for the lump
};

struct BSPVertex
{
	Vec3 Position;
	Vec2 TextureCoord;
	Vec2 LightMapCoord;
	Vec3 Normal;

	byte Color[4];
};

// A face in the BSP map
struct BSPFace
{
	int TexID;
	int Effect;
	int Type;
	int StartVertexIndex;
	int NumOfVerts;
	int StartIndex;
	int NumOfIndices;
	int LightMapID;
	int LightMapCorner[2];
	int LightMapSize[2];
	Vec3 MapPos;
	Vec3 MapVecs[2];
	Vec3 Normal;
	int size[2];
};

// The BSP texture
struct BSPTexture
{
	char Name[64];
	int Flags;
	int Contents;
};

// The BSP lightmap
struct BSPLightmap
{
	byte ImageBits[128][128][3];
};

// What the fuck?
class Bitset
{
public:
	Bitset() : Bits(0), Size(0) {}
	~Bitset() { if (Bits) { delete Bits; Bits = NULL; } }
	void Resize(int count) { Size = count / 32 + 1; if (Bits) { delete[] Bits; Bits = 0; } Bits = new unsigned int[Size]; ClearAll(); }
	void Set(int i) { Bits[i >> 5] |= (1 << (i & 31)); }
	int On(int i) { return Bits[i >> 5] & (1 << (i & 31)); }
	void Clear(int i) { Bits[i >> 5] &= ~(1 << (i & 31)); }
	void ClearAll() { memset(Bits, 0, sizeof(unsigned int) * Size); }
private:
	unsigned int* Bits;
	int Size;
};

// Our BSP Map class.
class CBSPMap
{
public:
	CBSPMap();
	~CBSPMap();

	Result LoadBSP(const char* filename);

	void RenderLevel(CEngineDevice* Dev);

	void Destroy();
private:
	void FindTextureExtension(char* filename);
	void RenderFace(int FaceIndex, CEngineDevice* Dev);
	int NumOfVerts;
	int NumOfFaces;
	int NumOfIndices;
	int NumOfTextures;
	int NumOfLightmaps;

	int* Indices;
	BSPVertex* Verts;
	BSPFace* Faces;

	unsigned int LightMaps[1024];
	unsigned int Textures[1024];
	Bitset FacesDrawn;
};