#pragma once


// MeterM1
#ifndef ROUND
#define ROUND(x) (int)((x) + 0.5 - (double)((x) < 0))
#endif

#define BOUNDARY_POINTS 200
#define TABNUM 5

class MeterM1 : public CStatic
{
	DECLARE_DYNAMIC(MeterM1)

public:
	MeterM1();
	virtual ~MeterM1();

	void SetAngleRange(int nStartAngleDeg, int nEndAngleDeg);
	int m_nTabNum;
	void SetSubTicks(int nSubTicks);
	void SetTicks(int nTicks);
	void DrawValue(CDC *pDC);
	void SetColorTick(BOOL bColorTick = FALSE);
	BOOL m_bColorTick;
	void DrawNode(CDC *pDC);
	COLORREF m_colorTable[5];
	void SetValueDecimals(int nDecimals);
	void SetUnits(CString &strUnits);
	CString m_strUnits;
	int m_nValueDecimals;
	void SetScaleDecimals(int nDecimals);
	void SetRange(double dMin, double dMax);
	void SetNeedleColor (COLORREF colorNeedle);
	void UpdateNeedle(double dValue);
	COLORREF m_colorNeedle;
	int m_nScaleDecimals;
	
	double m_dCurrentValue;
	double m_dMaxValue;
	double m_dMinValue;

	void DrawNeedle(CDC *pDC);
	void ReconstructControl();
	void DrawMeterBackground(CDC *pDC, CRect &rect);
	int m_nStartAngleDeg;	// 仪表盘圆弧起始角度
	int m_nEndAngleDeg;		// 仪表盘圆弧终止角度
	int m_nTicks;			// 刻度数
	int m_nSubTicks;		// 分刻度数
protected:
	double m_dLeftAngleRad;
	double m_dRightAngleRad;
	int m_nCenterRadius;
	
	CRect m_rectCtrl;			// 控件区域
	CRect m_rectValue;			// 显示数值区域
	CRgn m_rgnBoundary;

	CBitmap *m_pBitmapOldBackground ;
	CBitmap m_bitmapBackground ;
	CDC m_dcBackground;

	CPoint m_ptMeterCenter;		// 仪表中心
	CPoint m_pointBoundary[BOUNDARY_POINTS]; // 边界点，用于绘制刻度

	CFont m_font;			// 显示文字字体

	COLORREF m_colorWindow;		// 背景色
	COLORREF m_colorHighlight;	
	COLORREF m_colorShadow;
	COLORREF m_colorButton;
	COLORREF m_colorText;		// 显示文本颜色

	int m_nRadiusFrame;			// 仪表盘边框半径
	//{{AFX_MSG(CMeter)
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


