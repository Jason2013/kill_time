#include "stdafx.h"

#include "entity.h"

CEntity::CEntity(CMD2Model * pmd2)
{
	stateStart = 0;
	stateEnd = 39;
	animSpeed = 7.0f;
	direction = 0.0f;
	entitySound = NULL;

	position = CVector(0,0,0);
	velocity = CVector(0,0,0);
	acceleration = CVector(0,0,0);
	size = 6.25f;

	currentFrame = 0;
	nextFrame = 1;
	interpol = 0.0f;

	pModel = pmd2;
}

CEntity::~CEntity()
{
	if (entitySound != NULL)
	{
		delete entitySound;
		entitySound = NULL;
	}
}

void CEntity::OnAnimate(float deltaTime)
{
	float cosYaw = (float)cos(DEG2RAD(direction));
	float sinYaw = (float)sin(DEG2RAD(direction)); 

	float speed = velocity.z * deltaTime;

	position.x += float(cosYaw)*speed;
	position.z += float(sinYaw)*speed;

	deltaT = deltaTime;	// used for interpolation

	CObject::OnAnimate(deltaTime);
}

void CEntity::OnDraw(CCamera *camera)
{
	glTranslatef(position.x, position.y, position.z);
	glRotatef(-direction, 0.0, 1.0, 0.0);
	glRotatef(90.0f, -1.0f, 0.0f, 0.0f);
	glScalef(0.25, 0.25, 0.25);
		
	pModel->AnimateModel(stateStart, stateEnd, deltaT*animSpeed, currentFrame, nextFrame, interpol);
	if (stateStart == 178)
	{
		VTRACE("(%u), stateStart=%d, stateEnd=%d, deltaT*animSpeed=%f, currentFrame=%d, nextFrame=%d, interpol=%f\n", this, stateStart, stateEnd, deltaT*animSpeed, currentFrame, nextFrame, interpol);

	}
}

void CEntity::LoadAudio(CAudioSystem *audioSystem, char *filename, bool is3DSound)
{
	if (entitySound != NULL)
	{
		delete [] entitySound;
		entitySound = new CAudio;
	}

	if (audioSystem != NULL)
		entitySound = audioSystem->Create(filename, is3DSound);
	else
	{
		delete [] entitySound;
		entitySound = NULL;
	}
}