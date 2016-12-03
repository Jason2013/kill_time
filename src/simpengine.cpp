#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN

//#include "stdafx.h"

#include "simpengine.h"

/*
	SIMPENGINE.CPP

	The SimpEngine implementation

	Author: Kevin Hawkins
	Date: 4/4/2001
	Description:

*/


void CSimpEngine::OnPrepare()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_SMOOTH);
	glClearStencil(0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	GLdouble fovy;

	fovy = gameWorld->camera->UsingTelescope() ? 5.4 : 54.0;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fovy, (float)width/(float)height, 0.1, 2000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	return;

}

void CSimpEngine::OnMouseDownL(float x, float y) 
{
	if (!gameWorld->IsGameDone())
		gameWorld->player->FireWeapon();
	//gameWorld->camera->
}

void CSimpEngine::OnMouseDownR(float x, float y) 
{
	if (!gameWorld->IsGameDone())
	{
		if (gameWorld->camera->UsingTelescope())
		{
			gameWorld->camera->SetTelescope(false);
		}
		else
		{
			gameWorld->camera->SetTelescope(true);
		}
	}
}

void CSimpEngine::OnKeyPressing(int nVirtKey)
{
	if (! gameWorld->IsGameDone())
	{
		switch(nVirtKey)
		{
		case DIK_F1:
			gameWorld->gui->SwitchShowHelp();
			break;
		case DIK_F2:
			gameWorld->SwitchWeapon();
			break;
		default:
			break;
		}
	}
}

void CSimpEngine::OnKeyDown(int nVirtKey)
{
	if (!gameWorld->IsGameDone())
	{
		switch (nVirtKey)
		{
		case VK_UP:
			gameCamera->velocity += CVector(0,0,2.0);
			break;
		case VK_DOWN:
			gameCamera->velocity += CVector(0,0,-2.0);
			break;
		case VK_RIGHT:
			gameCamera->velocity += CVector(1.0, 0.0, 0.0);
			break;
		case VK_LEFT:
			gameCamera->velocity += CVector(-1.0, 0.0, 0.0);
			break;
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		case VK_ADD:
			mouseSensitivity += 0.05f;
			break;
		case VK_SUBTRACT:
			mouseSensitivity -= 0.05f;
			if (mouseSensitivity < 0.01f)
				mouseSensitivity = 0.01f;
			break;
		//case VK_F2:
		//	gameCamera->SetTelescope(true);
		//	break;
		//case VK_F3:
		//	gameCamera->SetTelescope(false);
		//	break;
		case 1000:
			gameCamera->SetRunState(true);
			break;
		case 2000:
			gameCamera->SetRunState(false);
			break;
		default:
			break;
		}
	}
	else
	{
		if ((nVirtKey) == VK_ESCAPE)
			PostQuitMessage(0);
	}
}

void CSimpEngine::OnMouseMove(int deltaX, int deltaY)
{
	if (!gameWorld->IsGameDone())
	{
		if (gameCamera->UsingTelescope())
		{
			//
			gameCamera->yaw += (float)deltaX*mouseSensitivity/100.0f;
			gameCamera->pitch -= (float)deltaY*mouseSensitivity/100.0f;
		}
		else
		{


			gameCamera->yaw += deltaX*mouseSensitivity/10.0f;
			gameCamera->pitch -= deltaY*mouseSensitivity/10.0f;
		}
	}
}

void CSimpEngine::OnMouseMove(int x, int y, int centerX, int centerY)
{
	static float oldX;
	static float oldY;
	static float yaw = 0.0f;
	static float pitch = 0.0f;

	float mX, mY;

	mX = (float)x;
	mY = (float)y;

	if (mX < centerX/2)
		gameCamera->yaw -= 0.25f*mouseSensitivity;
	if (mX > centerX/2)
		gameCamera->yaw += 0.25f*mouseSensitivity;

	if (mY < centerY/2)
		gameCamera->pitch += 0.25f*mouseSensitivity;
	if (mY > centerY/2)
		gameCamera->pitch -= 0.25f*mouseSensitivity;

	oldX = mX;
	oldY = mY;
}