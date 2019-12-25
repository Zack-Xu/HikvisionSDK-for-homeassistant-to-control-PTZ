 /*
* Copyright(C) 2011,Hikvision Digital Technology Co., Ltd 
* 
* File   name：main.cpp
* Discription：demo for muti thread get stream
* Version    ：1.0
* Author     ：luoyuhua
* Create Date：2011-12-10
* Modification History：
*/

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include "HCNetSDK.h"
#include "iniFile.h"
#include<getopt.h>





// 取流相关信息，用于线程传递
typedef struct tagREAL_PLAY_INFO
{
	char szIP[16];
	int iUserID;
	int iChannel;
}REAL_PLAY_INFO, *LPREAL_PLAY_INFO;

// 回调实时流，由于调用NET_DVR_SaveRealData，此处不处理
void g_RealDataCallBack_V30(LONG lRealHandle, DWORD dwDataType, BYTE *pBuffer,DWORD dwBufSize,void* dwUser)
{
	//LPREAL_PLAY_INFO pPlayInfo = (LPREAL_PLAY_INFO)dwUser;
    //printf("[g_RealDataCallBack_V30]Get data, ip=%s, channel=%d, handle=%d, data size is %d, thread=%d\n", 
		//pPlayInfo->szIP, pPlayInfo->iChannel, lRealHandle, dwBufSize, pthread_self());

	//printf("[g_RealDataCallBack_V30]Get data, handle=%d, data size is %d, thread=%d\n", 
		//lRealHandle, dwBufSize, pthread_self());
	//NET_DVR_SaveRealData(lRealHandle, cFilename);
}

FILE *g_pFile = NULL;

void PsDataCallBack(LONG lRealHandle, DWORD dwDataType,BYTE *pPacketBuffer,DWORD nPacketSize, void* pUser)
{

	// if (dwDataType  == NET_DVR_SYSHEAD)
	// {/	
		//写入头数据
		// g_pFile = fopen("./record/ps.dat", "wb");
		
		// if (g_pFile == NULL)
		// {
			// printf("CreateFileHead fail\n");
			// return;
		// }

		//写入头数据
		// fwrite(pPacketBuffer, sizeof(unsigned char), nPacketSize, g_pFile);
		// printf("write head len=%d\n", nPacketSize);
	// }
	// else
	// {
		// if(g_pFile != NULL)
		// {
			// fwrite(pPacketBuffer, sizeof(unsigned char), nPacketSize, g_pFile);
			// printf("write data len=%d\n", nPacketSize);
		// }
	// }	

}



int GetStream()
{
	// 从配置文件读取设备信息 
	IniFile ini("Device.ini");
	unsigned int dwSize = 0;
	char sSection[16] = "DEVICE";

	
	char *sIP = ini.readstring(sSection, "ip", "error", dwSize);
	int iPort = ini.readinteger(sSection, "port", 0);
	char *sUserName = ini.readstring(sSection, "username", "error", dwSize); 
	char *sPassword = ini.readstring(sSection, "password", "error", dwSize);
	int iChannel = ini.readinteger(sSection, "channel", 0);
		
	NET_DVR_DEVICEINFO_V30 struDeviceInfo;
	int iUserID = NET_DVR_Login_V30(sIP, iPort, sUserName, sPassword, &struDeviceInfo);
	if(iUserID >= 0)
	{

    		//NET_DVR_CLIENTINFO ClientInfo = {0};
    		//ClientInfo.lChannel     = iChannel;  //channel NO.
    		//ClientInfo.lLinkMode    = 0;
    		//ClientInfo.sMultiCastIP = NULL;
    		//int iRealPlayHandle = NET_DVR_RealPlay_V30(iUserID, &ClientInfo, PsDataCallBack, NULL, 0);
		NET_DVR_PREVIEWINFO struPreviewInfo = {0};
		struPreviewInfo.lChannel =iChannel;
		struPreviewInfo.dwStreamType = 0;
		struPreviewInfo.dwLinkMode = 0;
		struPreviewInfo.bBlocked = 1;
		struPreviewInfo.bPassbackRecord  = 1;
		int iRealPlayHandle = NET_DVR_RealPlay_V40(iUserID, &struPreviewInfo, PsDataCallBack, NULL);
		if(iRealPlayHandle >= 0)
		{
			printf("[GetStream]---RealPlay %s:%d success, \n", sIP, iChannel, NET_DVR_GetLastError());
			//int iRet = NET_DVR_SaveRealData(iRealPlayHandle, "./record/realplay.dat");
			//NET_DVR_SetStandardDataCallBack(iRealPlayHandle, StandardDataCallBack, 0);

		}
		else
		{
			printf("[GetStream]---RealPlay %s:%d failed, error = %d\n", sIP, iChannel, NET_DVR_GetLastError());
		}
	}
	else
	{
		printf("[GetStream]---Login %s failed, error = %d\n", sIP, NET_DVR_GetLastError());
	}
	return iUserID;
}


