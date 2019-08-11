#include <iostream>
#include <exception>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>

extern int errno;

class Connection{
public:
	class Exception: public std::exception
	{
	public:
		Exception(){
			const char* temp = strerror(errno);
			errorStr = new char[strlen(temp)+1];
			strcpy(errorStr, temp);
		}
		Exception(const Exception& cpy){
			errorStr = new char [strlen(cpy.errorStr)+1];
			strcpy(errorStr, cpy.errorStr);
		}
		Exception(const char* msg){
			const char* temp = strerror(errno);
			errorStr = new char[strlen(temp)+strlen(msg)+1];
			strcpy(errorStr, msg);
			strcat(errorStr, temp);
		}
		virtual ~Exception(){ delete[] errorStr;}

		virtual const char* what() const noexcept{
			return errorStr; 
		}
	private:
		char* errorStr;
	};

	Connection();
	~Connection();
	void createServer(int portNumber);
	void connectToServer(const char* hostName, int portNumber);
	void closeConnection();
	void send(const void* buffer, int length);
	int  receive(void *buffer, int length);
	void waitForConnection(int maxConnections=1);
	void acceptRequest();
	int getPort(){return portNumber;}
private:
	int socketFd;
	int clientSocketFd;
	int portNumber;
	struct sockaddr_in serverAddr;
	struct sockaddr_in clientAddr;
	bool server;	// flags for state...
	bool client;
};

