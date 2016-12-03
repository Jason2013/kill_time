#ifndef __CAMERA_H
#define __CAMERA_H

#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN

/*
	CAMERA.H

	The interface for CCamera - the camera system

	Author: Kevin Hawkins
	Date: 3/29/2001
	Description: The CCamera class is the camera system

*/

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>

#include "vector.h"

class CObject;
class CWorld;
class CTerrain;

class CCamera
{
private:
	// these are used for moving and changing camera orientation
	// through the MoveTo/LookTo methods
	CVector initPosition, finalPosition;
	CVector initLookAt, finalLookAt;

	CVector lookAtVel;			// velocity for looking at objects
	CVector lookAtAccel;		// acceleration for looking at objects

	void UpdateLookAt();
	void UpdateMoveTo();

	bool m_telescope;
	float m_speedScale;

public:
	CVector position;			// position of camera
	CVector velocity;			// velocity of camera
	CVector acceleration;		// acceleration of camera
	CVector lookAt;			// lookat vector

	// up, forward, right vectors
	CVector up;
	CVector forward;
	CVector right;

	// yaw and pitch angles
	float yaw;
	float pitch;

	int screenWidth, screenHeight;
	int centerX, centerY;

	// bottom, right, top, left clip plane normals
	CVector clipNormmals[4];

	CWorld * world;
	CTerrain * terrain;

	CCamera();
	CCamera(int width, int height) { m_telescope = false;m_speedScale = 1.0f;}
	CCamera(CVector *look);
	CCamera(CVector *pos, CVector *look);
	~CCamera();

	void SetTelescope(bool state);
	bool UsingTelescope();

	void LookAt(CObject *object);
	void LookAtNow(CObject *object);
	void MoveTo(CObject *object);
	void MoveToNow(CObject *object);
	void MoveToNow(scalar_t x, scalar_t y, scalar_t z);

	// right rotation along y-axis (yaw)
	void RotateYaw(scalar_t radians);
	void RotatePitch(scalar_t radians);
	void RotateRoll(scalar_t radians);

	// do physics calculations
	void Animate(scalar_t deltaTime);
	void SetRunState(bool);

	inline void CalcClipNormals();
	inline BOOL VisibleOfPoint(const CVector & pt);
	inline BOOL VisibleOfPointEx(const CVector & pt, const float & dis);
	inline BOOL VisibleOfTriangle(const CVector & v1, const CVector & v2, const CVector & v3);
	inline int VisibleOfSphere(const CVector & center, const float & r);
};

//extern const GLfloat g_fFovY;
extern GLfloat g_fAspect;

/*
==============================================
*/

