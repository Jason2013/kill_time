//#include "stdafx.h"

#include "player.h"
#include "world.h"

void CPlayer::FireWeapon()
{
	// use camera->lookAt vector and multiply
	// this lookAt vector by the ammo's speed
	// to get the ammo velocity vector.
	// create the new ammo and attach it to
	// the world. fire the ammo

	CRocket *newRocket;
	if (world->GetWeapon() == wRocket)
	{
		newRocket = new CRocket(world->GetMd2Model(Rocket), wRocket);
	}
	else
	{
		newRocket = new CRocket(world->GetMd2Model(Rocket), wGun);
	}

	newRocket->pitch = camera->pitch;
	newRocket->direction = direction;
	newRocket->position = position;
	newRocket->forward = camera->lookAt.UnitVector();

	newRocket->SetAudioSystem(audioSys);
	newRocket->LoadAudio(audioSys, "media/explode.wav", false);
	newRocket->AttachTo(terrain);

	if (world->GetWeapon() == wRocket)
	{
		return;
	}

	while (! newRocket->Exploded())
	{
		CNode * pNode = CNode::header;
		while (pNode != NULL)
		{
			CObject * obj = (CObject*)pNode;
			if ((CObject*)newRocket != obj && ((newRocket->position - obj->position).Length() <= (newRocket->size + obj->size)) )
			{
				newRocket->OnCollision(obj);
				obj->OnCollision(newRocket);
			}
			pNode = pNode->next;
		}
		newRocket->OnAnimate(0.025f);
	}
}