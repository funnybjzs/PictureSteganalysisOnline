#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#include "Common.h"
#include <iostream>
using namespace std;
#include "OraDBOpration.h"
#include "Config.h"
#define MSGHEADLEN 8 

bool GetHostIp(char *ip,uint32_t len,char *dev)
{
	char cmd[64];
	sprintf(cmd,"ifconfig %s | grep 'inet 地址' | awk -F ' *|:' '{print $4}'",dev);
	FILE *pp=popen(cmd,"r");
	if(!pp)
		return false;
	if(fgets(ip,len,pp)==NULL)
	{
		pclose(pp);
		return false;
	}
	pclose(pp);
	return true;
}
void *HandleFun(void *arg)
{
	pthread_detach(pthread_self());
	int acceptfd=*((int *)arg),ret;
	uint8_t msgheadBuf[MSGHEADLEN];
    uint32_t recvlen=0;
	uint32_t interfaceType;
	while((recvlen+=recv(acceptfd,msgheadBuf,MSGHEADLEN,0))!=MSGHEADLEN);
	interfaceType=ntohl(*(uint32_t *)(msgheadBuf));
	if(interfaceType!=CMDINTERFACE&&interfaceType!=CONFIGINTERFACE)
	{
		printf("接收指令或配置文件 接口类型错误\n");
		close(acceptfd);
		free(arg);
		return (void *)0;
	}
	switch(interfaceType)
	{
		case CMDINTERFACE:
			{
				uint32_t contentLen=ntohl(*(uint32_t *)(msgheadBuf+sizeof(uint32_t)));
				recvlen=0;
				uint8_t *contentBuf=(uint8_t *)malloc(sizeof(uint8_t)*contentLen);
				if(contentBuf==NULL)
				{
					fprintf(stderr,"File:%s,line:%d,func:%s,errno:%d,strerror:%s\n",__FILE__,__LINE__,__func__,errno,strerror(errno));
					return (void *)0;
				}
				while((recvlen+=recv(acceptfd,contentBuf,contentLen,0))!=contentLen);
				uint32_t feedbackflag=htonl(0x1);
				uint32_t cmd=ntohl(*(uint32_t *)contentBuf);
				if(cmd!=EQUSTART&&cmd!=EQUSTOP)
				{
					printf("指令内容错误\n");
					fprintf(stderr,"File:%s,line:%d,func:%s,errno:%d,strerror:%s\n",__FILE__,__LINE__,__func__,errno,strerror(errno));
					feedbackflag=htonl(0xf);
				}
				else
				{
					if(cmd==EQUSTART&&heartCondition==0)
					{
						heartCondition=1;
					}
					else if(cmd==EQUSTART&&heartCondition==1)
					{
						printf("服务正常状态 接收到启动指令\n");
						feedbackflag=htonl(0xf);
					}
					else if(cmd==EQUSTOP&&heartCondition==1)
					{
						heartCondition=0;
					}
					else if(cmd==EQUSTOP&&heartCondition==0)
					{
						printf("服务关闭状态 接收到关闭指令\n");
						feedbackflag=htonl(0xf);
					}
				}
				uint8_t username[30]={0};
				memcpy(username,contentBuf+4,30);
				free(contentBuf);
				uint8_t feedbackBuf[12]={0};
				uint32_t feedbackcontentlen=htonl(4+4);
				uint32_t cmdcpy=htonl(cmd);
				memcpy(feedbackBuf,&feedbackcontentlen,4);
				memcpy(feedbackBuf+4,&cmdcpy,4);
				memcpy(feedbackBuf+4+4,&feedbackflag,4);
				ret=send(acceptfd,feedbackBuf,12,0);
				if(ret<0)
				{
					fprintf(stderr,"File:%s,line:%d,func:%s,errno:%d,strerror:%s\n",__FILE__,__LINE__,__func__,errno,strerror(errno));
				}
				close(acceptfd);
				free(arg);
				char WebServerIp[32]={0};
				uint32_t WebServerPort;
				WebServerPort=GetPrivateProfileInt("WebServer","WebServerPort",0,"./config.ini");
				GetPrivateProfileString("WebServer","WebServerIp","",WebServerIp,32,"./config.ini");
				ret=SendHeartbeatOnce(WebServerIp,WebServerPort);
				if(1!=ret)
				{
					fprintf(stderr,"File:%s,line:%d,func:%s,errno:%d,strerror:%s\n",__FILE__,__LINE__,__func__,errno,strerror(errno));
				}
				//写入数据库
				char hostip[32]={0},Nicdev[16]={0};
				GetPrivateProfileString("HostInfo","NicDev","eth0",Nicdev,16,"./config.ini");
				if(!GetHostIp(hostip,32,Nicdev))
				{
					printf("无法获取本机IP地址 无法将指令信息写入数据库\n");
					return (void *)0;

				}
				char sql[256]={0},equType[4]={0};
				GetPrivateProfileString("HostInfo","EquType","01",equType,4,"./config.ini");
				OraDBOpration *ora=new OraDBOpration();
				if(ora->Init())
				{
					sprintf(sql,"insert into qt_oper_log values (oper_id_seq.nextval,'%s','%s','%s',(select sysdate from dual),'%s')",equType,hostip,cmd==EQUSTOP ? "0000":(cmd==EQUSTART ? "0001":"1111"),username);
					ora->ExecuteNonQuery(sql);
					delete ora;
				}
				else
				{
					delete ora;
				}
				break;
			}
		case CONFIGINTERFACE:
			{
				recvlen=0;
				char username[30]={0};
				uint32_t filelen=ntohl(*(uint32_t *)(msgheadBuf+sizeof(uint32_t)))-30;
				while((recvlen+=recv(acceptfd,username,30,0))!=30);
				char fileBuf[1024]={0};
				FILE *configfp=fopen("config","w");
				uint32_t totalsize=0;
				while(totalsize!=filelen)
				{
					recvlen=recv(acceptfd,fileBuf,1024,0);
					fwrite(fileBuf,1,recvlen,configfp);
					totalsize+=recvlen;
				}
				fclose(configfp);
				uint8_t feedbackBuf[8]={0};
				uint32_t feedbackcontentlen=htonl(4);
				uint32_t feedbackflag=htonl(0x1);
				memcpy(feedbackBuf,&feedbackcontentlen,4);
				memcpy(feedbackBuf+4,&feedbackflag,4);
				ret=send(acceptfd,feedbackBuf,8,0);
				if(ret<0)
				{
					fprintf(stderr,"File:%s,line:%d,func:%s,errno:%d,strerror:%s\n",__FILE__,__LINE__,__func__,errno,strerror(errno));
				}
				close(acceptfd);
				free(arg);
				//写入数据库
				char hostip[32]={0},Nicdev[16]={0};
				GetPrivateProfileString("HostInfo","NicDev","eth0",Nicdev,16,"./config.ini");
				if(!GetHostIp(hostip,32,Nicdev))
				{
					printf("无法获取本机IP地址 无法将指令信息写入数据库\n");
					return (void *)0;

				}
				char sql[256]={0},equType[4]={0};
				GetPrivateProfileString("HostInfo","EquType","01",equType,4,"./config.ini");
				OraDBOpration *ora=new OraDBOpration();
				if(ora->Init())
				{
					sprintf(sql,"insert into qt_oper_log values (oper_id_seq.nextval,'%s','%s','0010',(select sysdate from dual),'%s')",equType,hostip,username);
					ora->ExecuteNonQuery(sql);
					delete ora;
				}
				else
				{
					delete ora;
				}
				break;
			}
		default:
			break;

	}
	return (void *)0;

}
int RecvCmdAndConfig(int localport)
{
	int sockfd,ret,acceptfd;
	struct sockaddr_in localaddr={0},clientaddr;
	localaddr.sin_family=AF_INET;
	localaddr.sin_port=htons(localport);
	localaddr.sin_addr.s_addr=INADDR_ANY;
	sockfd=socket(AF_INET,SOCK_STREAM,0);
	if(sockfd<0)
	{
		fprintf(stderr,"File:%s,line:%d,func:%s,errno:%d,strerror:%s\n",__FILE__,__LINE__,__func__,errno,strerror(errno));
		return 0;
	}
	ret=bind(sockfd,(struct sockaddr *)&localaddr,sizeof(struct sockaddr));
	if(ret<0)
	{
		fprintf(stderr,"File:%s,line:%d,func:%s,errno:%d,strerror:%s\n",__FILE__,__LINE__,__func__,errno,strerror(errno));
		return 0;
	}
	ret=listen(sockfd,5);
	if(ret<0)
	{
		fprintf(stderr,"File:%s,line:%d,func:%s,errno:%d,strerror:%s\n",__FILE__,__LINE__,__func__,errno,strerror(errno));
		return 0;
	}
	while(1)
	{
		uint32_t tmplen=sizeof(struct sockaddr);
		acceptfd=accept(sockfd,(struct sockaddr *)&clientaddr,&tmplen);
		if(acceptfd<0)
		{
			fprintf(stderr,"File:%s,line:%d,func:%s,errno:%d,strerror:%s\n",__FILE__,__LINE__,__func__,errno,strerror(errno));
			return 0;
		}
		int *tmp=(int *)malloc(sizeof(int));
		*tmp=acceptfd;
		pthread_t t;
		ret=pthread_create(&t,NULL,HandleFun,tmp);
		if(ret!=0)
		{
			fprintf(stderr,"File:%s,line:%d,func:%s,errno:%d,strerror:%s\n",__FILE__,__LINE__,__func__,errno,strerror(errno));
			return 0;
		}

	}
	return 0;
}

