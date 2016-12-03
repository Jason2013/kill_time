// SphereWorld32.c
// OpenGL SuperBible
// Program by Richard S. Wright Jr.
// This program demonstrates a full featured robust Win32
// OpenGL framework

///////////////////////////////////////////////////////////////////////////////
 //Include Files
#include <windows.h>                // Win32 Framework (No MFC)
#include <gl\gl.h>                  // OpenGL
#include <gl\glu.h>                 // GLU Library
#include <stdio.h>                  // Standard IO (sprintf)
#include "wglext.h"    // WGL Extension Header
#include "glext.h"	    // OpenGL Extension Header
#include "gltools.h"   // GLTools library
#include "resource.h"               // Dialog resources


// Initial rendering options specified by the user.
struct STARTUPOPTIONS {
    DEVMODE	devMode;			// Display mode to use
    int	 nPixelFormat;			// Pixel format to use
    int	 nPixelFormatMS;		// Multisampled pixel format
    BOOL bFullScreen;			// Full screen?
    BOOL bFSAA;
    BOOL bVerticalSync;
    };

extern struct STARTUPOPTIONS startupOptions;    // Startup options info


///////////////////////////////////////////////////////////////////////////////
// Forward Declarations

// Startup Dialog Procedure
BOOL APIENTRY StartupDlgProc (HWND hDlg, UINT message, 
                                        UINT wParam, LONG lParam);

// Find the best available pixelformat, including if Multisample is available
// void FindBestPF(HDC hDC, int *nRegularFormat, int *nMSFormat);

BOOL ShowStartupOptions(void);          // Initial startup dialog

///////////////////////////////////////////////////////////////////////////////
// Extension function pointers
extern PFNWGLGETPIXELFORMATATTRIBIVARBPROC wglGetPixelFormatAttribivARB;
extern PFNGLWINDOWPOS2IPROC glWindowPos2i;
extern PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT;


