
/********************************************************************************
*																				*
*	Description:	be used for sending or receiving data from serial port		*
*	Author	   :	Yingmou.Zhong												*
*	Date	   :    2016-06-09													*
*																				*
*********************************************************************************/
#include "StdAfx.h"
#include "ComOperating.h"

ComOperating::ComOperating(void)
{
	m_COM_Handle = NULL;
	memset(&m_DCBPrama,0,sizeof(m_DCBPrama));
	memset(&m_OverlappedRead,0,sizeof(m_OverlappedRead));
	memset(&m_OverlappedWrite,0,sizeof(m_OverlappedWrite));
}

ComOperating::~ComOperating(void)
{
}

int ComOperating::GetCom(vector<string> *ComNameStr)
{
	HANDLE hCom;
	char Temp[255];//血的教训，数组一定要足够大否则容易造成指针访问越界
	int FlagNum = 0;
	for(int count=1;count<=9;count++)//逐个检测com口是否能打开
	{
		sprintf(Temp,"COM%d",count);
		hCom = CreateFile(Temp,0,0,0,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0);
		if(INVALID_HANDLE_VALUE != hCom)
		{
			CloseHandle(hCom);
			(*ComNameStr).push_back(Temp);
			FlagNum = FlagNum+1;
		}
	}
	if(!FlagNum)
		return false;
	return FlagNum;
}

void ComOperating::ComSetting(SettingParam_t *pSettingParam)//对外只提供四个参数进行设置分别是：端口号、波特率、奇偶校验、数据位
{

	m_DCBPrama.DCBlength = sizeof(DCB);
	if(!GetCommState(m_COM_Handle,&m_DCBPrama))
		GetLastError();
	m_DCBPrama.fBinary = true;
	m_DCBPrama.fOutxCtsFlow = false;
	m_DCBPrama.fOutxDsrFlow = false;
	m_DCBPrama.fDtrControl = DTR_CONTROL_DISABLE;
	m_DCBPrama.fDsrSensitivity = false;
	m_DCBPrama.fTXContinueOnXoff = true;
	m_DCBPrama.fOutX = false;
	m_DCBPrama.fInX = false;
	m_DCBPrama.fErrorChar = false;
	m_DCBPrama.fNull = false;
	m_DCBPrama.fRtsControl = RTS_CONTROL_ENABLE;
	m_DCBPrama.fAbortOnError = false;
	m_DCBPrama.fDummy2 = 0;
	//m_DCBPrama.wReserved = 0;//wReserved
	m_DCBPrama.XonLim = 0;
	m_DCBPrama.XoffLim = 0;
	m_DCBPrama.XonChar = 0;
	m_DCBPrama.XoffChar = 0;
	m_DCBPrama.ErrorChar = 0;
	m_DCBPrama.EofChar = 0;
	m_DCBPrama.EvtChar = 0;
	m_DCBPrama.wReserved1 = 0;
	m_DCBPrama.BaudRate = pSettingParam->BaudRate;
	m_DCBPrama.Parity = pSettingParam->Parity;
	m_DCBPrama.ByteSize = pSettingParam->ByteSize;
	m_DCBPrama.StopBits =0;
	if(m_DCBPrama.Parity == 0)
	{
		m_DCBPrama.fParity = false;
	}
	else
	{
		m_DCBPrama.fParity = true;
	}

}

void ComOperating::GetComSetting(DCB *dcb)
{
	*dcb = m_DCBPrama;
}


