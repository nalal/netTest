#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h> 

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>

using namespace std;

int test()
{
	
	int retInt = 0;
	int net_soc;
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
	int test = connect(
		net_soc, 
		(struct sockaddr *) &server_address, 
		sizeof(server_address)
	);
	cout << test << endl;
	
	if(test == -1)
	{
		cout << "CONNECT FAILED" << endl;
	}
	else
	{
		cout << "CONNECT SUCCEDED" << endl;
		char recieved[256];
		recv(net_soc, &recieved, sizeof(recieved), 0);
		cout << "GOT MESSAGE (" << recieved << ")" << endl;
		cout << "CLOSING SOCKET" << endl;
		close(net_soc);
		cout << "SOCKET CLOSED" << endl;
	}
	return retInt;
}

int main()
{
	cout << "runing test" << endl;
	return test();
}
