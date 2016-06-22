// PaperPlaneDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PaperPlane.h"
#include "PaperPlaneDlg.h"
#include "math.h"
//#include "ComOperating.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CPaperPlaneDlg �Ի���




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


// CPaperPlaneDlg ��Ϣ�������

BOOL CPaperPlaneDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	CMenu* pMenu = new CMenu;
	pMenu -> LoadMenu(IDR_MENU);
	AllMeterInit();//��ʼ�����������Ǳ���
	pTPOpenGL = new TPOpenGL(0);//ʵ����OpenGL��,����һ��Ҫ�ͷ��ڴ�����ڴ�й¶
	if(!OpenGLInit())//��ʼ��OpenGL
	{
		MessageBox("OpenGl��ʼ�����ɹ�","ERROR",MB_OK | MB_ICONINFORMATION);
		return false;
	}
	pComOperating = new ComOperating();//ʵ�������ڲ�����
	FillComboxItems();//���combox�ؼ���Ŀ
	SetTimer(1,10,NULL);//������ʱ�����ڲ���
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CPaperPlaneDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
//
HCURSOR CPaperPlaneDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CPaperPlaneDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	UpdateNeedleValue();//���ø����Ǳ���ָ��ָ������ֻ�ǲ���Ч��
#ifndef YMOPENGL
	pTPOpenGL->RenderScene(hrenderDC);
#endif
	CDialog::OnTimer(nIDEvent);
}

bool CPaperPlaneDlg::FullScreenCapture()
{
	CDC *pDC=GetDesktopWindow()->GetDC();//��ĻDC
    int BitPerPixel = pDC->GetDeviceCaps(BITSPIXEL);//�����ɫģʽ
    int Width = pDC->GetDeviceCaps(HORZRES);
    int Height = pDC->GetDeviceCaps(VERTRES);
    
    CDC memDC;//�ڴ�DC
    memDC.CreateCompatibleDC(pDC);
    
    CBitmap memBitmap, *oldmemBitmap;//��������Ļ���ݵ�bitmap
    memBitmap.CreateCompatibleBitmap(pDC, Width, Height);

    oldmemBitmap = memDC.SelectObject(&memBitmap);//��memBitmapѡ���ڴ�DC
    memDC.BitBlt(0, 0, Width, Height, pDC, 0, 0, SRCCOPY);//������Ļͼ���ڴ�DC

    //���´��뱣��memDC�е�λͼ���ļ�
    BITMAP bmp;
    memBitmap.GetBitmap(&bmp);//���λͼ��Ϣ
    
    FILE *fp = fopen("d:\\PaperPlane.jpg", "w+b");
	if(fp == NULL)
	{
		MessageBox("OpenGl��ʼ�����ɹ�","ERROR",MB_OK | MB_ICONINFORMATION);
		return 0;
	}
    BITMAPINFOHEADER bih = {0};//λͼ��Ϣͷ
    bih.biBitCount = bmp.bmBitsPixel;//ÿ�������ֽڴ�С
    bih.biCompression = BI_RGB;
    bih.biHeight = bmp.bmHeight;//�߶�
    bih.biPlanes = 1;
    bih.biSize = sizeof(BITMAPINFOHEADER);
    bih.biSizeImage = bmp.bmWidthBytes * bmp.bmHeight;//ͼ�����ݴ�С
    bih.biWidth = bmp.bmWidth;//���
    
    BITMAPFILEHEADER bfh = {0};//λͼ�ļ�ͷ
    bfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);//��λͼ���ݵ�ƫ����
    bfh.bfSize = bfh.bfOffBits + bmp.bmWidthBytes * bmp.bmHeight;//�ļ��ܵĴ�С
    bfh.bfType = (WORD)0x4d42;
    
    fwrite(&bfh, 1, sizeof(BITMAPFILEHEADER), fp);//д��λͼ�ļ�ͷ
    
    fwrite(&bih, 1, sizeof(BITMAPINFOHEADER), fp);//д��λͼ��Ϣͷ
    
    byte * p = new byte[bmp.bmWidthBytes * bmp.bmHeight];//�����ڴ汣��λͼ����

    GetDIBits(memDC.m_hDC, (HBITMAP) memBitmap.m_hObject, 0, Height, p, 
        (LPBITMAPINFO) &bih, DIB_RGB_COLORS);//��ȡλͼ����

    fwrite(p, 1, bmp.bmWidthBytes * bmp.bmHeight, fp);//д��λͼ����

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
	MessageBox("��ͼ�ɹ���������D��","�ɹ�",MB_OK | MB_ICONINFORMATION);
}
void CPaperPlaneDlg::AllMeterInit() //�ڴ˴���ʼ�������Ǳ���ָ��
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

void CPaperPlaneDlg::UpdateNeedleValue()//�ڴ˴������Ǳ���ָ��ָ����ֵ
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
	CheckCom();//���com��
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