void CCamera::CalcClipNormals()
{
	CVector n(lookAt.x - position.x, lookAt.y - position.y, lookAt.z - position.z);
	n.Normalize();
	CVector v(0.0f, 1.0f, 0.0f);
	CVector u;
	CrossProduct(n, v, u); // u = cross(n, v)
	CrossProduct(u, n, v); // v = cross(u, n)
	u.Normalize();
	v.Normalize();

	GLfloat h = tan((GLfloat)(54.0f * 3.1415926535f /360.0f));///180.0*3.14/2.0f));
	GLfloat w = g_fAspect * h; //1.33333333333f * h;//

	/*
	3....2
	.    .
	.    .
	0....1
	*/

	CVector v0(u*(-w) + v*(-h) + n);
	CVector v1(u*w    + v*(-h) + n);
	CVector v2(u*w    + v*h    + n);
	CVector v3(u*(-w) + v*h    + n);

	//v0.Normalize();
	//v1.Normalize();
	//v2.Normalize();
	//v3.Normalize();


	//glColor3f(1.0f, 0.0f, 0.0f); // red
	//float dis = 1.0f;
	//CVector vt = position  + v0*50.0f + n * dis;//;//+ n*5.0f;
	//
	//// bottom
	//glBegin(GL_LINE_LOOP);
	//	glVertex3f(vt.x, vt.y, vt.z);// position + n*5.0f + v0*50.0f);
	//	vt = position + n * dis;// + n*5.0f;// + v0*50.0f;
	//	glVertex3f(vt.x, vt.y, vt.z);
	//	//glVertex3fv(position + n*5.0f);// + v0*50.0f);
	//	vt = position + v1*50.0f + n * dis;// + n*5.0f;
	//	glVertex3f(vt.x, vt.y, vt.z);
	//	//glVertex3fv(position + n*5.0f + v1*50.0f);
	//glEnd();

	//// right
	//glBegin(GL_LINE_LOOP);
	//vt = position  + v1*50.0f + n * dis;
	//glVertex3f(vt.x, vt.y, vt.z);// position + n*5.0f + v0*50.0f);
	//vt = position + n * dis;// + n*5.0f;// + v0*50.0f;
	//glVertex3f(vt.x, vt.y, vt.z);
	////glVertex3fv(position + n*5.0f);// + v0*50.0f);
	//vt = position + v2*50.0f + n * dis;// + n*5.0f;
	//glVertex3f(vt.x, vt.y, vt.z);
	////glVertex3fv(position + n*5.0f + v1*50.0f);
	//glEnd();

	//// top
	//glBegin(GL_LINE_LOOP);
	//vt = position  + v2*50.0f + n * dis;
	//glVertex3f(vt.x, vt.y, vt.z);// position + n*5.0f + v0*50.0f);
	//vt = position + n * dis;// + n*5.0f;// + v0*50.0f;
	//glVertex3f(vt.x, vt.y, vt.z);
	////glVertex3fv(position + n*5.0f);// + v0*50.0f);
	//vt = position + v3*50.0f + n * dis;// + n*5.0f;
	//glVertex3f(vt.x, vt.y, vt.z);
	////glVertex3fv(position + n*5.0f + v1*50.0f);
	//glEnd();

	//// left
	//glBegin(GL_LINE_LOOP);
	//vt = position  + v3*50.0f + n * dis;
	//glVertex3f(vt.x, vt.y, vt.z);// position + n*5.0f + v0*50.0f);
	//vt = position + n * dis;// + n*5.0f;// + v0*50.0f;
	//glVertex3f(vt.x, vt.y, vt.z);
	////glVertex3fv(position + n*5.0f);// + v0*50.0f);
	//vt = position + v0*50.0f + n * dis;// + n*5.0f;
	//glVertex3f(vt.x, vt.y, vt.z);
	////glVertex3fv(position + n*5.0f + v1*50.0f);
	//glEnd();

	CrossProduct(v1, v0, clipNormmals[0]);
	CrossProduct(v2, v1, clipNormmals[1]);
	CrossProduct(v3, v2, clipNormmals[2]);
	CrossProduct(v0, v3, clipNormmals[3]);

	clipNormmals[0].Normalize();
	clipNormmals[1].Normalize();
	clipNormmals[2].Normalize();
	clipNormmals[3].Normalize();

	return;
}

inline BOOL CCamera::VisibleOfPoint(const CVector & pt)
{
	CVector t(pt.x - position.x, pt.y - position.y, pt.z - position.z);
	GLfloat dot;

	dot = t.x * clipNormmals[1].x + t.y * clipNormmals[1].y + t.z * clipNormmals[1].z;
	if (dot<0) // right clip
		return FALSE;

	dot = t.x * clipNormmals[3].x + t.y * clipNormmals[3].y + t.z * clipNormmals[3].z;
	if (dot<0) // left clip
		return FALSE;

	dot = t.x * clipNormmals[0].x + t.y * clipNormmals[0].y + t.z * clipNormmals[0].z;
	if (dot<0) // bottom clip
		return FALSE;

	dot = t.x * clipNormmals[2].x + t.y * clipNormmals[2].y + t.z * clipNormmals[2].z;
	if (dot<0) // top clip
		return FALSE;

	return TRUE;
}

