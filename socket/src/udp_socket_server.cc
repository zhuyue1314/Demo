/*
 * Author   : Kai Zhao
 * Email    : loverszhao@gmail.com
 * Reference:
 */

/*
 * Description:
 */

#include "udp_socket_server.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

using std::cout;
using std::endl;

int UdpSocketServer::bind(const int port)
{
	cout << "UdpSocketServer::bind(port=" << port << ")" << endl;

	struct sockaddr_in local_addr;

	memset((char*)&local_addr, 0, sizeof(local_addr));
	local_addr.sin_family = AF_INET;
	local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	local_addr.sin_port = htons(port);

	if (::bind(sockfd, (struct sockaddr *)&local_addr, sizeof(local_addr)) == -1) {
		perror("bind() failed");
		return -1;
	}
	cout << "\tbind() ok" << endl;

	return 0;
}

static void usage()
{
	cout << endl;
	cout << "======================" << endl;
	cout << "\tThis is udp server" << endl;
	cout << "======================" << endl;
	cout << endl;

	cout << "\tusage:" << endl;
	cout << "\t\t./server port" << endl;
	cout << endl;
}

int main(int argc, char **argv)
{
	if (argc != 2) {
		usage();
		return -1;
	}

	UdpSocketServer udp_server;

	udp_server.create_socket();
	udp_server.bind(atoi(argv[1]));

	char msg[1024];
	int i = 1;

	while (1) {
		cout << "[s] Server is waiting message from client" << endl;

		cout << "[s] [" << i++ << "] ";
		if (udp_server.recv() == -1)
			break;

		cout << "[s] Please enter message which will be sent to the server:" << endl;
		fgets(msg, sizeof(msg), stdin);

		if (udp_server.send(msg, strlen(msg)) == -1)
			break;
	}

	return 0;
}
