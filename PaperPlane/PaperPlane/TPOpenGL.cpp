/********************************************************************************
*																				*
*	Description:	be used for sending or receiving data from serial port		*
*	Author	   :	Yingmou.Zhong												*
*	Date	   :    2016-06-20													*
*																				*
*********************************************************************************/
#include "StdAfx.h"
#include "TPOpenGL.h"

int TPOpenGL::InitGL(GLvoid)										// All Setup For OpenGL Goes Here
{
	glPolygonMode(GL_FRONT,GL_FILL);
	glPolygonMode(GL_BACK,GL_FILL);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);
	glViewport(0,0,259,231);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45,1,0.1,100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
	return TRUE;										// Initialization Went OK
}

bool TPOpenGL::SetWindowPixelFormat(HDC hDC)
{
	PIXELFORMATDESCRIPTOR pixelDesc;

	pixelDesc.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pixelDesc.nVersion = 1;

	pixelDesc.dwFlags = PFD_DRAW_TO_WINDOW | 
	PFD_SUPPORT_OPENGL |
	PFD_DOUBLEBUFFER |
	PFD_TYPE_RGBA;

	pixelDesc.iPixelType = PFD_TYPE_RGBA;
	pixelDesc.cColorBits = 32;
	pixelDesc.cRedBits = 0;
	pixelDesc.cRedShift = 0;
	pixelDesc.cGreenBits = 0;
	pixelDesc.cGreenShift = 0;
	pixelDesc.cBlueBits = 0;
	pixelDesc.cBlueShift = 0;
	pixelDesc.cAlphaBits = 0;
	pixelDesc.cAlphaShift = 0;
	pixelDesc.cAccumBits = 0;
	pixelDesc.cAccumRedBits = 0;
	pixelDesc.cAccumGreenBits = 0;
	pixelDesc.cAccumBlueBits = 0;
	pixelDesc.cAccumAlphaBits = 0;
	pixelDesc.cDepthBits = 0;
	pixelDesc.cStencilBits = 1;
	pixelDesc.cAuxBuffers = 0;
	pixelDesc.iLayerType = PFD_MAIN_PLANE;
	pixelDesc.bReserved = 0;
	pixelDesc.dwLayerMask = 0;
	pixelDesc.dwVisibleMask = 0;
	pixelDesc.dwDamageMask = 0;

	PixelFormat = ChoosePixelFormat(hDC,&pixelDesc);
	if(PixelFormat==0) // Choose default
	{
	 PixelFormat = 1;
	 if(DescribePixelFormat(hDC,PixelFormat,
	  sizeof(PIXELFORMATDESCRIPTOR),&pixelDesc)==0)
	 {
	  return FALSE;
	 }
	}

	if(SetPixelFormat(hDC,PixelFormat,&pixelDesc)==FALSE)

	{ 
	 return FALSE;
	}

	return TRUE;
}

bool TPOpenGL::CreateViewGLContext(HDC hDC)
{
	hrenderRC = wglCreateContext(hDC);

	if(hrenderRC==NULL)
	return FALSE;
	if(wglMakeCurrent(hDC,hrenderRC)==FALSE)
	return FALSE;
	return TRUE;
}

void TPOpenGL::RenderScene(HDC &hrenderDC)   
{	static GLfloat	rtri=0;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glTranslatef(0.0f,0.0f,-6.0f);      // Move Left 1.5 Units And Into The Screen 6.
	glRotatef(rtri,0.0f,1.0f,0.0f);
	glBegin(GL_TRIANGLES);								// Drawing Using Triangles
		glColor3f(1.0f,0.0f,0.0f);						// Set The Color To Red
		glVertex3f( 0.0f, 1.0f, 0.0f);					// Top
		glColor3f(0.0f,1.0f,0.0f);						// Set The Color To Green
		glVertex3f(-1.0f,-1.0f, 0.0f);					// Bottom Left
		glColor3f(0.0f,0.0f,1.0f);						// Set The Color To Blue
		glVertex3f( 1.0f,-1.0f, 0.0f);					// Bottom Right
	glEnd();
	rtri+=1.0f;
	SwapBuffers(hrenderDC);
}
/*
void TPOpenGL::testaxes()
{
	//…Ë÷√◊¯±Í÷·—’…´
	glColor3f(1.0f, 1.0f, 1.0f);
	//x÷·
	glBegin(GL_LINES);
	glVertex3f(-500,0.0,0.0);
	glVertex3f(500,0.0,0.0);
	glEnd();
	glBegin(GL_LINES);
	for(float i=-50;i<50;i++)
	{
	glVertex3f(i*10,0.0,0.0);
	glVertex3f(i*10,1.5,1.5);
	}
	glPushMatrix();                     


	//y÷·
	glBegin(GL_LINES);
	glVertex3f(0,-250,0.0);
	glVertex3f(0,200,0.0);
	glEnd();
	glBegin(GL_LINES);
	for(float i=-25.0;i<20.0;i++)
	{
	glVertex3f(0,i*10,0.0);
	glVertex3f(1.5,i*10,1.5);
	}
	glEnd();
	glPushMatrix();                       //ª≠º˝Õ∑
	glTranslatef(0.0f, 200.0f, 0.0f);
	glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
	//glutWireCone(5, 10, 100, 100);
	glPopMatrix();


	//z÷·
	glBegin(GL_LINES);
	glVertex3f(0,0.0,-250);
	glVertex3f(0,0.0,250);
	glEnd();
	glBegin(GL_LINES);
	for(float i=-25.0;i<25.0;i++)
	{
	glVertex3f(0,0.0,i*10);
	glVertex3f(1.5,1.5,i*10);
	}
	glEnd();
	glPushMatrix();                       //ª≠º˝Õ∑
	glTranslatef(0.0f, 0.0f, 245.0f);
	glRotatef(0.0f, 1.0f, 0.0f, 0.0f);
	//glutWireCone(5, 10, 100, 100);
	glPopMatrix();
}*/