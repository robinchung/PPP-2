// FlyPosture.cpp : implementation file
//

#include "stdafx.h"
#include "PaperPlane.h"
#include "FlyPosture.h"
#include "MemDC.h"

// FlyPosture

IMPLEMENT_DYNAMIC(FlyPosture, CStatic)

FlyPosture::FlyPosture()
{

}

FlyPosture::~FlyPosture()
{
}


BEGIN_MESSAGE_MAP(FlyPosture, CStatic)
	ON_WM_PAINT()
	ON_WM_SIZE()
END_MESSAGE_MAP()

// FlyPosture message handlers


void FlyPosture::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CStatic::OnPaint() for painting messages
	GetClientRect (&m_rectCtrl);
	CMemDC memDC(&dc, &m_rectCtrl);
	if(m_dcBackground.GetSafeHdc()== NULL|| (m_bmBackground.m_hObject == NULL))
	{
		m_dcBackground.CreateCompatibleDC(&dc);
		m_bmBackground.CreateCompatibleBitmap(&dc, m_rectCtrl.Width(), m_rectCtrl.Height()) ;
		//m_dcBackground.FillSolidRect(m_rectCtrl,RGB(0,0,0));
		m_bmOldBackground = m_dcBackground.SelectObject(&m_bmBackground) ;
		DrawAxis(&m_dcBackground,m_rectCtrl);
	}
	memDC.BitBlt(0, 0, m_rectCtrl.Width(), m_rectCtrl.Height(), 
		&m_dcBackground, 0, 0, SRCCOPY) ;
}

void FlyPosture::OnSize(UINT nType, int cx, int cy)
{
	CStatic::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
}

void FlyPosture::DrawAxis(CDC *pDC,CRect &rect)
{
	CRgn rgn;
	POINT topleft,bottomright;
	topleft = rect.TopLeft();
	bottomright = rect.BottomRight();
	POINT pt1[4] = {{topleft.x,topleft.y},{bottomright.x/5,bottomright.y/5},{bottomright.x/5,bottomright.y*4/5},{topleft.x,bottomright.y}};
	POINT pt2[4] = {{bottomright.x/5,bottomright.y/5},{bottomright.x*4/5,bottomright.y/5},{bottomright.x*4/5,bottomright.y*4/5},{bottomright.x/5,bottomright.y*4/5}};
	POINT pt3[4] = {{topleft.x,bottomright.y},{bottomright.x/5,bottomright.y*4/5},{bottomright.x*4/5,bottomright.y*4/5},{bottomright.x,bottomright.y}};
	BYTE style[4] = {{ PT_MOVETO},{PT_LINETO},{PT_LINETO},{PT_LINETO}}; 
	CBrush brush(RGB(60,179,113));
	CBrush brush2(RGB(255,255,255));
	pDC->FillRect(rect,&brush2);
	pDC->SelectObject(&brush);
	pDC->Polygon(pt1,4);
	pDC->Polygon(pt2,4);
	pDC->Polygon(pt3,4);
}