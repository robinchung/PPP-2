// PaperPlaneDlg.h : 头文件
//

#pragma once
#include "meterm1.h"
#include "meterm2.h"
#include "meterm3.h"
#include "meterm4.h"
#include "meterm5.h"
//#include "flyposture.h"
#include "ComOperating.h"
#include "TPOpenGL.h"

// CPaperPlaneDlg 对话框
class CPaperPlaneDlg : public CDialog
{
// 构造
public:
	CPaperPlaneDlg(CWnd* pParent = NULL);	// 标准构造函数
	~CPaperPlaneDlg()
	{	
		delete pComOperating;
		delete pTPOpenGL;
	};
// 对话框数据
	enum { IDD = IDD_PAPERPLANE_DIALOG };
	bool FullScreenCapture();//全屏截图
	afx_msg void OnFullScreenCapture();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
/*-----------------------------------------------------------------------------------------------*///仪表盘相关start
	MeterM1 meter_m1;
	MeterM2 meter_m2;
	MeterM3 meter_m3;
	MeterM4 meter_m4;
	MeterM5 meter_m5;
	void AllMeterInit();//初始化仪表盘指针
	void UpdateNeedleValue();//用于更新仪表盘指针指向
/*-----------------------------------------------------------------------------------------------*///飞行姿态相关
	//FlyPosture flyposture; 
public:
	bool OpenGLInit(); 
protected:
	HDC hrenderDC;
	TPOpenGL *pTPOpenGL;
/*-----------------------------------------------------------------------------------------------*///串口相关
public:
	void CheckCom();
	void FillComboxItems();
	afx_msg void OnBnClickedButtonSenddata();
	afx_msg void OnBnClickedButtonOpenport();
protected:
	ComOperating *pComOperating;
/*-----------------------------------------------------------------------------------------------*/
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);//响应定时器，主要用于测试
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	
};
