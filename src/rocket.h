#ifndef __ROCKET_H
#define __ROCKET_H

/*
	
	FILENAME
	What is in this file?
	OpenGL Game Programming
	Author: Kevin Hawkins
	Date: XX/XX/XXXX
	Description:

*/

#include "types.h"
#include "explosion.h"
#include "terrain.h"
#include "player.h"
#include "camera.h"
#include "object.h"
#include "vector.h"

class CWorld;

class CRocket : public CEntity
{
private:
	void SetupExplosionTexture();
	inline void Explode(bool onCollide);

public:
	void OnAnimate(scalar_t deltaTime);
	void OnCollision(CObject *collisionObject);
protected:

	void OnDraw(CCamera *camera);
	void OnPrepare();

public:
	// direction rocket points
	float pitch;
	float distanceTravel;	// distance rocket has travelled
	CVector forward;		// forward vector of rocket (for pitch/dir)
	bool isExplosion;
	//Weapon m_wType;

	CTexture *explosionTex;
	CExplosion *explosion;

	CRocket(CMD2Model * pmd2);//, Weapon type = wRocket);
	~CRocket();

	void Load();
	void Unload();
	bool Exploded() {return isExplosion;}

};

#endif