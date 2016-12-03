//#include "stdafx.h"

#include "rocket.h"

CRocket::CRocket(CMD2Model * pmd2, Weapon type) : CEntity(pmd2)
{
	//VTRACE("CRocket::CRocket(%x)\n", this);
	velocity = CVector(0.0, 0.0, 120.0);
	acceleration = CVector(0.0, 0.0, 0.0);
	distanceTravel = 0.0f;
	size = 1.0f;
	isExplosion = false;

	explosion = NULL;
	explosionTex = new CTexture;
	m_wType = type;

	Load();
	
}

CRocket::~CRocket()
{
	//VTRACE("CRocket::~CRocket(%x)\n", this);
	if (explosion != NULL)
	{
		explosion->KillSystem();
		delete [] explosion;
		explosion = NULL;
	}

	if (explosionTex != NULL)
	{
		delete explosionTex;
		explosionTex = NULL;
	}
}

void CRocket::SetupExplosionTexture()
{
	glGenTextures(1, &explosionTex->texID);
	glBindTexture(GL_TEXTURE_2D, explosionTex->texID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	//glTexImage2D(GL_TEXTURE_2D, 0, 4, explosionTex->width, explosionTex->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, explosionTex->data);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 4, explosionTex->width, explosionTex->height, GL_RGBA, GL_UNSIGNED_BYTE, explosionTex->data);
}

void CRocket::OnAnimate(scalar_t deltaTime)
{
	//VTRACE("CRocket::OnAnimate(%f)", deltaTime);
	float cosYaw = (float)cos(DEG2RAD(direction));
	float sinYaw = (float)sin(DEG2RAD(direction)); 
	float sinPitch = (float)sin(DEG2RAD(pitch));

	float speed = velocity.z * deltaTime;

	position.x += float(cosYaw)*speed;
	position.y += float(sinPitch)*speed;
	position.z += float(sinYaw)*speed;

	distanceTravel += position.Length();

	if (isExplosion)
		explosion->Update(deltaTime);

	if (!isExplosion)
	{
		if (distanceTravel >= 2500.0f)
		{
			//isExplosion = true;
			//velocity = CVector(0.0, 0.0, 0.0);
			//explosion = new CExplosion(10, position, 0.1f, explosionTex->texID);
			Explode(false);
		}
	}

	CObject::OnAnimate(deltaTime);
}

void CRocket::OnCollision(CObject *collisionObject)
{
	if (!isExplosion)
	{
		if (typeid(*collisionObject) == typeid(CTerrain))
		{
			if (((CTerrain*)collisionObject)->GetHeight(position.x, position.z) + size >= position.y)
			{
				Explode(true);
				PlaySound();
			}
		}
		if (typeid(*collisionObject) == typeid(COgroEnemy) || typeid(*collisionObject) == typeid(CSodEnemy))
		{
			Explode(true);
			PlaySound();
		}
	}
}

void CRocket::Explode(bool onCollide)
{
	int num = onCollide ? 500 : 200;
	if (m_wType == wGun)
		num /= 10;

	float spread = m_wType == wGun ? 2.0f : 8.0f;

	isExplosion = true;
	velocity = CVector(0.0, 0.0, 0.0);
	explosion = new CExplosion(num, position, spread, explosionTex->texID);
	
}

void CRocket::OnDraw(CCamera *camera)
{
	// if the rocket has not yet exploded, draw the rocket model
	if (!isExplosion)
	{
		//if (! camera->VisibleOfSphere(position, size))
		if (! visible)
		{
			return;
		}
		glEnable(GL_TEXTURE_2D);
		glColor3f(1.0, 1.0, 1.0);

		glPushMatrix();
		glTranslatef(position.x, position.y, position.z);
		glRotatef(-direction, 0.0, 1.0, 0.0);
		glScalef(0.025f, 0.025f, 0.025f);
		pModel->RenderFrame(0);
		glPopMatrix();

		glDisable(GL_TEXTURE_2D);
	}
	// draw explosion
	else
	{
		glDisable(GL_FOG);
		explosion->Render(camera);
		glEnable(GL_FOG);
	}
}


void CRocket::Load()
{
	//CMD2Model::Load("models\\rocketair.md2", "models\\rocket.pcx");
	explosionTex->LoadTexture("media\\explosion.bmp");
	SetupExplosionTexture();
}

void CRocket::Unload()
{
}

void CRocket::OnPrepare()
{
	// perform collision detection from this entity with all other objects in world
	//if (!isExplosion)
	//	// TODO »ð¼ý¶ÔÏóÛs×²¼ì²â
	//	/*ProcessCollisions(FindRoot())*/;

	if (isExplosion)
	{
		if (explosion->IsDead() && !audioSys->GetPerformance()->IsPlaying(entitySound->GetSegment(), NULL))
		{
			explosion->KillSystem();
			delete explosion;
			explosion = NULL;
			isExplosion = false;

			isDead = true;
		}
	}
}
