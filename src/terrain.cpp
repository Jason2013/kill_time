//#include "stdafx.h"

#include "terrain.h"

//CTerrain::CTerrain()
//{
//	width = 256;
//	scanDepth = 80.0;
//	terrainMul = 50.0;
//	textureMul = 0.25;
//	heightMul = 175.0;
//	fogColor[0] = 0.75f;
//	fogColor[1] = 0.9f;
//	fogColor[2] = 1.0f;
//	fogColor[3] = 1.0f;
//
//	// CObject attributes
//	position = CVector(0,0,0);
//	velocity = CVector(0,0,0);
//	acceleration = CVector(0,0,0);
//	size = sqrt(width*terrainMul*width*terrainMul + width*terrainMul*width*terrainMul);
//
//	BuildTerrain(width, 0.5f);
//}
//
CTerrain::CTerrain(int w, float rFactor)
{
	width = w;
	scanDepth = 80.0; //80;
	terrainMul = 50.0;
	textureMul = 0.25;
	heightMul = 175.0;
	fogColor[0] = 0.75f;
	fogColor[1] = 0.9f;
	fogColor[2] = 1.0f;
	fogColor[3] = 1.0f;

	heightMap = NULL;

	// CObject attributes
	position = CVector(0,0,0);
	velocity = CVector(0,0,0);
	acceleration = CVector(0,0,0);
	size = sqrt(width*terrainMul*width*terrainMul + width*terrainMul*width*terrainMul);

	BuildTerrain(width, rFactor);
}

void CTerrain::BuildTerrain(int w, float rFactor)
{
	width = w;
	heightMap = new float[width*width];

	vertices = new CVector[width*width];
	vertVisible = new BOOL[width*width];
	vertTexCoord = new VertTex[width*width];
	

	//trisCG = new CVector[(width-1)*(width-1)*2];
	trisVisible = new BOOL[(width-1)*(width-1)*2];
	trisIndex = new TriIndex[(width-1)*(width-1)*2];
	trisDrawIndex = new int[(width-1)*(width-1)*2*3]; // num triangle * 3
	//trisBufferF = new float[(width-1)*(width-1)*2];
	
	memset(heightMap, 0, sizeof(float)*width*width);

	MakeTerrainPlasma(heightMap, width, rFactor);
	
	// load texture
	terrainTex[0].LoadTexture("media/ground.tga");
	glGenTextures(1, &terrainTex[0].texID);
	glBindTexture(GL_TEXTURE_2D, terrainTex[0].texID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	//or GL_CLAMP
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);	//or GL_CLAMP

	switch (terrainTex[0].textureType)
	{
	case BMP:
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, terrainTex[0].width, terrainTex[0].height,
					GL_RGB, GL_UNSIGNED_BYTE, terrainTex[0].data);
		break;
	case PCX:
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, terrainTex[0].width, terrainTex[0].height,
					GL_RGBA, GL_UNSIGNED_BYTE, terrainTex[0].data);
		break;
	case TGA:
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, terrainTex[0].width, terrainTex[0].height,
					GL_RGB, GL_UNSIGNED_BYTE, terrainTex[0].data);
		break;
	default:
		break;
	}
}

void CTerrain::OnCollision(CObject *collisionObject)
{}

void CTerrain::OnDraw(CCamera *camera)
{
	glEnable(GL_DEPTH_TEST);

	//glFogi(GL_FOG_MODE, GL_LINEAR);
	//glFogfv(GL_FOG_COLOR, fogColor);
	//glFogf(GL_FOG_START, scanDepth * 0.2f);
	//glFogf(GL_FOG_END, scanDepth * 2.5f);
	//glHint(GL_FOG_HINT, GL_FASTEST);
	//glEnable(GL_FOG);

	//glDisable(GL_FOG);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, terrainTex[0].texID);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	CalcVertexVisibility(camera);
	CalcTriangleVisibility(camera);
	CalcTriangleDrawIndex();

	glColor3f(1.0, 1.0, 1.0);

	glDrawElements(GL_TRIANGLES, trisDrawNum, GL_UNSIGNED_INT, trisDrawIndex);
	return;
}

// RangedRandom()
// Returns a random number between v1 and v2
float CTerrain::RangedRandom(float v1,float v2)
{
	return v1 + (v2-v1)*((float)rand())/((float)RAND_MAX);
}

// NormalizeTerrain()
// Given a height field, normalize it so that the minimum altitude
// is 0.0 and the maximum altitude is 1.0
void CTerrain::NormalizeTerrain(float field[],int size)
{
	float maxVal,minVal,dh;
	int i;

	/*
	Find the maximum and minimum values in the height field
	*/ 
	maxVal = field[0];
	minVal = field[0];

	for (i=1;i<size*size;i++)
	{
		if (field[i] > maxVal) 
		{
			maxVal = field[i];
		}
		else if (field[i] < minVal) 
		{
			minVal = field[i];
		}
	}

	/*
	Find the altitude range (dh)
	*/
	if (maxVal <= minVal) return;
	dh = maxVal-minVal;

	/*
	Scale all the values so they are in the range 0-1
	*/
	for (i=0;i<size*size;i++)
	{
		field[i] = (field[i]-minVal)/dh;//field[i] = 0;
	}

	for (int z=0; z<width; z++)
	{
		for (int x=0; x<width; x++)
		{
			int index = z*width + x;
			vertices[index].x = terrainMul*x;
			vertices[index].y = heightMul * field[index];//textureMul*x;
			vertices[index].z = terrainMul*z;
		}
	}

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, vertices);

	CalcVertexTexCoord(); // texture coordinate
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, vertTexCoord);

	CalcTriangleIndex(); // triangle indexes

}

