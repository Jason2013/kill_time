// SphereWorld32.c
// OpenGL SuperBible
// Program by Richard S. Wright Jr.
// This program demonstrates a full featured robust Win32
// OpenGL framework

///////////////////////////////////////////////////////////////////////////////
// Include Files
#include "stdafx.h"

#include <windows.h>                // Win32 Framework (No MFC)
#include <gl\gl.h>                  // OpenGL
#include <gl\glu.h>                 // GLU Library
#include <stdio.h>                  // Standard IO (sprintf)
#include "wglext.h"    // WGL Extension Header
#include "glext.h"	    // OpenGL Extension Header
#include "gltools.h"   // GLTools library
#include "resource.h"               // Dialog resources

#include "startupOptions.h"


///////////////////////////////////////////////////////////////////////////////
struct STARTUPOPTIONS startupOptions;    // Startup options info

///////////////////////////////////////////////////////////////////////////////
// Forward Declarations

// Startup Dialog Procedure
BOOL APIENTRY StartupDlgProc (HWND hDlg, UINT message, 
							  UINT wParam, LONG lParam);

BOOL ShowStartupOptions(void);          // Initial startup dialog

///////////////////////////////////////////////////////////////////////////////
// Extension function pointers
PFNWGLGETPIXELFORMATATTRIBIVARBPROC wglGetPixelFormatAttribivARB = NULL;
PFNGLWINDOWPOS2IPROC glWindowPos2i = NULL;
PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = NULL;


/////////////////////////////////////////////////////////////////////////////
// Dialog procedure for the startup dialog
BOOL APIENTRY StartupDlgProc (HWND hDlg, UINT message, UINT wParam, LONG lParam)
{
	switch (message)
	{
		// Initialize the dialog box
	case WM_INITDIALOG:
		{
			int nPF;
			HDC hDC;                        // Dialogs device context
			HGLRC hRC;
			DEVMODE devMode;
			unsigned int iMode;
			unsigned int nWidth;	// Current settings
			unsigned int nHeight;
			char cBuffer[64];
			HWND hListBox;

			PIXELFORMATDESCRIPTOR pfd = {   // Not going to be too picky
				sizeof(PIXELFORMATDESCRIPTOR),
					1,
					PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
					PFD_TYPE_RGBA,		// Full color
					32,					// Color depth
					0,0,0,0,0,0,0,		// Ignored
					0,0,0,0,		    // Accumulation buffer
					16,					// Depth bits
					8,					// Stencil bits
					0,0,0,0,0,0 };		// Some used, some not


				// Initialize render options
				startupOptions.bFSAA = FALSE;
				startupOptions.bFullScreen = FALSE;
				startupOptions.bVerticalSync = FALSE;

				// Create a "temporary" OpenGL rendering context
				hDC = GetDC(hDlg);

				// Set pixel format one time....
				nPF = ChoosePixelFormat(hDC, &pfd);
				SetPixelFormat(hDC, nPF, &pfd);
				DescribePixelFormat(hDC, nPF, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

				// Create the GL context
				hRC = wglCreateContext(hDC);
				wglMakeCurrent(hDC, hRC);

				// Set text in dialog
				SetDlgItemText(hDlg, IDC_VENDOR, (const char *)glGetString(GL_VENDOR));
				SetDlgItemText(hDlg, IDC_RENDERER, (const char *)glGetString(GL_RENDERER));
				SetDlgItemText(hDlg, IDC_VERSION, (const char *)glGetString(GL_VERSION));

				// Vertical Sync off by default
				/*if(gltIsExtSupported("WGL_EXT_swap_control"))
				EnableWindow(GetDlgItem(hDlg, IDC_VSYNC_CHECK), TRUE);*/

				// Find a multisampled and non-multisampled pixel format
				//FindBestPF(hDC, &startupOptions.nPixelFormat, &startupOptions.nPixelFormatMS);

				// Done with GL context
				wglMakeCurrent(hDC, NULL);
				wglDeleteContext(hRC);

				// Enumerate display modes
				iMode = 0;
				nWidth = GetSystemMetrics(SM_CXSCREEN);	// Current settings
				nHeight = GetSystemMetrics(SM_CYSCREEN);
				hListBox = GetDlgItem(hDlg, IDC_DISPLAY_COMBO);
				while(EnumDisplaySettings(NULL, iMode, &devMode))
				{
					//if(devMode.dmBitsPerPel == pfd.cColorBits)
					{
						int iItem;
						sprintf(cBuffer,"%d x %d x %dbpp @%dhz", devMode.dmPelsWidth,
							devMode.dmPelsHeight, devMode.dmBitsPerPel, devMode.dmDisplayFrequency);

						iItem = SendMessage(hListBox, CB_ADDSTRING, 0, (LPARAM)cBuffer);
						SendMessage(hListBox, CB_SETITEMDATA, iItem, iMode);

						if(devMode.dmPelsHeight == nHeight &&
							devMode.dmPelsWidth == nWidth)
							SendMessage(hListBox, CB_SETCURSEL, iItem, 0);
					}
					iMode++;
				}


				// Set other defaults /////////////
				// Windowed or full screen
				CheckDlgButton(hDlg, IDC_FS_CHECK, BST_CHECKED);	

				// FSAA, but only if support detected
				if(startupOptions.nPixelFormatMS != 0)
					EnableWindow(GetDlgItem(hDlg, IDC_MULTISAMPLED_CHECK), TRUE);

				return (TRUE);
		}
		break;

		// Process command messages
	case WM_COMMAND:      
		{
			// Validate and Make the changes
			if(LOWORD(wParam) == IDOK)
			{
				// Read options ////////////////////////////////////////
				// Display mode
				HWND hListBox = GetDlgItem(hDlg, IDC_DISPLAY_COMBO);
				int iMode = SendMessage(hListBox, CB_GETCURSEL, 0, 0);
				iMode = SendMessage(hListBox, CB_GETITEMDATA, iMode, 0);
				EnumDisplaySettings(NULL, iMode, &startupOptions.devMode);

				// Full screen or windowed?
				if(IsDlgButtonChecked(hDlg, IDC_FS_CHECK))
					startupOptions.bFullScreen = TRUE;
				else
					startupOptions.bFullScreen = FALSE;


				// FSAA
				if(IsDlgButtonChecked(hDlg, IDC_MULTISAMPLED_CHECK))
					startupOptions.bFSAA = TRUE;
				else
					startupOptions.bFSAA = FALSE;

				// Vertical Sync.
				if(IsDlgButtonChecked(hDlg, IDC_VSYNC_CHECK))
					startupOptions.bVerticalSync = TRUE;
				else
					startupOptions.bVerticalSync = FALSE;


				EndDialog(hDlg,TRUE);
			}

			if(LOWORD(wParam) == IDCANCEL)
				EndDialog(hDlg, FALSE);
		}
		break;

		// Closed from sysbox
	case WM_CLOSE:
		EndDialog(hDlg,FALSE); // Same as cancel
		break;
	}

	return FALSE;
}


///////////////////////////////////////////////////////////////////////////////
// Display the startup screen (just a modal dialog box)
BOOL ShowStartupOptions(void)
{
	return DialogBox ((HINSTANCE)GetModuleHandle(NULL),
		MAKEINTRESOURCE(IDD_DLG_INTRO),
		NULL,
		StartupDlgProc);
}


