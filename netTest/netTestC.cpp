#include <iostream>
#include <thread> 

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h> 
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

using namespace std;

bool running(true);
bool connected;

int retStat = 1;
int test;
int net_soc;

void getM()
{
	char recieved[256];
	while(running)
	{
		retStat = recv(net_soc, &recieved, sizeof(recieved), 0);
		cout << "SERVER: " << recieved << endl;
	}
}

void sendM()
{
	char *message;
	while(running)
	{
		cin.getline(message, 256);
		send(net_soc, message, sizeof(message), 0);
	}
}

void alivePing()
{
	while(connected)
	{
		sleep(500);
		send(net_soc, "P", 1, 0);
	}
}

int networkLoop()
{
	int retInt = 0;
	int port = 9999;
	string instr;
	cout << "IP: ";
	cin >> instr;
	net_soc = socket(AF_INET, SOCK_STREAM, 0);
	const char *ip = instr.c_str();
	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(port);
	inet_aton(ip, &server_address.sin_addr);
	cout << "CONNECTING TO SERVER ON PORT " << port << endl;
	test = connect(
		net_soc, 
		(struct sockaddr *) &server_address, 
		sizeof(server_address)
	);
	cout << "Got connect code " << test << endl;
	
	if(test == -1)
	{
		cout << "CONNECT FAILED" << endl;
	}
	else
	{
		connected = true;
		thread alivePingT (alivePing);
		cout << "CONNECT SUCCEDED" << endl;
		thread getMT (getM);
		thread sendMT (sendM);
		while(running and retStat != 0)
		{
			//keep alive
		}
		cout << "retStat RETURNED " << retStat << endl;
		cout << "CONNECTION LOST" << endl;
		cout << "TERMINATING" << endl;
		running = false;
	}
	return retInt;
}

int main()
{
	cout << "runing test" << endl;
	return networkLoop();
}
