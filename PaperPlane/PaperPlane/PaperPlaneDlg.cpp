// PaperPlaneDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PaperPlane.h"
#include "PaperPlaneDlg.h"
#include "math.h"
//#include "ComOperating.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CPaperPlaneDlg 对话框




CPaperPlaneDlg::CPaperPlaneDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPaperPlaneDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPaperPlaneDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_METER1, meter_m1);
	DDX_Control(pDX, IDC_STATIC_METER2, meter_m2);
	DDX_Control(pDX, IDC_STATIC_METER3, meter_m3);
	DDX_Control(pDX, IDC_STATIC_METER4, meter_m4);
	DDX_Control(pDX, IDC_STATIC_METER_BATTERY, meter_m5);
	//DDX_Control(pDX, IDC_FLY, flyposture);
}

BEGIN_MESSAGE_MAP(CPaperPlaneDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_TIMER()
	ON_COMMAND(ID_32771, &CPaperPlaneDlg::OnFullScreenCapture)
	ON_BN_CLICKED(IDC_BUTTON_SENDDATA, &CPaperPlaneDlg::OnBnClickedButtonSenddata)
	ON_BN_CLICKED(IDC_BUTTON_OPENPORT, &CPaperPlaneDlg::OnBnClickedButtonOpenport)
END_MESSAGE_MAP()


// CPaperPlaneDlg 消息处理程序

BOOL CPaperPlaneDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	CMenu* pMenu = new CMenu;
	pMenu -> LoadMenu(IDR_MENU);
	AllMeterInit();//初始化画出所有仪表盘
	pTPOpenGL = new TPOpenGL(0);//实例化OpenGL类,后面一定要释放内存否则内存泄露
	if(!OpenGLInit())//初始化OpenGL
	{
		MessageBox("OpenGl初始化不成功","ERROR",MB_OK | MB_ICONINFORMATION);
		return false;
	}
	pComOperating = new ComOperating();//实例化串口操作类
	FillComboxItems();//填充combox控件条目
	SetTimer(1,10,NULL);//启动定时器用于测试
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CPaperPlaneDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CPaperPlaneDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
//
HCURSOR CPaperPlaneDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CPaperPlaneDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	UpdateNeedleValue();//调用更新仪表盘指针指向，现在只是测试效果
#ifndef YMOPENGL
	pTPOpenGL->RenderScene(hrenderDC);
#endif
	CDialog::OnTimer(nIDEvent);
}

bool CPaperPlaneDlg::FullScreenCapture()
{
	CDC *pDC=GetDesktopWindow()->GetDC();//屏幕DC
    int BitPerPixel = pDC->GetDeviceCaps(BITSPIXEL);//获得颜色模式
    int Width = pDC->GetDeviceCaps(HORZRES);
    int Height = pDC->GetDeviceCaps(VERTRES);
    
    CDC memDC;//内存DC
    memDC.CreateCompatibleDC(pDC);
    
    CBitmap memBitmap, *oldmemBitmap;//建立和屏幕兼容的bitmap
    memBitmap.CreateCompatibleBitmap(pDC, Width, Height);

    oldmemBitmap = memDC.SelectObject(&memBitmap);//将memBitmap选入内存DC
    memDC.BitBlt(0, 0, Width, Height, pDC, 0, 0, SRCCOPY);//复制屏幕图像到内存DC

    //以下代码保存memDC中的位图到文件
    BITMAP bmp;
    memBitmap.GetBitmap(&bmp);//获得位图信息
    
    FILE *fp = fopen("d:\\PaperPlane.jpg", "w+b");
	if(fp == NULL)
	{
		MessageBox("OpenGl初始化不成功","ERROR",MB_OK | MB_ICONINFORMATION);
		return 0;
	}
    BITMAPINFOHEADER bih = {0};//位图信息头
    bih.biBitCount = bmp.bmBitsPixel;//每个像素字节大小
    bih.biCompression = BI_RGB;
    bih.biHeight = bmp.bmHeight;//高度
    bih.biPlanes = 1;
    bih.biSize = sizeof(BITMAPINFOHEADER);
    bih.biSizeImage = bmp.bmWidthBytes * bmp.bmHeight;//图像数据大小
    bih.biWidth = bmp.bmWidth;//宽度
    
    BITMAPFILEHEADER bfh = {0};//位图文件头
    bfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);//到位图数据的偏移量
    bfh.bfSize = bfh.bfOffBits + bmp.bmWidthBytes * bmp.bmHeight;//文件总的大小
    bfh.bfType = (WORD)0x4d42;
    
    fwrite(&bfh, 1, sizeof(BITMAPFILEHEADER), fp);//写入位图文件头
    
    fwrite(&bih, 1, sizeof(BITMAPINFOHEADER), fp);//写入位图信息头
    
    byte * p = new byte[bmp.bmWidthBytes * bmp.bmHeight];//申请内存保存位图数据

    GetDIBits(memDC.m_hDC, (HBITMAP) memBitmap.m_hObject, 0, Height, p, 
        (LPBITMAPINFO) &bih, DIB_RGB_COLORS);//获取位图数据

    fwrite(p, 1, bmp.bmWidthBytes * bmp.bmHeight, fp);//写入位图数据

    delete [] p;

    fclose(fp);

    memDC.SelectObject(oldmemBitmap);
	memDC.DeleteDC();
	ReleaseDC(pDC);
	return 1;
}

