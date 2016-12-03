//#include "stdafx.h"
#include <tchar.h>
#include <string.h>
#include "gui.h"
#include <math.h>

#include "world.h"
extern int g_nVisibleOfEnemies;

//CGUI::CGUI()
//{
//	minutesLeft = secondsLeft = enemiesLeft = 0;
//	font = new CFont("Arial", 16);
//	endText = new CFont("Arial", 40);
//}

CGUI::CGUI()
{
	minutesLeft = secondsLeft = enemiesLeft = 0;
	m_font1 = CreateFont(-13,0,0,0,400,0,0,0,GB2312_CHARSET,0,0,0,FF_MODERN,"����");
	m_font2 = CreateFont(-26,0,0,0,400,0,0,0,GB2312_CHARSET,0,0,0,FF_MODERN,"����");
	m_font3 = CreateFont(-40,0,0,0,400,0,0,0,GB2312_CHARSET,0,0,0,FF_MODERN,"����_GB2312");
	m_font4 = CreateFont(-15,0,0,0,400,0,0,0,GB2312_CHARSET,0,0,0,FF_MODERN,"����");

	m_showHelp = GL_TRUE;
}

//CGUI::~CGUI()
//{
//	font->ClearFont();
//	endText->ClearFont();
//	delete font;
//	delete endText;
//}

CGUI::~CGUI()
{
	DeleteObject(m_font1);
	DeleteObject(m_font2);
	DeleteObject(m_font3);
	DeleteObject(m_font4);
}

void CGUI::SetCurrentTime(float timeLeft)//, float fps)
{
	// timeLeft is in seconds

	minutesLeft = (int)(timeLeft / 60.0);	// 60 seconds in 1 minute
	secondsLeft = (int)floor(timeLeft) % 60;
	millisecondsLeft = (int)((timeLeft - (float)floor(timeLeft)) * 60);
}

void CGUI::SetFPS(float fps)
{
	m_fps = fps;
}

void CGUI::SetEnemiesLeft(int eLeft)
{
	enemiesLeft = eLeft;
}

void CGUI::DrawWinner()
{

	static TCHAR szBuffer[256];

	EnterOrtho();

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_STENCIL_TEST);

//	endText->SetRGB(1.0f, 0.2f, 0.2f);

//	endText->SetPos2D(-100, 30);
//	endText->Print("YOU WIN!");

//	endText->SetPos2D(-160, -20);
//	endText->Print("Press Esc to exit");

	glColor3f(1.0f,0.2f,0.2f);

	sprintf(szBuffer, "��ʤ���ˣ�");
	ShowText(0, 26, szBuffer, Center, type2);

	sprintf(szBuffer, "��ESC���˳�");
	ShowText(0, 0, szBuffer, Center, type2);

	glEnable(GL_STENCIL_TEST);
	glEnable(GL_TEXTURE_2D);

	LeaveOrtho();
}

void CGUI::DrawLoser()
{

	static TCHAR szBuffer[256];

	EnterOrtho();

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_STENCIL_TEST);

	glColor3f(0.2f,1.0f,0.2f);

//	endText->SetRGB(0.2f, 1.0f, 0.2f);
//	endText->SetPos2D(-100, 30);
//	endText->Print("YOU LOSE!");

	sprintf(szBuffer, "��ʧ���ˣ�");
	ShowText(0, 26, szBuffer, Center, type2);

//	endText->SetPos2D(-160, -20);
//	endText->Print("Press Esc to exit");

	sprintf(szBuffer, "��ESC���˳�");
	ShowText(0, 0, szBuffer, Center, type2);

	glEnable(GL_STENCIL_TEST);
	glEnable(GL_TEXTURE_2D);

	LeaveOrtho();
}