inline BOOL CCamera::VisibleOfPointEx(const CVector & pt,  const float & dis)
{
	CVector n(lookAt.x - position.x, lookAt.y - position.y, lookAt.z - position.z);
	n.Normalize();

	CVector newpos(position.x - n.x * dis, position.y - n.y * dis, position.z - n.z * dis);
	CVector t(pt.x - newpos.x, pt.y - newpos.y, pt.z - newpos.z);
	GLfloat dot;

	dot = t.x * clipNormmals[1].x + t.y * clipNormmals[1].y + t.z * clipNormmals[1].z;
	if (dot<0) // right clip
		return FALSE;

	dot = t.x * clipNormmals[3].x + t.y * clipNormmals[3].y + t.z * clipNormmals[3].z;
	if (dot<0) // left clip
		return FALSE;

	dot = t.x * clipNormmals[0].x + t.y * clipNormmals[0].y + t.z * clipNormmals[0].z;
	if (dot<0) // bottom clip
		return FALSE;

	dot = t.x * clipNormmals[2].x + t.y * clipNormmals[2].y + t.z * clipNormmals[2].z;
	if (dot<0) // top clip
		return FALSE;

	return TRUE;
}

inline BOOL CCamera::VisibleOfTriangle(const CVector & v1, const CVector & v2, const CVector & v3)
{

	GLfloat dot1, dot2, dot3;
	CVector t1, t2, t3;

	for (int i=0; i<4; i++)
	{
		t1.x = v1.x - position.x;
		t1.y = v1.y - position.y;
		t1.z = v1.z - position.z;
		dot1 = t1.x * clipNormmals[i].x + t1.y * clipNormmals[i].y + t1.z * clipNormmals[i].z;

		t2.x = v2.x - position.x;
		t2.y = v2.y - position.y;
		t2.z = v2.z - position.z;
		dot2 = t2.x * clipNormmals[i].x + t2.y * clipNormmals[i].y + t2.z * clipNormmals[i].z;

		t3.x = v3.x - position.x;
		t3.y = v3.y - position.y;
		t3.z = v3.z - position.z;
		dot3 = t3.x * clipNormmals[i].x + t3.y * clipNormmals[i].y + t3.z * clipNormmals[i].z;

		if (dot1 <=0 && dot2 <=0 && dot3 <= 0)
		{
			return FALSE;
		}
	}
	return TRUE;
}

inline BOOL CCamera::VisibleOfSphere(const CVector & center, const float & r)
{
	GLfloat dot;
	CVector t;

	t.x = center.x + clipNormmals[1].x * r - position.x;
	t.y = center.y + clipNormmals[1].y * r - position.y;
	t.z = center.z + clipNormmals[1].z * r - position.z;
	dot = t.x * clipNormmals[1].x + t.y * clipNormmals[1].y + t.z * clipNormmals[1].z;
	if (dot<0) // right clip
	{
		return -1;
	}

	t.x = center.x + clipNormmals[3].x * r - position.x;
	t.y = center.y + clipNormmals[3].y * r - position.y;
	t.z = center.z + clipNormmals[3].z * r - position.z;
	dot = t.x * clipNormmals[3].x + t.y * clipNormmals[3].y + t.z * clipNormmals[3].z;
	if (dot<0) // left clip
	{
		return -2;
	}

	t.x = center.x + clipNormmals[0].x * r - position.x;
	t.y = center.y + clipNormmals[0].y * r - position.y;
	t.z = center.z + clipNormmals[0].z * r - position.z;
	dot = t.x * clipNormmals[0].x + t.y * clipNormmals[0].y + t.z * clipNormmals[0].z;
	if (dot<0) // bottom clip
	{
		return -3;
	}

	t.x = center.x + clipNormmals[2].x * r - position.x;
	t.y = center.y + clipNormmals[2].y * r - position.y;
	t.z = center.z + clipNormmals[2].z * r - position.z;
	dot = t.x * clipNormmals[2].x + t.y * clipNormmals[2].y + t.z * clipNormmals[2].z;
	if (dot < 0) // top clip
	{
		return -4;
	}

	return 1;
}

#endif