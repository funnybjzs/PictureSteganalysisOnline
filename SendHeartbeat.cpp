#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "Common.h"

volatile bool heartCondition=1;

uint32_t GetHeartbeatInfo(uint8_t **buf)
{
	uint32_t interfaceType=htonl(HEARTBEATINTERFACE);
	uint32_t heartbeat;
	uint32_t contentLen=htonl(sizeof(heartbeat));
	uint32_t bufLen=0;
	uint32_t offset=0;
	if(heartCondition)
	  heartbeat=htonl(HEARTSERVICERUN);
	else
	  heartbeat=htonl(HEARTSERVICECLOSE);
	bufLen=sizeof(uint8_t)*(sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint32_t));
	*buf=(uint8_t *)malloc(bufLen);
	if(*buf==NULL)
	{
		printf("heartbeatBuf malloc error\n");
		fprintf(stderr,"File:%s,line:%d,func:%s,errno:%d,strerror:%s\n",__FILE__,(int)__LINE__,__func__,errno,strerror(errno));
		return 0;
	}
	memset(*buf,0,bufLen);
	memcpy(*buf+offset,&interfaceType,sizeof(interfaceType));
	offset+=sizeof(interfaceType);
	memcpy(*buf+offset,&contentLen,sizeof(contentLen));
	offset+=sizeof(contentLen);
	memcpy(*buf+offset,&heartbeat,sizeof(heartbeat));
	return bufLen;
}
bool SendHeartbeat(char *serverip,uint32_t serverport,uint32_t interval)
{
	int socketfd,ret;
	struct sockaddr_in serveraddr={0};
	uint8_t *heartbeatBuf;
	uint32_t heartbeatBuflen=0;
	serveraddr.sin_family=AF_INET;
	serveraddr.sin_port=htons(serverport);
	serveraddr.sin_addr.s_addr=inet_addr(serverip);
	while(1)
	{
		socketfd=socket(AF_INET,SOCK_STREAM,0);
		if(socketfd<0)
		{
			fprintf(stderr,"File:%s,line:%d,func:%s,errno:%d,strerror:%s\n",__FILE__,__LINE__,__func__,errno,strerror(errno));
			return 0;
		}
		do
		{
			ret=connect(socketfd,(struct sockaddr *)&serveraddr,sizeof(struct sockaddr));
			if(ret!=0)
				printf("无法连接Web监控服务器\n");
			sleep(1);
		}
		while(ret<0);
		heartbeatBuflen=GetHeartbeatInfo(&heartbeatBuf);
		if(0==heartbeatBuflen)
		{
			free(heartbeatBuf);
			fprintf(stderr,"File:%s,line:%d,func:%s,errno:%d,strerror:%s\n",__FILE__,__LINE__,__func__,errno,strerror(errno));
			return 0;
		}
		ret=send(socketfd,heartbeatBuf,heartbeatBuflen,0);
		if(ret<0)
		{
			free(heartbeatBuf);
			fprintf(stderr,"File:%s,line:%d,func:%s,errno:%d,strerror:%s\n",__FILE__,__LINE__,__func__,errno,strerror(errno));
			return 0;
		}
		free(heartbeatBuf);
		close(socketfd);
		sleep(interval);
	}
	return 1;
}

bool SendHeartbeatOnce(char *serverip,uint32_t serverport)
{
	int socketfd,ret;
	struct sockaddr_in serveraddr={0};
	uint8_t *heartbeatBuf;
	uint32_t heartbeatBuflen=0;
	serveraddr.sin_family=AF_INET;
	serveraddr.sin_port=htons(serverport);
	serveraddr.sin_addr.s_addr=inet_addr(serverip);
	socketfd=socket(AF_INET,SOCK_STREAM,0);
	if(socketfd<0)
	{
		fprintf(stderr,"File:%s,line:%d,func:%s,errno:%d,strerror:%s\n",__FILE__,__LINE__,__func__,errno,strerror(errno));
		return 0;
	}
	do
	{
		ret=connect(socketfd,(struct sockaddr *)&serveraddr,sizeof(struct sockaddr));
		if(ret<0)
			printf("无法连接Web监控服务器\n");
		sleep(1);
	}
	while(ret<0);
	heartbeatBuflen=GetHeartbeatInfo(&heartbeatBuf);
	if(0==heartbeatBuflen)
	{
		free(heartbeatBuf);
		fprintf(stderr,"File:%s,line:%d,func:%s,errno:%d,strerror:%s\n",__FILE__,__LINE__,__func__,errno,strerror(errno));
		return 0;
	}
	ret=send(socketfd,heartbeatBuf,heartbeatBuflen,0);
	if(ret<0)
	{
		free(heartbeatBuf);
		fprintf(stderr,"File:%s,line:%d,func:%s,errno:%d,strerror:%s\n",__FILE__,__LINE__,__func__,errno,strerror(errno));
		return 0;
	}
	free(heartbeatBuf);
	close(socketfd);
	return 1;
}

