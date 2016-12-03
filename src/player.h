#ifndef __PLAYER_H
#define __PLAYER_H

/*

	PLAYER.H

	The CPlayer class

	OpenGL Game Programming
	Author: Kevin Hawkins
	Date: 3/30/2001
	Description:

*/

#include <typeinfo.h>
class CWorld;
#include "audiosystem.h"
#include "sod.h"
#include "ogro.h"
#include "rocket.h"
#include "camera.h"
#include "object.h"
#include "terrain.h"
////#include "world.h"

class CPlayer : public CObject
{
private:
	
	CCamera *camera;
	CTerrain *terrain;
	CAudioSystem *audioSys;
	CAudio *rocketSound;
CWorld * world;
protected:
	void OnCollision(CObject *collisionObject)
	{
		if (typeid(*collisionObject) == typeid(CTerrain))
		{
			position.y = ((CTerrain*)collisionObject)->GetHeight(position.x, position.z) + size;
		}
		else if (typeid(*collisionObject) == typeid(COgroEnemy))
		{
			if (camera)
			{
				if (camera->velocity.z > 0.0)
					camera->velocity = CVector(0.0, 0.0, 0.0);
			}
		}
		else if (typeid(*collisionObject) == typeid(CSodEnemy))
		{
			if (camera)
			{
				if (camera->velocity.z > 0.0)
					camera->velocity = CVector(0.0, 0.0, 0.0);
			}
		}
		else if (typeid(*collisionObject) == typeid(CPlayer))
		{
		}
	}

	void OnPrepare()
	{
		if (camera)
		{
			position = camera->position;		// the player stays with the camera
			direction = camera->yaw;
			pitch = camera->pitch;
		}
	}

public:
	float direction;	// direction player is facing (same as camera)
	float pitch;		// pitch of player's lookAt vector

	CPlayer() { size = 7.0f; camera = NULL; terrain = NULL; audioSys = NULL; rocketSound = NULL; }
	~CPlayer() {}

	void SetCamera(CCamera *c) { camera = c; }
	void SetWorld(CWorld * w) {world = w;}
	void DetachCamera() { camera = NULL; }

	void SetTerrain(CTerrain *t) { terrain = t; }

	void FireWeapon();
	void SetAudioSystem(CAudioSystem *aSys) { audioSys = aSys; }
	BOOL CalcVisibility(CCamera * camera){visible = FALSE;return visible;}
};

#endif