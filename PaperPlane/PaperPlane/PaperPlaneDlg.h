// PaperPlaneDlg.h : ͷ�ļ�
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

// CPaperPlaneDlg �Ի���
class CPaperPlaneDlg : public CDialog
{
// ����
public:
	CPaperPlaneDlg(CWnd* pParent = NULL);	// ��׼���캯��
	~CPaperPlaneDlg()
	{	
		delete pComOperating;
		delete pTPOpenGL;
	};
// �Ի�������
	enum { IDD = IDD_PAPERPLANE_DIALOG };
	bool FullScreenCapture();//ȫ����ͼ
	afx_msg void OnFullScreenCapture();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
/*-----------------------------------------------------------------------------------------------*///�Ǳ������start
	MeterM1 meter_m1;
	MeterM2 meter_m2;
	MeterM3 meter_m3;
	MeterM4 meter_m4;
	MeterM5 meter_m5;
	void AllMeterInit();//��ʼ���Ǳ���ָ��
	void UpdateNeedleValue();//���ڸ����Ǳ���ָ��ָ��
/*-----------------------------------------------------------------------------------------------*///������̬���
	//FlyPosture flyposture; 
public:
	bool OpenGLInit(); 
protected:
	HDC hrenderDC;
	TPOpenGL *pTPOpenGL;
/*-----------------------------------------------------------------------------------------------*///�������
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

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);//��Ӧ��ʱ������Ҫ���ڲ���
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	
};
