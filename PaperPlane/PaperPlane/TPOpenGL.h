/********************************************************************************
*																				*
*	Description:	be used for sending or receiving data from serial port		*
*	Author	   :	Yingmou.Zhong												*
*	Date	   :    2016-06-20													*
*																				*
*********************************************************************************/
#pragma once
#include "gl/gl.h"
#include "gl/glu.h"
#include "gl/glaux.h"

class TPOpenGL
{
public:
	TPOpenGL(int p):PixelFormat(p){};
	~TPOpenGL(void){};
	int InitGL(GLvoid);
	bool SetWindowPixelFormat(HDC hDC);
	bool CreateViewGLContext(HDC hDC);
	void RenderScene(HDC &hrenderDC);
private:

	HGLRC hrenderRC;
	int PixelFormat;
};

