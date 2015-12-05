#ifndef __GUI_H
#define __GUI_H

#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN

/*
	
	GUI.H
	The GUI class interface
	OpenGL Game Programming
	Author: Kevin Hawkins
	Date: 4/2/2001
	Description:

*/

#include <windows.h>
#include "font.h"
#include "texture.h"
//#include "world.h"
class CWorld;

class CGUI
{
private:
	int minutesLeft, secondsLeft, millisecondsLeft;
	float m_fps;
	int enemiesLeft;

	CFont *font;
	//CFont *crosshair;
	CFont *endText;

	//HFONT m_font1, m_font2, m_font3, m_font4;

	GLboolean m_showHelp;
	CWorld * world;

public:
	CGUI();
	~CGUI();

	//enum Align {Left, Center, Right};
	//enum TextType {type1, type2, type3, type4};

	void SetCurrentTime(float timeLeft);
	void SetEnemiesLeft(int eLeft);
	void SetFPS(float fps);
	void SetWorld(CWorld *w){world = w;}// float fps);
	void Draw();
	void Animate(float deltaTime);

	void DrawWinner();
	void DrawLoser();

	//void ShowText(int x, int y, LPCTSTR lpszText, Align align, TextType type);
	//void ShowHelp(GLint width, GLint height,GLboolean show);
	//void SwitchShowHelp();
	//void ShowState(GLint width, GLint height);

	static inline void EnterOrtho(GLint * width = NULL, GLint * height = NULL);
	static inline void LeaveOrtho();
};

void CGUI::EnterOrtho(GLint * width, GLint * height)
{
	GLint vp[4];
	glGetIntegerv(GL_VIEWPORT, vp);

	if (width != NULL)
	{
		*width = vp[2];
	}
	if (height != NULL)
	{
		*height = vp[3];
	}

	glMatrixMode(GL_PROJECTION);

	glPushMatrix();
	glLoadIdentity();
	glOrtho((GLdouble)-vp[2]/2.0, (GLdouble)vp[2]/2.0,
		(GLdouble)-vp[3]/2.0, (GLdouble)vp[3]/2.0,
		-1.0, 1.0);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

}
void CGUI::LeaveOrtho()
{
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}


#endif