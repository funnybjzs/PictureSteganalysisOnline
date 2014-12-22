#include "TcpSocket.h"
TcpServer::TcpServer(int listenPort) {
	if ((socket_fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
		throw "socket() failed";
	}

	memset(&myserver, 0, sizeof(myserver));
	myserver.sin_family = AF_INET;
	myserver.sin_addr.s_addr = htonl(INADDR_ANY);
	myserver.sin_port = htons(listenPort);

	if (bind(socket_fd, (sockaddr*) &myserver, sizeof(myserver)) < 0) {
		throw "bind() failed";
	}

	if (listen(socket_fd, 10) < 0) {
		throw "listen() failed";
	}

}

bool TcpServer::Accpet() {
	socklen_t sin_size = sizeof(struct sockaddr_in);
	if ((client_fd = accept(socket_fd, (struct sockaddr*) &remote_addr,
			&sin_size)) == -1) {
		throw "Accept error!";
		return false;
	} else {
		printf("Received a client connection from %s\n",
				(char*) inet_ntoa(remote_addr.sin_addr));
		return true;
	}
}

void TcpServer::RecvMsg() {
	char buffer[MAXSIZE];
	memset(buffer, 0, MAXSIZE);
	int len = 0;
	while ((len = recv(client_fd, buffer, MAXSIZE, 0)) > 0) {
          RecieveData(buffer,len,client_fd);
		  memset(buffer, 0, MAXSIZE);
	}
}

void TcpServer::Start(){
	this->Accpet();
	this->RecvMsg();
}

void TcpServer::Stop(){
	close(client_fd);
	//close(socket_fd);
}

void TcpServer::Close(){
	//close(client_fd);
	close(socket_fd);
}
