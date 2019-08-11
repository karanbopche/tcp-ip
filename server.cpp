#include "socketclass.h"


int main(){

	Connection connection;
	char buffer[200];
	connection.createServer(12000);
	connection.waitForConnection(5);
	connection.acceptRequest();
	while(1){
		connection.receive(buffer, sizeof(buffer));
		printf("received: %s\n",buffer);
		if(strncmp("exit", buffer, 4)==0)
			break;
		bzero(buffer, sizeof(buffer));
		printf("Enter reply: ");
		fgets(buffer, sizeof(buffer), stdin);
		connection.send(buffer, strlen(buffer));
		bzero(buffer, sizeof(buffer));
	}
	return 0;
}
