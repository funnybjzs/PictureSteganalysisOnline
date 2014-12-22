//============================================================================
// Name        : Steganalysis.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "TcpSocket.h"
#include "systemall.h"

#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include "Config.h"
#include "Common.h"
using namespace std;

void *SendHeartbeatHandleFun(void *arg)
{
	char WebServerIp[32]={0};
	uint32_t WebServerPort,HeartbeatInterval;
	WebServerPort=GetPrivateProfileInt("WebServer","WebServerPort",0,"./config.ini");
	HeartbeatInterval=GetPrivateProfileInt("Heartbeat","HeartbeatInterval",0,"./config.ini");
	GetPrivateProfileString("WebServer","WebServerIp","",WebServerIp,32,"./config.ini");
	bool ret;
	ret=SendHeartbeat(WebServerIp,WebServerPort,HeartbeatInterval);
	if(0==ret)
	{
		fprintf(stderr,"File:%s,line:%d,func:%s,errno:%d,strerror:%s\n",__FILE__,__LINE__,__func__,errno,strerror(errno));
	}
	return (void*)0;
}
void *RecvCmdAndConfigHandleFun(void * arg)
{
	RecvCmdAndConfig(CMDANDCONFIGPORT );
	return (void *) 0;
}

int main() {
	if(!(0==access("./config.ini",0)))
	{
		printf("请将配置文件config.ini与可执行文件放在同一目录下\n");
		return 0;
	}
	pthread_t heartbeat;
	int ret;
	ret=pthread_create(&heartbeat,NULL,SendHeartbeatHandleFun,NULL);
	if(ret!=0)
	{
		fprintf(stderr,"File:%s,line:%d,func:%s,errno:%d,strerror:%s\n",__FILE__,__LINE__,__func__,errno,strerror(errno));
		return 0;
	}
	pthread_t recvcmd;
	ret=pthread_create(&recvcmd,NULL,RecvCmdAndConfigHandleFun,NULL);
	if(ret!=0)
	{
		fprintf(stderr,"File:%s,line:%d,func:%s,errno:%d,strerror:%s\n",__FILE__,__LINE__,__func__,errno,strerror(errno));
		return 0;
	}


	try
	{

		InitSystemEnv();

	    TcpServer tcp(34890);
        while(true){
	    tcp.Start();
	    tcp.Stop();
        }

        tcp.Close();

       FreeSystemEnv();

	}
	catch (const char *msg)
	{
		cout<<msg;
	}
	return 0;
}