// FilterHeightBand()
// Erosion filter -
// FilterHeightBand applies a FIR filter across a row or column of the height field
void CTerrain::FilterHeightBand(float *band,int stride,int count,float filter)
{
	int i,j=stride;
	float v = band[0];
	for (i=0;i<count-1;i++)
	{
		band[j] = filter*v + (1-filter)*band[j];
		v = band[j];
		j+=stride;
	}
}


// FilterHeightField()
// Erosion filter -
// Erodes a terrain in all 4 directions
void CTerrain::FilterHeightField(float field[],int size,float filter)
{
	int i;

	// Erode rows left to right
	for (i=0;i<size;i++)
	{
		FilterHeightBand(&field[size*i],1,size,filter);
	}
	
	// Erode rows right to left
	for (i=0;i<size;i++)
	{
		FilterHeightBand(&field[size*i+size-1],-1,size,filter);
	}

	// Erode columns top to bottom
	for (i=0;i<size;i++)
	{
		FilterHeightBand(&field[i],size,size,filter);
	}

	// Erode columns bottom to top
	for (i=0;i<size;i++)
	{
		FilterHeightBand(&field[size*(size-1)+i],-size,size,filter);
	}
}


// MakeTerrainPlasma()
// desc: Genereate terrain using diamond-square (plasma) algorithm
void CTerrain::MakeTerrainPlasma(float field[],int size,float rough)
{
	int i,j,ni,nj,mi,mj,pmi,pmj,rectSize = size;
	float dh = (float)rectSize/2,r = (float)pow((long double)2,(long double)-1*rough);

//	Since the terrain wraps, all 4 "corners" are represented by the value at 0,0,
//		so seeding the heightfield is very straightforward
//	Note that it doesn't matter what we use for a seed value, since we're going to
//		renormalize the terrain after we're done
	field[0] = 0;


	while(rectSize > 0)
	{
		
		/*
		Diamond step -

		Find the values at the center of the retangles by averaging the values at 
		the corners and adding a random offset:


		a.....b
		.     .  
		.  e  .
		.     .
		c.....d   

		e  = (a+b+c+d)/4 + random

		In the code below:
		a = (i,j)
		b = (ni,j)
		c = (i,nj)
		d = (ni,nj)
		e = (mi,mj)

		*/
		       
		for (i=0;i<size;i+=rectSize)
		for (j=0;j<size;j+=rectSize)
		{
			
			ni = (i+rectSize)%size;
			nj = (j+rectSize)%size;

			mi = (i+rectSize/2);
			mj = (j+rectSize/2);

			field[mi+mj*size] = (field[i+j*size] + field[ni+j*size] + field[i+nj*size] + field[ni+nj*size])/4 + RangedRandom(-dh/2,dh/2);
		}

		/*
		Square step -

		Find the values on the left and top sides of each rectangle
		The right and bottom sides are the left and top sides of the neighboring rectangles,
		  so we don't need to calculate them

		The height field wraps, so we're never left hanging.  The right side of the last
			rectangle in a row is the left side of the first rectangle in the row.  The bottom
			side of the last rectangle in a column is the top side of the first rectangle in
			the column

              .......
		      .     .
		      .     .
		      .  d  .
		      .     .
		      .     .
		......a..g..b
		.     .     .
		.     .     .
		.  e  h  f  .
		.     .     .
		.     .     .
		......c......

		g = (d+f+a+b)/4 + random
		h = (a+c+e+f)/4 + random
		
		In the code below:
			a = (i,j) 
			b = (ni,j) 
			c = (i,nj) 
			d = (mi,pmj) 
			e = (pmi,mj) 
			f = (mi,mj) 
			g = (mi,j)
			h = (i,mj)

		*/
		for (i=0;i<size;i+=rectSize)
		for (j=0;j<size;j+=rectSize)
		{
			ni = (i+rectSize)%size;
			nj = (j+rectSize)%size;

			mi = (i+rectSize/2);
			mj = (j+rectSize/2);

			pmi = (i-rectSize/2+size)%size;
			pmj = (j-rectSize/2+size)%size;

			// Calculate the square value for the top side of the rectangle
			field[mi+j*size] = (field[i+j*size] + field[ni+j*size] + field[mi+pmj*size] + field[mi+mj*size])/4 + RangedRandom(-dh/2,dh/2);

			// Calculate the square value for the left side of the rectangle
			field[i+mj*size] = (field[i+j*size] + field[i+nj*size] + field[pmi+mj*size] + field[mi+mj*size])/4 + RangedRandom(-dh/2,dh/2);
		}

		// Setup values for next iteration
		// At this point, the height field has valid values at each of the coordinates that fall on a rectSize/2 boundary

		rectSize /= 2;
		dh *= r;
	}
	
	// Normalize terrain so minimum value is 0 and maximum value is 1
	NormalizeTerrain(field,size);
}