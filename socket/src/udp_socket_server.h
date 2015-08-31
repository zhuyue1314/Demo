#ifndef _UDP_SOCKET_SERVER_H_
#define _UDP_SOCKET_SERVER_H_

#include <arpa/inet.h>

class UdpSocketServer {

public:
	UdpSocketServer();
	~UdpSocketServer();
	int create_socket();
	int bind(const int port);
	int recv();
	int send(const char *buf, const int size);

private:
	int sockfd;
	struct sockaddr_in client_addr;
};

#endif
