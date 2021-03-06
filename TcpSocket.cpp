#include "TcpSocket.h"
#include "Common.h"
#include "errno.h"
TcpServer::TcpServer(int listenPort) {
	if ((socket_fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
		throw "socket() failed";
	}

	memset(&myserver, 0, sizeof(myserver));
	myserver.sin_family = AF_INET;
	myserver.sin_addr.s_addr = htonl(INADDR_ANY);
	myserver.sin_port = htons(listenPort);

	int on =1;
	if(setsockopt(socket_fd,SOL_SOCKET,SO_REUSEADDR,(void *)&on,sizeof(on)))
	{
		 throw "setsockopt() failed";
	}

	if (bind(socket_fd, (sockaddr*) &myserver, sizeof(myserver)) < 0) {
		throw "bind() failed";
	}

	if (listen(socket_fd, 10) < 0) {
		throw "listen() failed";
	}

	cout<<"----------系统服务启动，开始接受数据----------"<<endl;

}

bool TcpServer::Accpet() {
	socklen_t sin_size = sizeof(struct sockaddr_in);
	if ((client_fd = accept(socket_fd, (struct sockaddr*) &remote_addr,
			&sin_size)) == -1) {
		throw "Accept error!";
		return false;
	} else {
		printf("开始接受客户端的网络连接： %s\n",
				(char*) inet_ntoa(remote_addr.sin_addr));
		return true;
	}
}

void TcpServer::RecvMsg() {
	char buffer[MAXSIZE];
	memset(buffer, 0, MAXSIZE);
	int len = 0;
 // cout<<"准备开始接收数据..."<<endl;
	while (((len = recv(client_fd, buffer, MAXSIZE, 0)) > 0)
			&& (heartCondition == true)) {
	//	cout<<"接收数据大小： "<<len<<endl;
		RecieveData(buffer, len, client_fd);
		  memset(buffer, 0, MAXSIZE);
	   // cout<<"接收完毕： "<<endl;
	}
	ClearAllTmpData();
	//cout<<"退出断开连接"<<endl;
}

void TcpServer::Start(){
	if(heartCondition==true)
	{
		this->Accpet();
		this->RecvMsg();
		this->Stop();
	}
}

void TcpServer::Stop(){
	 //  cout<<"client socket close ..."<<endl;
		close(client_fd);
	//close(socket_fd);
}

void TcpServer::Close(){
	//close(client_fd);
	close(socket_fd);
}
