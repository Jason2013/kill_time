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
	m_font1 = CreateFont(-13,0,0,0,400,0,0,0,GB2312_CHARSET,0,0,0,FF_MODERN,"宋体");
	m_font2 = CreateFont(-26,0,0,0,400,0,0,0,GB2312_CHARSET,0,0,0,FF_MODERN,"黑体");
	m_font3 = CreateFont(-40,0,0,0,400,0,0,0,GB2312_CHARSET,0,0,0,FF_MODERN,"楷体_GB2312");
	m_font4 = CreateFont(-15,0,0,0,400,0,0,0,GB2312_CHARSET,0,0,0,FF_MODERN,"宋体");

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

	sprintf(szBuffer, "你胜利了！");
	ShowText(0, 26, szBuffer, Center, type2);

	sprintf(szBuffer, "按ESC键退出");
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

	sprintf(szBuffer, "你失败了！");
	ShowText(0, 26, szBuffer, Center, type2);

//	endText->SetPos2D(-160, -20);
//	endText->Print("Press Esc to exit");

	sprintf(szBuffer, "按ESC键退出");
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

		sprintf(szBuffer, "每秒帧数:%03.2f", m_fps);
		ShowText(width / 2 - 10, y, szBuffer, Right, type1);

		y -= 16;

//		font->SetPos2D(x, y);
//		font->Print("Time: %02d:%02d:%02d",minutesLeft, secondsLeft, millisecondsLeft);
		sprintf(szBuffer, "剩余时间:%02d:%02d:%02d",minutesLeft, secondsLeft, millisecondsLeft);
		ShowText(width / 2 - 10, y, szBuffer, Right, type1);

		y -= 16;
		sprintf(szBuffer, "剩余敌人:%02d",enemiesLeft);
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
	static char * info[]={"F1键: 开启/关闭帮助",
		"F2键: 切换武器",
		"W键/向上方向键: 前进",
		"S键/向下方向键: 后退",
		"A键/向左方向键: 向左",
		"D键/向右方向键: 向右",
		"SHIFT键: 跑步(速度加倍)",
		"鼠标移动: 控制准星方向",
		"鼠标左键: 射击",
		"鼠标右键: 开启/关闭望远镜",
		"ESC键: 退出游戏",
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
	static char * info[]={"望远镜: %s",
		"武器: %s",
		"开启",
		"关闭",
		"火箭筒",
		"步枪",
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

	SIZE size ={0,0}, bmSize={0,0};                                        //位图尺寸
	HDC hMemDC = CreateCompatibleDC(NULL);				//暂存设备场景

	//HFONT hFont  =CreateFont(-15,0,0,0,400,0,0,0,GB2312_CHARSET,0,0,0,FF_MODERN,"宋体");

	HFONT hFontOld =(HFONT)SelectObject(hMemDC,hFont);							//选择新字体

	GetTextExtentPoint32(hMemDC,lpszText,_tcslen(lpszText),&bmSize);//获取字符位图大小

	HBITMAP hBitmap = CreateBitmap(bmSize.cx, bmSize.cy, 1, 1, NULL);//创建与hMemDC相关单色位图

	HBITMAP hBitmapOld = (HBITMAP)SelectObject(hMemDC,hBitmap); //字符位图与hMemDC关连

	SetBkColor  (hMemDC, RGB(0,     0,   0));			//底色，黑色
	SetTextColor(hMemDC, RGB(255, 255, 255));			//字色，白色
	TextOut(hMemDC, 0, 0, lpszText, strlen(lpszText));	//输出文字到暂存hMemDC

	size.cx = (bmSize.cx + 31) & (~31);				//边缘对齐
	size.cy = bmSize.cy;
	int bufsize =size.cy * size.cx;					//图形数据长度
	struct {  BITMAPINFOHEADER bih;
	RGBQUAD col[2];
	}bic;                                      //定义单色位图结构
	BITMAPINFO *binf = (BITMAPINFO *)&bic;            //获取位图结构信息
	binf->bmiHeader.biSize     = sizeof(binf->bmiHeader);//
	binf->bmiHeader.biWidth    = bmSize.cx; // bm.bmWidth;			//图形宽
	binf->bmiHeader.biHeight   = bmSize.cy; // bm.bmHeight;			//图形高
	binf->bmiHeader.biPlanes   = 1;                   //
	binf->bmiHeader.biBitCount = 1;                   //单色
	binf->bmiHeader.biCompression = BI_RGB;           //颜色方式
	binf->bmiHeader.biSizeImage   = bufsize;          //图形数据长度
	//UCHAR* Bits = new UCHAR[bufsize];					//定义图形数据块变量
	static UCHAR Bits[64 * 1024];					//定义图形数据块变量
	GetDIBits(hMemDC,hBitmap,0, bmSize.cy /* bm.bmHeight*/,Bits,binf,DIB_RGB_COLORS); 
	//取设备无关数据到Bits
	glPixelStorei(GL_UNPACK_ALIGNMENT ,1);			//控制像素存储
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
	//glRasterPos2i(x,y);                               //平面定位
	glBitmap(size.cx,size.cy,0,0,0,0,Bits);           //平面位图显示
	//delete Bits;                                      //删除Bits
	SelectObject(hMemDC, hBitmapOld);                        //恢复位图特性
	DeleteObject(hBitmap);
	SelectObject(hMemDC, hFontOld);
	//DeleteObject(hFont);
	DeleteDC(hMemDC);                                  //删除设备场景
}

void CGUI::SwitchShowHelp()
{
	m_showHelp = !m_showHelp;
}