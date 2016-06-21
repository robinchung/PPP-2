
/********************************************************************************
*																				*
*	Description:	be used for sending or receiving data from serial port		*
*	Author	   :	Yingmou.Zhong												*
*	Date	   :    2016-06-09													*
*																				*
*********************************************************************************/
#pragma once
#include <string>
#include <vector>
using namespace std;
#define ComCount 9
typedef struct
{
	DWORD ComNum;
	DWORD BaudRate;
	DWORD Parity;
	BYTE  ByteSize;
}SettingParam_t;

class ComOperating
{
public:
	ComOperating(void);
	~ComOperating(void);
	int GetCom(vector<string> *ComNameStr);
	void ComSetting(SettingParam_t *pSettingParam);
	void GetComSetting(DCB *dcb);
	long OpenCom(SettingParam_t *pSettingParam);
	long CloseCom();
	long SendData(BYTE *byteBuffer,long lngSize);
	long AcceptData(BYTE *byteBuffer,long lngSize);
	long ClearAcceptBuffer();
	long ClearSendBuffer();

private:
	HANDLE m_COM_Handle;
	DCB m_DCBPrama;
	OVERLAPPED m_OverlappedRead, m_OverlappedWrite;
};