void CPaperPlaneDlg::OnFullScreenCapture()
{
	// TODO: Add your command handler code here
	if(FullScreenCapture())
	MessageBox("截图成功，保存在D盘","成功",MB_OK | MB_ICONINFORMATION);
}
void CPaperPlaneDlg::AllMeterInit() //在此处初始化所有仪表盘指针
{
	COLORREF m_colorNeedle;
	m_colorNeedle = RGB(255, 255, 255);//etSysColor(COLOR_ACTIVEBORDER);
	meter_m1.SetNeedleColor(m_colorNeedle);
	meter_m1.SetColorTick(TRUE);
	meter_m2.SetNeedleColor(m_colorNeedle);
	meter_m2.SetColorTick(TRUE);
	meter_m3.SetNeedleColor(m_colorNeedle);
	meter_m3.SetColorTick(TRUE);
	meter_m4.SetNeedleColor(m_colorNeedle);
	meter_m4.SetColorTick(TRUE);
	meter_m5.SetNeedleColor(m_colorNeedle);
	meter_m5.SetColorTick(TRUE);
}

void CPaperPlaneDlg::UpdateNeedleValue()//在此处更新仪表盘指针指向数值
{
	static double dMStep = 0.5 ;
	static double dMValue = 0.0 ;

	dMValue += dMStep ;
	if (dMValue > meter_m1.m_dMaxValue)
	{
		dMStep = -fabs(dMStep) ;
		dMValue = meter_m1.m_dMaxValue-dMStep ;
	}
	else if (dMValue < meter_m1.m_dMinValue)
	{	
		dMStep = fabs(dMStep) ;
		dMValue = meter_m1.m_dMinValue+dMStep ;
	}
	meter_m1.UpdateNeedle(dMValue) ;
	meter_m2.UpdateNeedle(dMValue) ;
	meter_m3.UpdateNeedle(dMValue) ;
	meter_m4.UpdateNeedle(dMValue) ;
	meter_m5.UpdateNeedle(dMValue) ;

}

bool CPaperPlaneDlg::OpenGLInit()
{
	CWnd *wnd=GetDlgItem(IDC_FLY);
	hrenderDC=::GetDC(wnd->m_hWnd);
	if(pTPOpenGL->SetWindowPixelFormat(hrenderDC)==FALSE)
	return 0;
	if(pTPOpenGL->CreateViewGLContext(hrenderDC)==FALSE)
	return 0;
	pTPOpenGL->InitGL();
	return 1;
}

void CPaperPlaneDlg::CheckCom()
{
	string strtemp;
	int realCount,Count=0;
	vector<string>ComNameStr;
	realCount = pComOperating->GetCom(&ComNameStr);
	if(realCount != 0)
	{
		for(Count;Count<realCount;Count++)
		{
		strtemp = ComNameStr[Count];
		((CComboBox *)GetDlgItem(IDC_COMBO_COM))->AddString(strtemp.c_str());
		}
		((CComboBox *)GetDlgItem(IDC_COMBO_COM))->SetCurSel(0);
		strtemp.clear();
	}
	else((CComboBox *)GetDlgItem(IDC_COMBO_COM))->EnableWindow(false);
}

