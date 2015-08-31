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

UdpSocketServer::UdpSocketServer()
{
	sockfd = -1;
}

UdpSocketServer::~UdpSocketServer()
{
	if (sockfd != -1)
		close(sockfd);
}

int UdpSocketServer::create_socket()
{
	cout << "UdpSocketServer::create_socket()" << endl;

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd < 0) {
		perror("socket() failed");
		return -1;
	}

	cout << "\tsockfd=" << sockfd << endl;

	return 0;
}

int UdpSocketServer::bind(const int port)
{
	cout << "UdpSocketServer::bind(port=" << port << ")" << endl;

	struct sockaddr_in addr;

	memset((char*)&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(port);

	if (::bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
		perror("bind() failed");
		return -1;
	}
	cout << "\tbind() ok" << endl;

	return 0;
}

int UdpSocketServer::recv()
{
	char buf[1024];
	socklen_t addrlen = sizeof(client_addr);

	int len = recvfrom(sockfd, buf, sizeof(buf), 0,
					(struct sockaddr *)&client_addr, &addrlen);
	if (len == -1) {
		perror("recv() failed");
		cout << "sockfd=" << sockfd << endl;
		return -1;
	}
	buf[len] = 0;
	cout << "Get message: " << buf << endl;

	return 0;
}

int UdpSocketServer::send(const char *buf, const int size)
{
	if (sendto(sockfd, buf, size, 0, (struct sockaddr *)&client_addr,
			sizeof(client_addr)) == -1) {
		perror("sendto() failed");
		return -1;
	}
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
