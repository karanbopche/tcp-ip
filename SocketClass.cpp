#include "socketclass.h"

Connection::Connection(){
	memset(&serverAddr, 0, sizeof(serverAddr));
	memset(&clientAddr, 0, sizeof(clientAddr));
	socketFd = -1;
	clientSocketFd = -1;
	server =false;
	client = false;
}

Connection::~Connection(){
	closeConnection();
}

void Connection::createServer(int portNumber){
	socketFd = socket(AF_INET, SOCK_STREAM, 0);
	
	if(socketFd<0)
		throw Exception("socket: ");
	
	this->portNumber = portNumber;
	
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = INADDR_ANY;
	serverAddr.sin_port = htons(portNumber);

	if(bind(socketFd,(struct sockaddr*)&serverAddr, sizeof(serverAddr))<0)
		throw Exception("bind: ");
	// binding successful.
	server = true;
}

void Connection::closeConnection(){
	server = false;
	client = false;
	close(socketFd);
	close(clientSocketFd);	
	memset(&serverAddr, 0, sizeof(serverAddr));
	memset(&clientAddr, 0, sizeof(clientAddr));
}
void Connection::waitForConnection(int maxConnections){
	int ret = listen(socketFd, maxConnections);
	if(ret < 0)
		throw Exception("listen: ");
}

void Connection::acceptRequest(){
	socklen_t clientLen;
	clientLen = sizeof(clientAddr);
	clientSocketFd = accept(socketFd, (struct sockaddr*)&clientAddr, &clientLen);
	if(clientSocketFd < 0)
		throw Exception("accept: ");
}

void Connection::send(const void* buffer, int length){
	int sentLen = 0;
	while(sentLen <= length){
		int l = write(clientSocketFd, &((const char*)buffer)[sentLen], length);
		if(l<0)
			throw Exception("write: ");
		sentLen += l;
	}
}

int Connection::receive(void* buffer, int length){
	int ret = read(clientSocketFd, buffer, length);
	if(ret < 0)
		throw Exception("read: ");
	return ret;
}

void Connection::connectToServer(const char* hostName, int port){
	// object is server not client..or already connected to someone.
	if(server == true || client == true)
		return;
	struct hostent *server;

	portNumber = port;
	server = gethostbyname(hostName);
	clientSocketFd = socket(AF_INET, SOCK_STREAM, 0);
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = INADDR_ANY;
	serverAddr.sin_port = htons(portNumber);
	
	if(connect(clientSocketFd, (struct sockaddr*)&serverAddr, sizeof(serverAddr))<0)
		throw Exception("connect: ");
}


