/*
 * Author   : Kai Zhao
 * Email    : loverszhao@gmail.com
 * Reference:
 */

/*
 * Description:
 */

#include "udp_socket.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

using std::cout;
using std::endl;

UdpSocket::UdpSocket()
{
	sockfd = -1;
}

UdpSocket::~UdpSocket()
{
	if (sockfd != -1)
		close(sockfd);
}

int UdpSocket::create_socket()
{
	cout << "UdpSocket::create_socket()" << endl;

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd < 0) {
		perror("socket() failed");
		return -1;
	}

	cout << "\tsockfd=" << sockfd << endl;

	return 0;
}

int UdpSocket::recv()
{
	char buf[1024];
	socklen_t addrlen = sizeof(addr);

	int len = recvfrom(sockfd, buf, sizeof(buf), 0,
					(struct sockaddr *)&addr, &addrlen);
	if (len == -1) {
		perror("recv() failed");
		cout << "sockfd=" << sockfd << endl;
		return -1;
	}
	buf[len] = 0;
	cout << "Get message: " << buf << endl;

	return 0;
}

int UdpSocket::send(const char *buf, const int size)
{
	if (sendto(sockfd, buf, size, 0, (struct sockaddr *)&addr,
			sizeof(addr)) == -1) {
		perror("sendto() failed");
		return -1;
	}

	return 0;
}
