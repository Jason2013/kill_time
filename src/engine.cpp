#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN

//#include "stdafx.h"
#include "vtrace.h"
#include <stdlib.h>

#include "engine.h"
#include "HiResTimer.h"
#include "camera.h"
#include "world.h"

void CEngine::CheckInput(float deltaTime)
{
	static float buttonDelta = 0.0f;

	int mouseDeltaX, mouseDeltaY;	// changes in the mouse position

	// decrease amount of time until next possible recognized button pressing
	buttonDelta -= deltaTime;

	if (buttonDelta < 0.0f)
		buttonDelta = 0.0f;

	// update devices
	inputSystem->Update();

	// retrieve the latest mouse movements
	inputSystem->GetMouseMovement(mouseDeltaX, mouseDeltaY);
	OnMouseMove(mouseDeltaX, mouseDeltaY);

	if (inputSystem->KeyDown(DIK_W) || inputSystem->KeyDown(DIK_UP))
		OnKeyDown(VK_UP);
	if (inputSystem->KeyDown(DIK_S) || inputSystem->KeyDown(DIK_DOWN))
		OnKeyDown(VK_DOWN);
	if (inputSystem->KeyDown(DIK_A) || inputSystem->KeyDown(DIK_LEFT))
		OnKeyDown(VK_LEFT);
	if (inputSystem->KeyDown(DIK_D) || inputSystem->KeyDown(DIK_RIGHT))
		OnKeyDown(VK_RIGHT);
	if (inputSystem->KeyDown(DIK_ADD))
		OnKeyDown(VK_ADD);
	if (inputSystem->KeyDown(DIK_SUBTRACT))
		OnKeyDown(VK_SUBTRACT);
	if (inputSystem->KeyDown(DIK_ESCAPE))
		OnKeyDown(VK_ESCAPE);
	if (inputSystem->KeyDown(DIK_F2))
		OnKeyDown(VK_F2);
	if (inputSystem->KeyDown(DIK_F3))
		OnKeyDown(VK_F3);
	if (inputSystem->KeyDown(DIK_LSHIFT) || inputSystem->KeyDown(DIK_RSHIFT))
	{
		OnKeyDown(1000);
	}
	else
	{
		OnKeyDown(2000);
	}
	if (inputSystem->ButtonDown(0))
	{
		if (buttonDelta == 0.0f)
		{
			OnMouseDownL(0,0);
			buttonDelta = 0.5f; // 重置延时为0.5秒
		}
	}
	if (inputSystem->KeyPressing(DIK_F1))
		OnKeyPressing(DIK_F1);
	if (inputSystem->KeyPressing(DIK_F2))
		OnKeyPressing(DIK_F2);
	{
		static bool s_lastState = false;
		bool state = inputSystem->ButtonDown(1);
		if (state && (!s_lastState))
		{
			OnMouseDownR(0, 0);
		}
		s_lastState = state;
	}
}

void CEngine::GameCycle(float deltaTime)
{
	CCamera *camera = OnGetCamera();	// get the camera
	CWorld *world = OnGetWorld();		// get the world

	if (useDInput)
		CheckInput(deltaTime);

	// setup opengl for frame (clear, identity)
	OnPrepare();

	// prepare objects and perform collisions
	world->Prepare();

	// move/orient camera
	camera->Animate(deltaTime);

	// move/orient objects
	world->Animate(deltaTime);//, timer->GetFPS(1));

	// draw objects
	world->Draw(camera);

	// swap buffers
	SwapBuffers(hDC);
}


// EnterMessageLoop()
// desc: the Windows message loop
LRESULT CEngine::EnterMessageLoop()
{
	//	Message loop
	MSG msg;
	timer = new CHiResTimer;

	timer->Init();
	
	OnGetWorld()->SetTimer(timer);

    SetWindowText(hWnd, "杀戮时刻");
	while (true)
	{
		GameCycle(timer->GetElapsedSeconds());
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}

			TranslateMessage (&msg);
			DispatchMessage (&msg);
		}
	}

	delete timer;

	return msg.wParam;
}