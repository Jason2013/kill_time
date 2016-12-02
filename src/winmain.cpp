#define WIN32_MEAN_AND_LEAN
#define WIN32_EXTRA_LEAN

/*
	WINMAIN.CPP

	Contains WinMain, the main windows function

	Author: Kevin Hawkins
	Date: 3/19/2001
	Description:

*/


//#include "stdafx.h"
#include "utils.h"
#include "vtrace.h"
#include <windows.h>

#include "oglwindow.h"		// the OpenGL window class
#include "vector.h"
#include "engine.h"			// the engine's main class
#include "simpengine.h"
#include "startupOptions.h"

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int nCmdShow)
{
	int loopRet;
	bool fscreen;
	//char str[700];

	if (!CheckRequiredFiles())
	{
		return -1;
	}

	VTRACE_ON;
	VTRACE("\n=== Program Start ===\n");

	CoInitialize(NULL);

	if (!COGLWindow::RegisterWindow(hInst))
	{
		MessageBox(NULL, "Failed to register window class", "Error", MB_OK);
		return -1;
	}

	srand((unsigned int)time(NULL));

	CSimpEngine *engine = NULL;

	try
	{	
		if (ShowStartupOptions() != IDOK)
		{
			return 0;
		}

		fscreen = (startupOptions.bFullScreen == TRUE);

		//engine = new CSimpEngine("OpenGL Game", fscreen, 800, 600, 16);
		engine = new CSimpEngine("OpenGL Game", fscreen, 
			startupOptions.devMode.dmPelsWidth,
			startupOptions.devMode.dmPelsHeight,
			startupOptions.devMode.dmBitsPerPel);// , 768, 32);
//winmain_1:
		//engine = new CSimpEngine("OpenGL Game", false, 800, 600, 32);
			//startupOptions.devMode.dmPelsWidth,
			//startupOptions.devMode.dmPelsHeight,
			//startupOptions.devMode.dmBitsPerPel);// , 768, 32);

		engine->OnGetWorld()->LoadGameResource();
		engine->OnGetWorld()->LoadWorld();
		loopRet = engine->EnterMessageLoop();
		engine->OnGetWorld()->FreeGameResource();

		delete engine;

		return loopRet;
	}
	catch(char *sz)
	{	
		MessageBox(NULL, sz, 0, 0);
		delete engine;
	}

	CoUninitialize();

	VTRACE_OFF;
	VTRACE("\n=== Program End ===\n");

	return -1;
}