#ifndef __TERRAIN_H
#define __TERRAIN_H

#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN

/*
	TERRAIN.H

	The CTerrain class interface

	OpenGL Game Programming
	Author: Kevin Hawkins
	Date: 3/29/2001
	Description: The CTerrain class is derived from CObject, and represents
			   the main world. Randomly generated terrain and frustrum clipping
			   are provided. 

			   The random terrain is created with the midpoint
			   displacement algorithm.

	Random terrain generation Copyright (C) Jason Shankel, 2000

*/

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>

#include "camera.h"
#include "object.h"
#include "texture.h"
#include "vector.h"

struct TriIndex
{
	GLushort v1,v2,v3;
};

struct VertTex
{
	GLfloat s,t;
};

class CTerrain : public CObject
{
private:
	int width;		// terrain is of size width X width
					// preferrably with 2^n = width
	float terrainMul;
	float heightMul;
	float scanDepth;
	float textureMul;

	float RangedRandom(float v1,float v2);
	void NormalizeTerrain(float field[],int size);
	void FilterHeightBand(float *band,int stride,int count,float filter);
	void FilterHeightField(float field[],int size,float filter);
	void MakeTerrainPlasma(float field[],int size,float rough);

protected:
	// terrain doesn't move, so no physics animations
	void OnAnimate(scalar_t deltaTime) {}

	void OnDraw(CCamera *camera);
	void OnCollision(CObject *collisionObject);

public:
	float *heightMap;		// dynamic heightmap
	CTexture terrainTex[5];	// for multiple textures on the terrain
	float fogColor[4];		// color of the fog/sky

	CVector * vertices;
	BOOL	* vertVisible; // Visible Flag;
	VertTex * vertTexCoord;

	//CVector		* trisCG; // Center of Gravity
	BOOL		* trisVisible; // Visibility of Triangles
	TriIndex	* trisIndex; // Triangles Indexes
	int			* trisDrawIndex;
	UINT		  trisDrawNum;
	//float		* trisBufferF;
	UINT		  trisVisibleCount;


	//CTerrain();
	CTerrain(int width, float rFactor);
	~CTerrain()
	{
		delete [] heightMap;

		delete [] vertices;
		delete [] vertVisible;
		delete [] vertTexCoord;

		//delete [] trisCG;
		delete [] trisVisible;
		delete [] trisIndex;
		delete [] trisDrawIndex;
		//delete [] trisBufferF;
	}

	void Load() {}
	void Unload() {}

	void BuildTerrain(int width, float rFactor);

	float GetWidth() { return (float)width; }
	float GetMul() { return terrainMul; }
	float GetScanDepth() { return scanDepth; }

	void CalcVertexTexCoord()
	{
		for (int z=0; z<width; z++)
		{
			for (int x=0; x<width; x++)
			{
				vertTexCoord[x+z*width].s = x * textureMul;
				vertTexCoord[x+z*width].t = z * textureMul;
			}
		}
	}

	void CalcTriangleIndex()
	{
		for (int index=0; index<(width-1)*(width-1)*2; index++)
		{
			int m = (width-1)*2;
			int z = index / m;
			int x0 = index % m;
			int x = x0 / 2;
			if (x0 % 2 == 0)
			{
				trisIndex[index].v1 = x + z*width;
				trisIndex[index].v2 = x + (z+1)*width;
				trisIndex[index].v3 = x +1 + z * width;
			}
			else // x0 % 2 == 1
			{
				trisIndex[index].v1 = x +1 + z * width;
				trisIndex[index].v2 = x + (z+1)*width;
				trisIndex[index].v3 = x + 1 + (z+1)*width;
			}
		}
	}

	void CalcVertexVisibility(CCamera * camera)
	{
		for (int i=0; i<(width*width); i++)
		{
			if (camera->VisibleOfPoint(vertices[i]))
			{
				vertVisible[i] = TRUE;
			}
			else
			{
				vertVisible[i] = FALSE;
			}
		}

		int z = (int)(camera->position.z / terrainMul);
		int x = (int)(camera->position.x / terrainMul);

		int x0 = x - 2;
		if (x0 <0) x0 =0;

		int x1 = x + 2;
		if (x1> width-1) x1 = width-1;

		int z0 = z - 2;
		if (z0 <0) z0 =0;

		int z1 = z + 2;
		if (z1> width-1) z1 = width-1;

		for (int ix =x0; ix <= x1; ix++ )
		{
			for (int iz = z0; iz <= z1; iz++)
			{
				vertVisible[iz*width+ix] = TRUE;
			}
		}
	}

	void CalcTriangleVisibility(CCamera * camera)
	{
		for (int index=0; index<(width-1)*(width-1)*2; index++)
		{
			if (vertVisible[trisIndex[index].v1] ||
				vertVisible[trisIndex[index].v2] ||
				vertVisible[trisIndex[index].v3])
			{
				trisVisible[index] = TRUE;

			}
			else
			{
				trisVisible[index] = FALSE;
			}
		}
	}

	void CalcTriangleDrawIndex()
	{
		trisDrawNum = 0;
		for (int i=0; i<(width-1)*(width-1)*2; i++)
		{
			if (trisVisible[i])
			{
				trisDrawIndex[trisDrawNum++] = trisIndex[i].v1;
				trisDrawIndex[trisDrawNum++] = trisIndex[i].v2;
				trisDrawIndex[trisDrawNum++] = trisIndex[i].v3;
			}
		}
	}


	float GetHeight(float x, float z)
	{	
		float projCameraX, projCameraZ;

		// divide by the grid-spacing if it is not 1
		projCameraX = (float)(x / terrainMul);
		projCameraZ = (float)(z / terrainMul);

		// compute the height field coordinates (Col0, Row0)
		// and (Col1, Row1) that identify the height field cell 
		// directly below the camera.
		int col0 = int(projCameraX);
		int row0 = int(projCameraZ);
		int col1 = col0 + 1;
		int row1 = row0 + 1;
		
		// make sure that the cell coordinates don't fall
		// outside the height field.
		if (col1 > width)
			col1 = 0;
		if (row1 > width)
			row1 = 0;

		// get the four corner heights of the cell from the height field
		float h00 = vertices[col0+row0*width].y;
		float h01 = vertices[col1+row0*width].y;
		float h11 = vertices[col1+row1*width].y;
		float h10 = vertices[col0+row1*width].y;

		// calculate the position of the camera relative to the cell.
		// note, that 0 <= tx, ty <= 1.
		float tx = projCameraX - float(col0);
		float ty = projCameraZ - float(row0);

		// the next step is to perform a bilinear interpolation
		// to compute the height of the terrain directly below
		// the object.
		float txty = tx * ty;

		float final_height = h00 * (1.0f - ty - tx + txty)
						+ h01 * (tx - txty)
						+ h11 * txty
						+ h10 * (ty - txty);

		return final_height;
	}

	BOOL DirectView(const CVector & v1, const CVector & v2)
	{
		CVector pos(v1);
		float px = v1.x, py = v1.y, pz = v1.z;
		CVector n(v2-v1);
		float len = n.Length();
		n.Normalize();
		float x = n.x * 3.0f;
		float y = n.y * 3.0f;
		float z = n.z * 3.0f;
		
		float loc = 0;
		for (float loc = 0.0f; loc < len; loc += 3.0f)
		{
			if (GetHeight(px,pz) > py)
			{
				return FALSE;
			}
			px += x;
			py += y;
			pz += z;
		}
		return TRUE;
	}
};



#endif