long ComOperating::OpenCom(SettingParam_t *pSettingParam)
{
	DCB dcb;
	long InBufferSize = 2048,OutBufferSize = 2048;//设置缓冲区buffer大小还需要调试
	char szMsg[255];
	if(pSettingParam->ComNum>9)
		sprintf(szMsg,"////.//COM%d",pSettingParam->ComNum);
	else
		sprintf(szMsg,"COM%d",pSettingParam->ComNum);
	m_COM_Handle = CreateFile(szMsg,GENERIC_READ | GENERIC_WRITE,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,NULL);
	if(m_COM_Handle == NULL)
		return (-1);
	//读写超时设置
	COMMTIMEOUTS CommTimeOuts;
	CommTimeOuts.ReadIntervalTimeout = 0;
	CommTimeOuts.ReadTotalTimeoutMultiplier = 0;
	CommTimeOuts.ReadTotalTimeoutConstant = 2500;
	CommTimeOuts.WriteTotalTimeoutMultiplier = 0;
	CommTimeOuts.WriteTotalTimeoutConstant = 2500;
	SetCommTimeouts(m_COM_Handle,&CommTimeOuts);
	m_OverlappedRead.hEvent = CreateEvent(NULL,TRUE,FALSE,NULL);
	m_OverlappedWrite.hEvent = CreateEvent(NULL,TRUE,FALSE,NULL);
	GetComSetting(&dcb);
	if(!SetCommState(m_COM_Handle,&dcb) || !SetupComm(m_COM_Handle,InBufferSize,OutBufferSize) ||
		m_OverlappedRead.hEvent == NULL || m_OverlappedWrite.hEvent == NULL)
	{
		DWORD dwError = GetLastError();
		if(m_OverlappedRead.hEvent != NULL) CloseHandle(m_OverlappedRead.hEvent);
		if(m_OverlappedWrite.hEvent != NULL) CloseHandle(m_OverlappedWrite.hEvent);
		CloseHandle(m_COM_Handle);
		m_COM_Handle == NULL;
		return dwError;
	}
	return 1;
}

long ComOperating:: CloseCom()
{
	if(m_COM_Handle == NULL )
		return( 1 );
	SetCommMask(m_COM_Handle ,NULL);
	SetEvent(m_OverlappedRead.hEvent);
	SetEvent(m_OverlappedWrite.hEvent);
	if( m_OverlappedRead.hEvent != NULL ) 
		CloseHandle( m_OverlappedRead.hEvent );
	if( m_OverlappedWrite.hEvent != NULL )
		CloseHandle( m_OverlappedWrite.hEvent );
	if (CloseHandle( m_COM_Handle )==FALSE)
		return (2);
	m_COM_Handle = NULL;
	return( 0 );
}

long ComOperating:: SendData(BYTE *byteBuffer,long lngSize)
{
	DWORD dwBytesWritten=lngSize;
	bool bWriteStat;
	COMSTAT ComStat;
	DWORD dwErrorFlags;
	if( m_COM_Handle == NULL ) return( -1 );
	ClearCommError(m_COM_Handle,&dwErrorFlags,&ComStat);
	bWriteStat=WriteFile(m_COM_Handle, byteBuffer, lngSize, &dwBytesWritten, &(m_OverlappedWrite));
	if(!bWriteStat)
	{
		if(GetLastError() == ERROR_IO_PENDING)
		{
			GetOverlappedResult(m_COM_Handle,&(m_OverlappedWrite),&dwBytesWritten,TRUE); //等待直到发送完毕
		}
		else
		{
			dwBytesWritten=0;
		}
	}
	 return (long)dwBytesWritten;
}

long ComOperating::AcceptData(BYTE *byteBuffer,long lngSize)
{
	DWORD lngBytesRead=lngSize;
	bool fReadStat;
	DWORD dwRes=0;
	if( m_COM_Handle == NULL )
	  return( -1 );
	fReadStat=ReadFile(m_COM_Handle,byteBuffer,lngSize,&lngBytesRead,&(m_OverlappedRead)); 
	//Sleep(1);
	if( !fReadStat )
	{
		if( GetLastError() == ERROR_IO_PENDING ) //重叠 I/O 操作在进行中 
		{
			dwRes=WaitForSingleObject(m_OverlappedRead.hEvent,1000); //等待，直到超时
			switch(dwRes)
			{
			case WAIT_OBJECT_0: //读完成 
				if(GetOverlappedResult(m_COM_Handle,&(m_OverlappedRead),&lngBytesRead,FALSE)==0)
				{
				//错误
				return -2;
				}
			break;
			case WAIT_TIMEOUT: //超时
			return -1;
			break;
			default: //WaitForSingleObject 错误
			break;
			}
		}
	}
	return lngBytesRead; 
}

long ComOperating::ClearAcceptBuffer()
{
	if(m_COM_Handle == NULL )
		return( -1 );
	else
		PurgeComm(m_COM_Handle,PURGE_RXABORT | PURGE_RXCLEAR);
	return 1;
}

long ComOperating::ClearSendBuffer()
{
	if(m_COM_Handle == NULL )
	return( -1 );
	else
	PurgeComm(m_COM_Handle,PURGE_TXABORT | PURGE_TXCLEAR);
	return 1;
}