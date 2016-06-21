#pragma once


// FlyPosture

class FlyPosture : public CStatic
{
	DECLARE_DYNAMIC(FlyPosture)

public:
	FlyPosture();
	virtual ~FlyPosture();
	void DrawAxis(CDC *pDC,CRect &rect);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
protected:
	CDC m_dcBackground;
	CBitmap m_bmBackground;
	CBitmap* m_bmOldBackground;
	CRect m_rectCtrl;
	CPoint left,right;
	DECLARE_MESSAGE_MAP()
};


