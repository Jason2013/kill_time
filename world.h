#ifndef __WORLD_H
#define __WORLD_H

/*
	WORLD.H

  	The CWorld class

	Author: Kevin Hawkins
	Date: 3/29/2001
	Description: The CWorld class represents the game world
			   of the engine. All objects, the terrain,
			   audio system, ambient world music and sound,
			   and the camera are held here.


*/

#include <dmusicc.h>               // DirectMusic includes
#include <dmusici.h>
#include <d3d8types.h>             // for D3DVECTOR
#include <cguid.h>                 // for GUID_NULL
#include <typeinfo.h>			// for RTTI

#include "types.h"
#include "gui.h"
#include "player.h"
#include "sod.h"
#include "ogro.h"
#include "entity.h"
#include "md2.h"
#include "object.h"
#include "camera.h"
#include "terrain.h"
#include "audiosystem.h"
#include "tree.h"
#include "HiResTimer.h"

#define MAX_ENEMIES 10

//class CHiResTimer;
//class COgroEnemy;
//class CSodEnemy;
//class CCamera;
//class CMD2Model;
//class CPlayer;



class CWorld
{
//public:
//	

private:
	CHiResTimer * timer;
	int numOgros, numSods;
	int screenWidth, screenHeight;
	bool gameDone;

	CMD2Model * m_models[Rocket-Sod+1];
	int m_weapon;

protected:
	void OnAnimate(float deltaTime);
	void OnDraw(CCamera *camera);
	void OnPrepare();

public:
	HWND hwnd;

	CTerrain *terrain;			// the terrain
	CCamera *camera;			// the camera
	CPlayer *player;			// the player
	CAudioSystem *audioSystem;	// the audio system
	CAudio *worldSound;			// the world ambient sound
	CGUI *gui;

	COgroEnemy *ogroEnemy;		// ogro enemies
	CSodEnemy *sodEnemy;		// sod enemies

	float timeStart;
	float timeElapsed;

	CWorld();
	CWorld(CCamera *c);
	~CWorld();

	// initialize terrain, load objects and put in container
	void LoadWorld();
	void UnloadWorld();

	void LoadGameResource();
	void FreeGameResource();

	void SetTimer(CHiResTimer * t){timer = t;}
	CHiResTimer * GetTimer(){return timer;}

	

	/*CMD2Model * m_models[Rocket-Sod+1];*/

	int CountObjectTypes(const type_info &classID);

	// do physics calculations for all objects in the world
	// including collisions
	void Animate(float deltaTime);

	// render all objects in the world
	void Draw(CCamera *camera);
	void Prepare() { OnPrepare(); }

	void FadeScreen();
	void SetScreen(int width, int height);

	bool IsGameDone() { return gameDone; }
	void QuitGame() { gameDone = true; }
	int GetOgros() { return numOgros; }
	int GetSods() { return numSods; }
	CMD2Model * GetMd2Model(Model m) { return m_models[m];}

	//void SwitchWeapon();
	//Weapon GetWeapon() {return (Weapon)m_weapon;}

	//void DrawTelescope();
};


#endif
