#include "stdafx.h"

#include "entity.h"

//int g_nVisibleOfEnemies;

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
	//if (! visible)
	//{
	//	return;
	//}

	glPushMatrix();

	glTranslatef(position.x, position.y, position.z);
	glRotatef(-direction, 0.0, 1.0, 0.0);
	glRotatef(90.0f, -1.0f, 0.0f, 0.0f);
	glScalef(0.25, 0.25, 0.25);

	pModel->AnimateModel(stateStart, stateEnd, deltaT*animSpeed, currentFrame, nextFrame, interpol);//, visible);

	glPopMatrix();
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

BOOL CEntity::CalcVisibility(CCamera * camera)
{
	int v = camera->VisibleOfSphere(position, size);
	if (v<0)
	{
		visible = FALSE;
		return visible;
	}

	CVector newpos(position);
	newpos.y += size;
	BOOL vd = camera->terrain->DirectView(camera->position, newpos);
	if (!vd)
	{
		visible = FALSE;
		return visible;
	}

	visible = TRUE;
	return visible;
}
int  CEntity::CalcDistanceSqr(CCamera * camera)
{
	int x1,y1,z1,x2,y2,z2,x3,y3,z3;

	x1 = (int)camera->position.x;
	y1 = (int)camera->position.y;
	z1 = (int)camera->position.z;

	x2 = (int)position.x;
	y2 = (int)position.y;
	z2 = (int)position.z;

	x3=(x2-x1);
	y3=(y2-y1);
	z3=(z2-z1);

	return x3*x3+y3*y3+z3*z3;
}