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



FILE *g_pFile = NULL;




int GetUserID(int iPort,char *sIP,char *sUserName,char *sPassword,int iChannel)
{


	NET_DVR_DEVICEINFO_V30 struDeviceInfo;
	int iUserID = NET_DVR_Login_V30(sIP, iPort, sUserName, sPassword, &struDeviceInfo);
	return iUserID;
}


int main(int argc,char *argv[])
{	
	printf("PTZ Control using SDK demo \n type ./hikvision -h to load help list\n normal use like this ./hikvision -I 192.168.1.22 -p 8000 -U admin -P 12345 -u 1\n");
	char dir;
	char *time;
	char arg;
	int speed =1;
	int Port=8000;
	char *IP="0.0.0.0";
	char *UserName= "admin";
	char *Password= "12345";
	int Channel =1;
	while((arg = getopt(argc, argv, "u:d:l:r:s:p:I:U:P:C:abcegoqh")) != -1)
	{
		switch(arg)
		{	
			case'I':
				IP = optarg;
				break;
			case'p':
				Port =atoi(optarg);
				break;
			case'U':
				UserName = optarg;
				break;
			case'P':
				Password = optarg;
				break;
			case'C':
				Channel = atoi(optarg);
				break;
			case's':
				speed = atoi(optarg);
				break;
			case'u':
				time = optarg;
				dir = 'u';
				break;
			case'd':
				time = optarg;
				dir = 'd';
				break;
			case'l':
				time = optarg;
				dir = 'l';
				break;
			case'r':
				time = optarg;
				dir = 'r';
				break;
			case'a':
				dir = 'a';
				break;
			case'b':
				dir = 'b';
				break;
			case'c':
				dir = 'c';
				break;
			case'e':
				dir = 'e';
				break;
			case'o':
				dir = 'u';
				break;
			case'g':
				dir = 'g';
				break;
			case'q':
				dir = 'q';
				break;
			case'h':
				
				printf(" - s x to change speed to x,x =1~9\n");
				printf(" - a to PTZ turn up without stop\n");
				printf(" - b to PTZ turn down without stop\n");
				printf(" - c to PTZ turn left without stop\n");
				printf(" - e to PTZ turn right without stop\n");
				printf(" - o to PTZ stop\n");
				printf(" - u x to PTZ turn up for x seconds \n");
				printf(" - d x to PTZ turn down for x seconds\n");
				printf(" - l x to PTZ turn left for x seconds\n");
				printf(" - r x to PTZ turn right for x seconds\n");
				break;
		}
	
		
	}
	
    NET_DVR_Init();
	//NET_DVR_SetLogToFile(3, "./record/");
	int UserID=GetUserID(Port,IP,UserName,Password,Channel);
	NET_DVR_PTZPOS m_struPtzCfg;
	DWORD dwReturn = 0;
	char nSize = sizeof(m_struPtzCfg);
	DWORD iLastErr = 0;
	unsigned char * str;
	usleep(2000);
	
	switch(dir)
		{	
			case'u':
				NET_DVR_PTZControlWithSpeed_Other(UserID,1,TILT_UP,0,speed);
				usleep(1000*1000*atoi(time));
				NET_DVR_PTZControlWithSpeed_Other(UserID,1,TILT_UP,1,speed);
				break;
			case'd':
				NET_DVR_PTZControlWithSpeed_Other(UserID,1,TILT_DOWN,0,speed);
				usleep(1000*1000*atoi(time));
				NET_DVR_PTZControlWithSpeed_Other(UserID,1,TILT_DOWN,1,speed);
				break;
			case'l':
				NET_DVR_PTZControlWithSpeed_Other(UserID,1,PAN_LEFT,0,speed);
				usleep(1000*1000*atoi(time));
				NET_DVR_PTZControlWithSpeed_Other(UserID,1,PAN_LEFT,1,speed);
				break;
			case'r':
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
			case'q':
				NET_DVR_PTZPreset_Other(UserID,1,SET_PRESET,1);
				break;
		}
	
	
	//注销用户
	NET_DVR_Logout(UserID);
    NET_DVR_Cleanup();
    return 0;
}