void CGUI::Draw()
{
	static TCHAR szBuffer[256];

	glDisable(GL_STENCIL_TEST);
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	{
		GLint width, height;
		EnterOrtho(&width, &height);

		

		GLfloat r = (width < height) ? (GLfloat)width : (GLfloat)height;
		r /= 2.0f;

		glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
		glBegin(GL_LINES);
			glVertex2f(-r/10.0f,0.0f);
			glVertex2f(r/10.0f,0.0f);
			glVertex2f(0.0f,-r/10.0f);
			glVertex2f(0.0f,r/10.0f);
		glEnd();


		int y = height /2 - 16;
		int x = width /2 - 140;

//		font->SetRGB(1.0f, 0.0f, 0.0f);
//		font->SetPos2D(x, y);
//		font->Print("FPS: %03.2f", m_fps);

		sprintf(szBuffer, "ÿ��֡��:%03.2f", m_fps);
		ShowText(width / 2 - 10, y, szBuffer, Right, type1);

		y -= 16;

//		font->SetPos2D(x, y);
//		font->Print("Time: %02d:%02d:%02d",minutesLeft, secondsLeft, millisecondsLeft);
		sprintf(szBuffer, "ʣ��ʱ��:%02d:%02d:%02d",minutesLeft, secondsLeft, millisecondsLeft);
		ShowText(width / 2 - 10, y, szBuffer, Right, type1);

		y -= 16;
		sprintf(szBuffer, "ʣ�����:%02d",enemiesLeft);
		ShowText(width / 2 - 10, y, szBuffer, Right, type1);
		
		glColor4f(0.0f, 0.0f, 1.0f, 1.0f);

		ShowHelp(width, height, GL_TRUE);
		glColor4f(1.0f, 1.0f, 0.0f, 1.0f);

		ShowState(width, height);

		LeaveOrtho();
	}

	glDisable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_STENCIL_TEST);
}

void CGUI::ShowHelp(GLint width, GLint height,GLboolean show)
{
	static char * info[]={"F1��: ����/�رհ���",
		"F2��: �л�����",
		"W��/���Ϸ����: ǰ��",
		"S��/���·����: ����",
		"A��/�������: ����",
		"D��/���ҷ����: ����",
		"SHIFT��: �ܲ�(�ٶȼӱ�)",
		"����ƶ�: ����׼�Ƿ���",
		"������: ���",
		"����Ҽ�: ����/�ر���Զ��",
		"ESC��: �˳���Ϸ",
	};
	int x = -width/2 + 10;
	int y = height/2 - 16;

	ShowText(x, y, info[0], Left, type1);
	y -= 16;
	if (m_showHelp)
	{
		for (int i=1; i<sizeof(info)/sizeof(info[0]); i++)
		{
			ShowText(x, y, info[i], Left, type1);
			y -= 16;
		}
	}
	return;
	
}

void CGUI::ShowState(GLint width, GLint height)
{
	static char buffer[32];
	static char * info[]={"��Զ��: %s",
		"����: %s",
		"����",
		"�ر�",
		"���Ͳ",
		"��ǹ",
	};
	int x = width/2 - 10;
	int y = -height/2 + 16;

	sprintf(buffer, info[0], (world->camera->UsingTelescope() ? info[2] : info[3] ));
	ShowText(x, y, buffer, Right, type1);
	y += 16;

	sprintf(buffer, info[1], (world->GetWeapon() == wRocket ? info[4] : info[5] ));
	ShowText(x, y, buffer, Right, type1);

	return;
	y += 16;


	if (m_showHelp)
	{
		for (int i=1; i<sizeof(info)/sizeof(info[0]); i++)
		{
			ShowText(x, y, info[i], Left, type1);
			y -= 16;
		}
	}
	return;

}