void CPaperPlaneDlg::FillComboxItems()
{
	CheckCom();//检测com口
	((CComboBox *)GetDlgItem(IDC_COMBO_BAUDRATE))->AddString("115200");
	((CComboBox *)GetDlgItem(IDC_COMBO_BAUDRATE))->AddString("9600");
	((CComboBox *)GetDlgItem(IDC_COMBO_BAUDRATE))->SetCurSel(1);
	((CComboBox *)GetDlgItem(IDC_COMBO_PARITY))->AddString("Odd");
	((CComboBox *)GetDlgItem(IDC_COMBO_PARITY))->AddString("Even");
	((CComboBox *)GetDlgItem(IDC_COMBO_PARITY))->AddString("Mark");
	((CComboBox *)GetDlgItem(IDC_COMBO_PARITY))->AddString("None");
	((CComboBox *)GetDlgItem(IDC_COMBO_PARITY))->SetCurSel(2);
	((CComboBox *)GetDlgItem(IDC_COMBO_BYTESIZE))->AddString("8");
	((CComboBox *)GetDlgItem(IDC_COMBO_BYTESIZE))->SetCurSel(0);
}

void CPaperPlaneDlg::OnBnClickedButtonSenddata()
{
	// TODO: Add your control notification handler code here
	CEdit *pbox;
	pbox = (CEdit*)GetDlgItem(IDC_BUTTON_SENDDATA);
	CString str;
	pbox->GetWindowTextA(str);
	BYTE *strsend;
	strsend = (BYTE*)str.GetBuffer(str.GetLength());
	pComOperating->SendData(strsend,2);
}

void CPaperPlaneDlg::OnBnClickedButtonOpenport()
{
	// TODO: Add your control notification handler code here
		// TODO: Add your control notification handler code here
	SettingParam_t params;
	CString ComStr,PortStr;
	GetDlgItem(IDC_BUTTON_OPENPORT_CLOSEPORT)->GetWindowText(PortStr);
	if(PortStr == "OpenPort")
	{
		((CComboBox *)GetDlgItem(IDC_COMBO_COM))->GetLBText(((CComboBox *)GetDlgItem(IDC_COMBO_COM))->GetCurSel(),ComStr);
		if(ComStr == "COM1")
			params.ComNum = 1;
		else if(ComStr == "COM2")
			params.ComNum = 2;
		else if(ComStr == "COM3")
			params.ComNum = 3;
		else if(ComStr == "COM4")
			params.ComNum = 4;
		else if(ComStr == "COM5")
			params.ComNum = 5;
		else if(ComStr == "COM6")
			params.ComNum = 6;
		else if(ComStr == "COM7")
			params.ComNum = 7;
		else if(ComStr == "COM8")
			params.ComNum = 8;
		else if(ComStr == "COM9")
			params.ComNum = 9;
		((CComboBox *)GetDlgItem(IDC_COMBO_BAUDRATE))->GetLBText(((CComboBox *)GetDlgItem(IDC_COMBO_BAUDRATE))->GetCurSel(),ComStr);
		if(ComStr == "9600")
			params.BaudRate = 9600;
		else
			params.BaudRate = 115200;
		((CComboBox *)GetDlgItem(IDC_COMBO_PARITY))->GetLBText(((CComboBox *)GetDlgItem(IDC_COMBO_PARITY))->GetCurSel(),ComStr);
		if(ComStr == "None") 
			params.Parity = 0;
		else if(ComStr == "Odd")
			params.Parity = 1;
		else if(ComStr == "Even") 
			params.Parity = 2;
		else if(ComStr == "Mark") 
			params.Parity = 3;
		params.ByteSize = 8;
		pComOperating->ComSetting(&params);
		pComOperating->OpenCom(&params);
		GetDlgItem(IDC_BUTTON_OPENPORT_CLOSEPORT)->SetWindowText(_T("ClosePort"));
	}
	else
	{
		pComOperating->CloseCom();
		GetDlgItem(IDC_BUTTON_OPENPORT_CLOSEPORT)->SetWindowText(_T("OpenPort"));
	}
	
}
