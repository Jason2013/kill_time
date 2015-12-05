#ifndef __ENTITY_H
#define __ENTITY_H

/*
	ENTITY.H

	The CEntity class

	Author: Kevin Hawkins
	Date: 3/29/2001
	Description: The CEntity class is derived from CMD2Model and
			   provides the functionality of an entity in the
			   game world. It holds the current MD2 animation
			   state, the entity angle heading, and the entity's
			   primary sound.

*/

#include <stdlib.h>
#include <time.h>
#include <typeinfo.h>

#include "md2.h"
#include "object.h"
#include "camera.h"
#include "audiosystem.h"
#include "terrain.h"

//extern int g_nVisibleOfEnemies;

class CEntity : public CObject// CMD2Model
{
public:
	void OnAnimate(float deltaTime);
protected:
	void OnDraw(CCamera *camera);

	void PlaySound() { audioSys->Play(entitySound, 1); }

	CMD2Model * pModel;

public:
	float direction;		// angle the entity is facing (in degrees)
	CAudio *entitySound;	// the sound the entity makes
						// currently only supports one sound
						// per entity
	CAudioSystem *audioSys;	// a pointer to the audio system


	CEntity(CMD2Model * pmd2 = NULL);
	~CEntity();

	int stateStart, stateEnd;	// state keyframe start/end
	float deltaT;
	float animSpeed;

	int currentFrame;
	int nextFrame;
	float interpol;

	void LoadAudio(CAudioSystem *audioSystem, char *filename, bool is3DSound);
	void SetAudioSystem(CAudioSystem *asys) { audioSys = asys; }

	virtual BOOL CalcVisibility(CCamera * camera);//{visible = TRUE;return visible;}
	virtual int  CalcDistanceSqr(CCamera * camera);//{return 0;}
};

#endif