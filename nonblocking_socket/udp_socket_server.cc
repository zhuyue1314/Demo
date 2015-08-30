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

int UdpSocketServer::bind(const char *ip, const int port)
{
	cout << "UdpSocketServer::bind(ip='" << ip << "', port=" << port << ")" << endl;

	struct sockaddr_in addr;

	memset((char*)&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);

	if (inet_aton(ip, &addr.sin_addr) < 0) {
		perror("inet_aton() failed");
		return -1;
	}

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
	socklen_t addrlen;

	int len = recvfrom(sockfd, buf, sizeof(buf), 0, (struct sockaddr *)&client_addr, &addrlen);
	if (len == -1) {
		perror("recv() failed");
		return -1;
	}
	buf[len] = 0;
	cout << "Get message: " << buf << endl;

	return 0;
}

int UdpSocketServer::send(const char *buf, const int size)
{
	if (sendto(sockfd, buf, size, 0, (struct sockaddr *)&client_addr, sizeof(client_addr)) == -1) {
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
	cout << "\t\t./server ip port" << endl;
	cout << endl;
}

int main(int argc, char **argv)
{
	if (argc != 3) {
		usage();
		return -1;
	}

	UdpSocketServer udp_server;

	udp_server.create_socket();
	udp_server.bind(argv[1], atoi(argv[2]));

	char buf[] = "Welcome to server";
	int i = 1;

	while (1) {
		cout << "[" << i++ << "] ";
		if (udp_server.recv() == -1)
			break;

		if (udp_server.send(buf, strlen(buf)) == -1)
			break;
	}

	return 0;
}