int main(int argc,char *argv[])
{	
	int dir;
	char *time;
	
    NET_DVR_Init();
	NET_DVR_SetLogToFile(3, "./record/");
	
	int UserID=GetStream();
	
	NET_DVR_PTZPOS m_struPtzCfg;
	DWORD dwReturn = 0;
	char nSize = sizeof(m_struPtzCfg);
	DWORD iLastErr = 0;
	unsigned char * str;
	
	// if (!NET_DVR_GetDVRConfig(UserID, NET_DVR_GET_PTZPOS, 1, &m_struPtzCfg, (DWORD)nSize, &dwReturn))
	// {
		// iLastErr = NET_DVR_GetLastError();
		// printf("NET_DVR_GetDVRConfig failed, error code= %d\n", iLastErr);
		// usleep(2000);
		//return 0;
	// }
    	// else
	// {
		// printf("catch ptz = %d，%d,%d \n",m_struPtzCfg.wPanPos,m_struPtzCfg.wTiltPos,m_struPtzCfg.wZoomPos);	
	// }
	printf(" Hikvision SDK to control the PTZ.  use - h for help\n");
	usleep(2000);
	int speed =1;
	while((dir = getopt(argc, argv, "u:d:l:r:s:abcegoph")) != -1)
	{
		switch(dir)
		{	
			case's':
				speed = atoi(optarg);
				break;
			case'u':
				time = optarg;
				NET_DVR_PTZControlWithSpeed_Other(UserID,1,TILT_UP,0,speed);
				usleep(1000*1000*atoi(time));
				NET_DVR_PTZControlWithSpeed_Other(UserID,1,TILT_UP,1,speed);
				break;
			case'd':
				time = optarg;
				NET_DVR_PTZControlWithSpeed_Other(UserID,1,TILT_DOWN,0,speed);
				usleep(1000*1000*atoi(time));
				NET_DVR_PTZControlWithSpeed_Other(UserID,1,TILT_DOWN,1,speed);
				break;
			case'l':
				time = optarg;
				NET_DVR_PTZControlWithSpeed_Other(UserID,1,PAN_LEFT,0,speed);
				usleep(1000*1000*atoi(time));
				NET_DVR_PTZControlWithSpeed_Other(UserID,1,PAN_LEFT,1,speed);
				break;
			case'r':
				time = optarg;
				NET_DVR_PTZControlWithSpeed_Other(UserID,1,PAN_RIGHT,0,speed);
				usleep(1000*1000*atoi(time));
				NET_DVR_PTZControlWithSpeed_Other(UserID,1,PAN_RIGHT,1,speed);
				break;
			case'a':

				NET_DVR_PTZControlWithSpeed_Other(UserID,1,TILT_DOWN,0,speed);
				break;
			case'b':

				NET_DVR_PTZControlWithSpeed_Other(UserID,1,TILT_UP,0,speed);
				break;
			case'c':

				NET_DVR_PTZControlWithSpeed_Other(UserID,1,PAN_LEFT,0,speed);
				break;
			case'e':

				NET_DVR_PTZControlWithSpeed_Other(UserID,1,PAN_RIGHT,0,speed);
				break;
			case'o':
				NET_DVR_PTZControlWithSpeed_Other(UserID,1,TILT_UP,1,speed);
				break;
			
			case'g':
				NET_DVR_PTZPreset_Other(UserID,1,GOTO_PRESET,1);
				usleep(2000);
				break;
			case'p':
				NET_DVR_PTZPreset_Other(UserID,1,SET_PRESET,1);
				break;
			case'h':
				printf(" - s x to change speed to x,x =1~9\n");
				printf(" - a to PTZ turn up without stop\n");
				printf(" - b to PTZ turn down without stop\n");
				printf(" - c to PTZ turn left without stop\n");
				printf(" - e to PTZ turn right without stop\n");
				printf(" - o to PTZ stop\n");
				printf(" - u x to PTZ turn up for x second \n");
				printf(" - d x to PTZ turn down for x second\n");
				printf(" - l x to PTZ turn left for x second\n");
				printf(" - r x to PTZ turn right for x second\n");
				break;
		}
	
		
	}
	
	//注销用户
	NET_DVR_Logout(UserID);
    NET_DVR_Cleanup();
    return 0;
}


