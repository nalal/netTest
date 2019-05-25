#include <iostream>
#include <thread> 
#include <algorithm> 
#include <vector>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h> 

#include "packets.h"

using namespace std;

bool running(true);
char message[256] = "This is a test message";
const int maxUsers = 5;
int client_socket;
int server_socket;
vector<chatUser> connected_clients;

void listenFor()
{
	while(running)
	{
		
		listen(server_socket, maxUsers);
		client_socket = accept(server_socket, NULL, NULL);
		cout << "client connected on socket " << client_socket << endl;
		chatUser User;
		User.setCUID(connected_clients.size());
		User.setSocketID(client_socket);
		const char *name;
		name = "name";
		User.setUName(name);
		cout << "uName is " << User.getUName() << endl;
		cout << "CUID is" << User.getCUID() << endl;
		connected_clients.push_back(User);
	}
}

void cPing(chatUser usr)
{
	//cout << "alivecheck test" << endl;
	char buf;
    int err = recv(usr.getSocketID(), &buf, 1, MSG_PEEK);
    if (err == 0)
    {
		cout << "Client died with error " << err << endl;
		int pos = usr.getCUID() - 1;
		connected_clients.erase(connected_clients.begin() + pos);
		
	}
}

void aliveCheck()
{
	while(running)
	{
		//sleep(500);
		if(connected_clients.size() > 1)
		{
			for_each(connected_clients.begin(), connected_clients.end(), cPing);
		}
		else if(connected_clients.size() == 1)
		{
			cPing(connected_clients.front());
		}
	}
}

void mSender(chatUser sock)
{
	send(sock.getSocketID(), message, sizeof(message), 0);
}

void getM()
{
	char recieved[256];
	int retStat;
	while(running)
	{
		while(connected_clients.size() > 0)
		{
			retStat = recv(client_socket, &recieved, sizeof(recieved), 0);
			//cout << retStat << endl;
			if(retStat != 0)
			{
				cout << sizeof(recieved) << endl;
				if(recieved != "")
				{
					//cout << "retStat " << retStat << endl;
					cout << "CLIENT-" << client_socket <<": " << recieved << endl;
					for_each(
						connected_clients.begin(), 
						connected_clients.end(), 
						mSender
					);
				}
			}
		}
	}
}

void sendM()
{
	while(running)
	{
		while(connected_clients.size() != 0)
		{
			cin.getline(message,256);
			if(connected_clients.size() == 1)
			{
				send((connected_clients.front()).getSocketID(), message, sizeof(message), 0);
			}
			else
			{
				for_each(connected_clients.begin(), connected_clients.end(), mSender);
			}
		}
	}
}

int serverLoop()
{
	int retVal = 0;
	int port = 9999;

	


	server_socket = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(port);
	inet_aton("192.168.1.69", &server_address.sin_addr);

	int bindRes = bind(
		server_socket, 
		(struct sockaddr*) &server_address, 
		sizeof(server_address)
	);
	
	if(bindRes != -1)
	{
		cout << "Port " << port << " bound." << endl;
	}
	else
	{
		cout << "Port " << port << " failed to bind, halting server." << endl;
		return 1;
	}
	cout << "SERVER RUNNING" << endl;
	cout << "LISTENING ON PORT " << port << endl;
	
	thread listenForT (listenFor);
	thread usrAliveCheck (aliveCheck);
	thread getMT (getM);
	thread sendMT (sendM);
	while(running)
	{
		sleep(500);
		cout << "ALIVE TICK" << endl;
		//keepAlive
	}

	/*while(running)
	{
		cout << "MSG:";
		cin.getline(message,256);
		cout << "SENDING MESSAGE '" << message << "' TO CLIENT" << endl;
		send(client_socket, message, sizeof(message), 0);
	}
	cout << "CLOSING SOCKET" << endl;
	close(server_socket);
	cout << "SOCKET CLOSED" << endl;
	*/
	return retVal;
}

int main()
{
	int retVal = serverLoop();
	return retVal;
}