void CGUI::ShowText(int x, int y, LPCTSTR lpszText, Align align, TextType type)
{
	struct
	{
		TextType type;
		HFONT font;
	} static sTable[] = {{type1, m_font1},
		{type2, m_font2},
		{type3, m_font3},
		{type4, m_font4}};
	
	HFONT hFont = NULL;
	for (int i=0; i<sizeof(sTable)/sizeof(sTable[0]); i++)
	{
		if (type == sTable[i].type)
		{
			hFont = sTable[i].font;
			break;
		}
	}

	SIZE size ={0,0}, bmSize={0,0};                                        //λͼ�ߴ�
	HDC hMemDC = CreateCompatibleDC(NULL);				//�ݴ��豸����

	//HFONT hFont  =CreateFont(-15,0,0,0,400,0,0,0,GB2312_CHARSET,0,0,0,FF_MODERN,"����");

	HFONT hFontOld =(HFONT)SelectObject(hMemDC,hFont);							//ѡ��������

	GetTextExtentPoint32(hMemDC,lpszText,_tcslen(lpszText),&bmSize);//��ȡ�ַ�λͼ��С

	HBITMAP hBitmap = CreateBitmap(bmSize.cx, bmSize.cy, 1, 1, NULL);//������hMemDC��ص�ɫλͼ

	HBITMAP hBitmapOld = (HBITMAP)SelectObject(hMemDC,hBitmap); //�ַ�λͼ��hMemDC����

	SetBkColor  (hMemDC, RGB(0,     0,   0));			//��ɫ����ɫ
	SetTextColor(hMemDC, RGB(255, 255, 255));			//��ɫ����ɫ
	TextOut(hMemDC, 0, 0, lpszText, strlen(lpszText));	//������ֵ��ݴ�hMemDC

	size.cx = (bmSize.cx + 31) & (~31);				//��Ե����
	size.cy = bmSize.cy;
	int bufsize =size.cy * size.cx;					//ͼ�����ݳ���
	struct {  BITMAPINFOHEADER bih;
	RGBQUAD col[2];
	}bic;                                      //���嵥ɫλͼ�ṹ
	BITMAPINFO *binf = (BITMAPINFO *)&bic;            //��ȡλͼ�ṹ��Ϣ
	binf->bmiHeader.biSize     = sizeof(binf->bmiHeader);//
	binf->bmiHeader.biWidth    = bmSize.cx; // bm.bmWidth;			//ͼ�ο�
	binf->bmiHeader.biHeight   = bmSize.cy; // bm.bmHeight;			//ͼ�θ�
	binf->bmiHeader.biPlanes   = 1;                   //
	binf->bmiHeader.biBitCount = 1;                   //��ɫ
	binf->bmiHeader.biCompression = BI_RGB;           //��ɫ��ʽ
	binf->bmiHeader.biSizeImage   = bufsize;          //ͼ�����ݳ���
	//UCHAR* Bits = new UCHAR[bufsize];					//����ͼ�����ݿ����
	static UCHAR Bits[64 * 1024];					//����ͼ�����ݿ����
	GetDIBits(hMemDC,hBitmap,0, bmSize.cy /* bm.bmHeight*/,Bits,binf,DIB_RGB_COLORS); 
	//ȡ�豸�޹����ݵ�Bits
	glPixelStorei(GL_UNPACK_ALIGNMENT ,1);			//�������ش洢
	if (align == Center)
	{
		glRasterPos2i(x-bmSize.cx/2,y);
	}
	else if (align == Right)
	{
		glRasterPos2i(x-bmSize.cx,y);
	}
	else
	{
		glRasterPos2i(x, y);
	}
	//glRasterPos2i(x,y);                               //ƽ�涨λ
	glBitmap(size.cx,size.cy,0,0,0,0,Bits);           //ƽ��λͼ��ʾ
	//delete Bits;                                      //ɾ��Bits
	SelectObject(hMemDC, hBitmapOld);                        //�ָ�λͼ����
	DeleteObject(hBitmap);
	SelectObject(hMemDC, hFontOld);
	//DeleteObject(hFont);
	DeleteDC(hMemDC);                                  //ɾ���豸����
}

void CGUI::SwitchShowHelp()
{
	m_showHelp = !m_showHelp;
}