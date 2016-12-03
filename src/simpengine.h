#ifndef __SIMPENGINE
#define __SIMPENGINE

#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN

/*
	SIMPENGINE.H

	The SimpEngine class

	Author: Kevin Hawkins
	Date: 3/29/2001
	Description: The SimpEngine class is the main interface for SimpEngine.
			   Derived from CEngine, tt provides the camera and world objects.

*/
#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>

#include "engine.h"
#include "object.h"
#include "camera.h"
#include "terrain.h"
#include "world.h"

class CSimpEngine : public CEngine
{
private:
	CCamera *gameCamera;
	CWorld *gameWorld;
	int m_keyState[256];

public:
	CCamera *OnGetCamera() { return gameCamera; }
	CWorld *OnGetWorld() { return gameWorld; }
protected:
	void OnPrepare();
	void OnMouseDownL(float x, float y);
	void OnMouseDownR(float x, float y) ;
	void OnMouseMove(int deltaX, int deltaY);
	void OnMouseMove(int x, int y, int centerX, int centerY);
	void OnKeyDown(int nVirtKey);
	//void KeyState(unsigned int nVirtKey);
	void OnKeyPressing(int nVirtKey);

public:
	CSimpEngine() 
	{
		gameCamera = new CCamera;
		gameWorld = new CWorld;
		//gameWorld->SetTimer(GetTimer());
		memset(m_keyState, 0, sizeof(m_keyState));
	}

	CSimpEngine(const char *szName, bool fscreen, int w, int h, int b) :
		CEngine(szName, fscreen, w, h, b) 
	{
		gameCamera = new CCamera;
		gameWorld = new CWorld(gameCamera);
		gameCamera->world = gameWorld;
		gameCamera->centerX = centerX;
		gameCamera->centerY = centerY;
		gameWorld->SetScreen(w,h);
		//gameWorld->SetTimer(GetTimer());
		memset(m_keyState, 0, sizeof(m_keyState));
	}

	~CSimpEngine() 
	{
		delete gameWorld;
		delete gameCamera;
		gameWorld = NULL;
		gameCamera = NULL;
	}
};


#endif