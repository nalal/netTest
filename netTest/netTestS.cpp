#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h> 

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>

using namespace std;

int serverLoop()
{
	bool running(true);
	int retVal = 0;
	int port = 9999;

	char message[256] = "This is a test message";

	int server_socket;
	server_socket = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(port);
	inet_aton("192.168.1.69", &server_address.sin_addr);

	bind(
		server_socket, 
		(struct sockaddr*) &server_address, 
		sizeof(server_address)
	);
	
	listen(server_socket, 5);
	cout << "SERVER RUNNING" << endl;
	cout << "LISTENING ON PORT " << port << endl;
	while(running)
	{

		int client_socket;
		client_socket = accept(server_socket, NULL, NULL);
		cout << "SENDING MESSAGE '" << message << "' TO CLIENT" << endl;
		send(client_socket, message, sizeof(message), 0);

		/*while(running)
		{

			
			cout << "TIC" << endl;
			sleep(1000); 
		}*/
	}
	cout << "CLOSING SOCKET" << endl;
	close(server_socket);
	cout << "SOCKET CLOSED" << endl;
	return retVal;
}

int main()
{
	int retVal = serverLoop();
	return retVal;
}
