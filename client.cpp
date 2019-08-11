#include "socketclass.h"


int main(){

	Connection connection;
	char buffer[200];
	connection.connectToServer("127.0.0.1", 12000);
	while(1){
		
		bzero(buffer, sizeof(buffer));
		printf("Enter message to send: ");
		fgets(buffer, sizeof(buffer), stdin);
		connection.send(buffer, strlen(buffer));
		bzero(buffer, sizeof(buffer));
		connection.receive(buffer, sizeof(buffer));
		printf("received: %s\n",buffer);
		if(strncmp("exit", buffer, 4)==0)
			break;
	}
	return 0;
}
