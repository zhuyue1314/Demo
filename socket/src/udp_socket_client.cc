/*
 * Author   : Kai Zhao
 * Email    : loverszhao@gmail.com
 * Reference:
 */

/*
 * Description:
 */

#include "udp_socket_client.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

using std::cout;
using std::endl;

UdpSocketClient::UdpSocketClient()
{
	sockfd = -1;
}

UdpSocketClient::~UdpSocketClient()
{
	if (sockfd != -1)
		close(sockfd);
}

int UdpSocketClient::create_socket()
{
	cout << "UdpSocketClient::create_socket()" << endl;

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd < 0) {
		perror("socket() failed");
		return -1;
	}

	cout << "\tsockfd=" << sockfd << endl;

	return 0;
}

int UdpSocketClient::connect(const char *dst_ip, const int dst_port)
{
	cout << "UdpSocketClient::connect(dst_ip='" << dst_ip << "', dst_port=" << dst_port << ")" << endl;

	memset((char*)&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(dst_port);

	if (inet_aton(dst_ip, &server_addr.sin_addr) < 0) {
		perror("inet_aton() failed");
		return -1;
	}
	cout << "\tconnect() ok" << endl;

	return 0;
}

int UdpSocketClient::recv()
{
	char buf[1024];
	socklen_t addrlen = sizeof(addrlen);

	int len = recvfrom(sockfd, buf, sizeof(buf), 0,
					(struct sockaddr *)&server_addr, &addrlen);
	if (len == -1) {
		perror("recv() failed");
		return -1;
	}
	buf[len] = 0;
	cout << "Get message: " << buf << endl;
}

int UdpSocketClient::send(const char *buf, const int size)
{
	if (sendto(sockfd, buf, size, 0, (struct sockaddr *)&server_addr,
			sizeof(server_addr)) == -1) {
		perror("sendto() failed");
		return -1;
	}

	return 0;
}

static void usage()
{
	cout << endl;
	cout << "======================" << endl;
	cout << "\tThis is udp client" << endl;
	cout << "======================" << endl;
	cout << endl;

	cout << "\tusage:" << endl;
	cout << "\t\t./client ip port" << endl;
	cout << endl;
}

int main(int argc, char **argv)
{
	if (argc != 3) {
		usage();
		return -1;
	}

	UdpSocketClient udp_client;

	udp_client.create_socket();
	udp_client.connect(argv[1], atoi(argv[2]));

	char msg[1024];
	int i = 1;

	while (1) {

		cout << "[c] Please enter message which will be sent to the server:" << endl;
		fgets(msg, sizeof(msg), stdin);

		if (udp_client.send(msg, strlen(msg)) == -1)
			break;

		sleep(2);

		cout << "[c] Client is waiting message from server" << endl;

		cout << "[c] [" << i++ << "] ";
		if (udp_client.recv() == -1)
			break;
	}

	return 0;
}
